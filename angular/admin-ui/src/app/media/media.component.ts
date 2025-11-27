import { Component, OnInit, Inject } from '@angular/core';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { PageEvent } from '@angular/material/paginator';
import { Router } from '@angular/router';
import { FormGroup, UntypedFormBuilder, UntypedFormControl, Validators } from '@angular/forms';

import { Media } from '../media';
import { MediaService } from '../media.service';
import { IconService }  from '../icon.service';
import { ConfirmComponent } from '../confirm/confirm.component';
import { MeService }  from '../me.service';
import { Me }  from '../me';
import { Info }  from '../info';
import { InfoService }  from '../info.service';
import { BootstrapService } from '../bootstrap.service';
import { Site } from '../site';

@Component({
  selector: 'app-media',
  templateUrl: './media.component.html',
  styleUrls: ['./media.component.css']
})
export class MediaComponent implements OnInit {

  private me: Me;
  private serverId: string;

  items: Media[];
  pageSizeOptions = [9, 36, 72];
  pageSize = 9;
  total: number;
  displayedColumns: string[] = [ "icon", "id", "name", "uuid", "date", "upstream", "actions" ];
  hasUpstream = false;
  isMirror = false;
  purgeCount = 0;

  constructor(
    public dialog: MatDialog,
    public router: Router,
    private formBuilder: UntypedFormBuilder,
    private mediaService: MediaService,
    private iconService: IconService,
    private infoService: InfoService,
    private meService: MeService,
    private bootstrapService: BootstrapService,
  )
  { }

  ngOnInit() {
    this.meService.getMe()
      .subscribe(me => {
        this.me = me;
        this.getInfos();
        this.getItems(0);
        this.updatePurgeCount();
     });
  }

  hasAdmin(): boolean {
    return this.me && this.me.admin;
  }

  private getInfos(): void {
    this.infoService.getInfos().subscribe(infos => {
      var mirror = infos.filter(e => e.type == "upstreamMirror");
      this.isMirror = mirror.length > 0 ? mirror[0].text == "true" : false;
      var serverId = infos.filter(e => e.type == "serverId");
      this.serverId = serverId.length > 0 ? serverId[0].text : null;
      this.hasUpstream = infos.filter(e => e.type == "upstream").length > 0;
    });
  }

  getItems(from: number): void {
    this.mediaService.getMedia(this.hasAdmin(), from, this.pageSize).subscribe(resp => {
      this.total = this.mediaService.getTotal(resp);
      this.items = resp.body;
    });
  }

  goPage(event: PageEvent): void {
    this.pageSize = event.pageSize;
    this.getItems(event.pageIndex * event.pageSize);
  }

  delete(item: Media): void {
    this.dialog.open(ConfirmComponent, {
        width: '400px',
        data: { title: "Delete Media", description: "Are you sure you want to remove the media permanently?" }
    }).afterClosed().subscribe(success => {
      if (success) {
        this.mediaService.deleteMedia(this.hasAdmin(), item).subscribe(success => {
          if (success) {
            this.items = this.items.filter(t => t !== item);
            this.updatePurgeCount();
          }
        });
      }
    });
  }

  getIcon(item: Media): string {
    return this.iconService.getIcon({ icon: "internal:media" });
  }

  queryUpstream(): void {
//     this.dialog.open(AddUpstreamMediaDialogComponent).afterClosed().subscribe(result => {
//         if (result) {
//           result.query = true;
//           this.mediaService.queryUpstreamMedia(result.name).subscribe(() => {
//             // need to refresh later
//           });
//         }
//     });
  }

  private updatePurgeCount() {
    if (this.hasAdmin()) {
      this.mediaService.getPurgeCount().subscribe(count => {
        this.purgeCount = count.count;
      });
    }
  }

  purge(): void {
    this.dialog.open(ConfirmComponent, { width: '400px',
        data: { title: "Purge Media", description: "Are you sure you want to purge " + this.purgeCount + " media permanently?" }
    }).afterClosed().subscribe(success => {
      if (success) {
        this.mediaService.purge().subscribe(() => {
          this.updatePurgeCount();
        });
      }
    });
  }

}
