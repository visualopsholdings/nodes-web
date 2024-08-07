import { Component, OnInit, EventEmitter, ViewChild, ElementRef } from '@angular/core';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { MatSnackBar } from '@angular/material/snack-bar';
import { FormGroup, FormBuilder, FormControl, Validators } from '@angular/forms';
import { PageEvent } from '@angular/material/paginator';
import { Router } from '@angular/router';
import { DomSanitizer, SafeResourceUrl, SafeUrl } from '@angular/platform-browser';
import { of, Observable } from 'rxjs';
import { catchError, map } from 'rxjs/operators';
import { HttpEventType, HttpErrorResponse } from '@angular/common/http';

import * as moment from 'moment';

import { InfoService }  from '../info.service';
import { Info }  from '../info';
import { MeService }  from '../me.service';
import { Me }  from '../me';

import { ConfirmComponent } from '../confirm/confirm.component';

@Component({
  selector: 'app-nodes',
  templateUrl: './nodes.component.html',
  styleUrls: ['./nodes.component.css']
})
export class NodesComponent implements OnInit {

  private me: Me;
  private infos: Info[] = [];
  private serverId = null;

  upstreamLastSeen: string;

  constructor(
    public dialog: MatDialog,
    public router: Router,
    private sanitizer: DomSanitizer,
    private infoService: InfoService,
    private meService: MeService,
    private snackBar: MatSnackBar
  ) {
  }

  ngOnInit() {
    this.meService.getMe().subscribe(me => {
        this.me = me
        this.getInfos();
       });
  }

  hasAdmin(): boolean {
    return this.me ? this.me.admin : false;
  }

  hasDefault(): boolean {
    return this.getInfo('serverId') == '6eae665a-f376-472d-87e5-5159c2e0b389';
  }

  private getInfos(): void {
    this.infoService.getInfos().subscribe(infos => {
      this.infos = infos;
      let serverId = infos.find(e => e.type == "serverId");
      if (serverId) {
        this.serverId = serverId.text;
      }
      let upstreamLastSeen = infos.find(e => e.type == "upstreamLastSeen");
      if (upstreamLastSeen) {
        this.upstreamLastSeen = upstreamLastSeen.text;
      }
    });
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

  private isOldTime(time: string): boolean {
    return moment().diff(moment(time), 'seconds') > 20;
  }

  fgColorLastSeenTime(time: string): string {
    return this.isOldTime(time) ? "red" : "black";
  }

}
