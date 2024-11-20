import { Component, OnInit, Input } from '@angular/core';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { ActivatedRoute } from '@angular/router';
import { Clipboard } from '@angular/cdk/clipboard';
import { MatSnackBar } from '@angular/material/snack-bar';

import { Stream } from '../stream';
import { StreamService }  from '../stream.service';
import { IconService }  from '../icon.service';
import { MeService }  from '../me.service';
import { Me }  from '../me';
import { InfoService }  from '../info.service';
import { SetEmojiDialogComponent } from '../set-emoji-dialog/set-emoji-dialog.component';
import { ConfirmComponent } from '../confirm/confirm.component';
import { StreamFlags } from '../../../../shared-ui/streamflags';
import { SetUibitsDialogComponent } from '../set-uibits-dialog/set-uibits-dialog.component';
import { IdeaService }  from '../idea.service';

@Component({
  selector: 'app-stream-detail',
  templateUrl: './stream-detail.component.html',
  styleUrls: ['./stream-detail.component.css']
})
export class StreamDetailComponent implements OnInit {

  @Input() stream: Stream;

  private me: Me;
  private serverId: string;

  streamflags: StreamFlags;
  purgeCount = 0;

  constructor(
    public dialog: MatDialog,
    private route: ActivatedRoute,
    private clipboard: Clipboard,
    private snackBar: MatSnackBar,
    private streamService: StreamService,
    private ideaService: IdeaService,
    private iconService: IconService,
    private meService: MeService,
    private infoService: InfoService,
  ) {}

  ngOnInit() {
    this.meService.getMe()
      .subscribe(me => {
        this.me = me;
        this.getStream();
        this.getInfos();
     });
  }

  hasAdmin(): boolean {
    return this.me.admin;
  }

  private getInfos(): void {
    this.infoService.getInfos().subscribe(infos => {
      var serverId = infos.filter(e => e.type == "serverId");
      this.serverId = serverId.length > 0 ? serverId[0].text : null;
    });
  }

  getStream(): void {
    const id = this.route.snapshot.paramMap.get('id');
    this.streamService.getStream(this.hasAdmin(), id)
      .subscribe(stream => {
        this.stream = stream;
        this.streamflags = new StreamFlags(this.stream.streambits);
        this.updatePurgeCount();
    });
  }

  editEmoji(): void {
    this.dialog.open(SetEmojiDialogComponent, {
        width: '400px',
        data: { emoji: this.stream.emoji }
    }).afterClosed().subscribe(result => {
      if (result) {
        this.streamService.updateStream(false, { _id: this.stream._id, emoji: result.emoji } as Stream)
          .subscribe(() => {
         this.getStream();
       });
      }
    });
  }

  save(): void {
    this.streamService.updateStream(false, { _id: this.stream._id, name: this.stream.name, streambits: this.stream.streambits} as Stream)
      .subscribe(() => {});
  }

  getIcon(stream: Stream): string {
    return this.iconService.getIcon(stream);
  }

  streamChange(): void {
    var stream = { _id: this.stream._id } as Stream;
    stream.streambits = this.streamflags.bits();
    this.streamService.updateStream(false, stream).subscribe(() => {
      this.stream.streambits = stream.streambits;
    });
  }

  setStreamBits(): void {
    let dialogRef = this.dialog.open(SetUibitsDialogComponent, { data: {
      title: "Set Stream Bits",
      uibits: this.stream.streambits
    } });
    dialogRef.afterClosed().subscribe(result => {
      if (result) {
        this.streamflags.update(result.uibits);
        this.streamChange();
      }
    });
  }

  private updatePurgeCount() {
    if (this.hasAdmin()) {
      this.ideaService.getPurgeCount(this.stream._id).subscribe(count => {
        this.purgeCount = count.count;
      });
    }
  }

  purge(): void {
    this.dialog.open(ConfirmComponent, { width: '400px',
        data: { title: "Purge Ideas", description: "Are you sure you want to purge " + this.purgeCount + " ideas permanently?" }
    }).afterClosed().subscribe(success => {
      if (success) {
        this.ideaService.purge(this.stream._id).subscribe(() => {
          this.updatePurgeCount();
        });
      }
    });
  }

}

