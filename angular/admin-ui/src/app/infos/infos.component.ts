import { Component, OnInit, EventEmitter, ViewChild, ElementRef } from '@angular/core';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { catchError, map } from 'rxjs/operators';
import { HttpEventType, HttpErrorResponse } from '@angular/common/http';
import { MatSnackBar } from '@angular/material/snack-bar';
import { of } from 'rxjs';

import { InfoService }  from '../info.service';
import { Info }  from '../info';
import { MeService }  from '../me.service';
import { Me }  from '../me';

import { ConfirmComponent } from '../confirm/confirm.component';

@Component({
  selector: 'app-infos',
  templateUrl: './infos.component.html',
  styleUrls: ['./infos.component.css']
})
export class InfosComponent implements OnInit {

  private me: Me;

  infos: Info[] = [];

  constructor(
    public dialog: MatDialog,
    private snackBar: MatSnackBar,
    private infoService: InfoService,
    private meService: MeService,
  ) { }

  ngOnInit() {
    this.meService.getMe().subscribe(me => {
        this.me = me
        this.getInfos();
      });
  }

  hasAdmin(): boolean {
    return this.me ? this.me.admin : false;
  }

  private getInfos(): void {
    this.infoService.getInfos().subscribe(infos => {
      this.infos = infos
    });
  }

  hasInfos(): boolean {
    return this.infos.length > 0;
  }

  hasInfo(type: string): boolean {
    if (!this.infos) {
      return false;
    }
    return this.infos.find(e => e.type == type) != null;
  }

  hasDefault(): boolean {
    return this.getInfo('serverId') == '6eae665a-f376-472d-87e5-5159c2e0b389';
  }

  getInfo(type: string): string {
    if (!this.infos) {
      return null;
    }
    let info = this.infos.find(e => e.type == type);
    if (info) {
      return info.text;
    }
    return null;
  }

  resetNode(): void {
      this.dialog.open(ConfirmComponent, {
          width: '400px',
          data: { title: "Reset Node",
            description: "Are you sure you want to reset the node? A new serverId and all keys will be reassigned. Only do this" +
            " if you are absolutely sure you need a new server ID. Clicking OK will reboot this server."}
      }).afterClosed().subscribe(success => {
        if (success) {
            this.infoService.updateInfos({ serverId: "none" })
              .subscribe(infos => this.infos = infos);
        }
      });
  }

}
