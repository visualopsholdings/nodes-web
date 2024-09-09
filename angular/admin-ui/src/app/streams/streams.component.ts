import { Component, OnInit, Inject, EventEmitter } from '@angular/core';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { PageEvent } from '@angular/material/paginator';
import { Router } from '@angular/router';
import { FormGroup, FormBuilder, FormControl, Validators } from '@angular/forms';
import { MatSnackBar } from '@angular/material/snack-bar';

import { Stream } from '../stream';
import { StreamService } from '../stream.service';
import { AddStreamDialogComponent } from '../add-stream-dialog/add-stream-dialog.component';
import { ConfirmComponent } from '../confirm/confirm.component';
import { IconService }  from '../icon.service';
import { MeService }  from '../me.service';
import { Me }  from '../me';
import { InfoService }  from '../info.service';
import { Info }  from '../info';
import { SocketService }  from '../socket.service';

@Component({
  selector: 'app-streams',
  templateUrl: './streams.component.html',
  styleUrls: ['./streams.component.css']
})
export class StreamsComponent implements OnInit {

  private me: Me;
  items: any[];
  pageSizeOptions = [9, 36, 72];
  pageSize = 9;
  total: number;
  displayedColumns: string[] = [ "icon", "emoji", "name", "id", "date", "actions" ];

  constructor(
    public dialog: MatDialog,
    public router: Router,
    private formBuilder: FormBuilder,
    private snackBar: MatSnackBar,
    private streamService: StreamService,
    private iconService: IconService,
    private meService: MeService,
    private infoService: InfoService,
    private socketService: SocketService,
  )
  {}

  ngOnInit() {
    this.meService.getMe()
      .subscribe(me => {
        this.me = me;
        this.getItems(0);
     });
  }

  hasAdmin(): boolean {
    return this.me && this.me.admin;
  }

  getItems(from: number): void {
    this.streamService.getStreams(this.hasAdmin(), from, this.pageSize)
      .subscribe(resp => {
        this.total = this.streamService.getTotal(resp);
        this.items = resp.body;
      });
  }

  goPage(event: PageEvent): void {
    this.pageSize = event.pageSize;
    this.getItems(event.pageIndex * event.pageSize);
  }

  add(): void {
    this.dialog.open(AddStreamDialogComponent).afterClosed().subscribe(result => {
      if (result) {
        if (result._id == '') {
          result._id = undefined;
        }
        this.streamService.addStream(result as Stream).subscribe(() => {
            this.getItems(0);
        });
      }
    });
  }

  delete(item: any): void {
    this.dialog.open(ConfirmComponent, {
        width: '400px',
        data: { title: "Delete Stream", description: "Are you sure you want to remove the stream permanently?" }
    }).afterClosed().subscribe(success => {
      if (success) {
        this.streamService.deleteStream(item).subscribe(success => {
          if (success) {
            this.items = this.items.filter(t => t !== item);
          }
        });
      }
    });
  }

  edit(item: any): void {
    this.router.navigateByUrl(`streams/${item._id}`);
  }

  getIcon(item: any): string {
    return this.iconService.getIcon(item);
  }

}
