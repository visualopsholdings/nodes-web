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
import { NodesService }  from '../nodes.service';
import { Node }  from '../node';
import { SetUpstreamDialogComponent } from '../set-upstream-dialog/set-upstream-dialog.component';
import { AddNodeDialogComponent } from '../add-node-dialog/add-node-dialog.component';
import { SocketService }  from '../socket.service';

@Component({
  selector: 'app-nodes',
  templateUrl: './nodes.component.html',
  styleUrls: ['./nodes.component.css']
})
export class NodesComponent implements OnInit {

  private me: Me;
  private infos: Info[] = [];
  private build: string;
  private serverId = null;

  items: Node[];
  pageSizeOptions = [9, 36, 72];
  pageSize = 9;
  total: number;
  query = "";
  queryCtrl = new FormControl("", []);
  displayedColumns: string[] = [ "name", "serverId", "mirror", "lastSeen", "valid", "actions" ];
  upstreamLastSeen: string;

  private onNodeSeen = new EventEmitter<any>();

  constructor(
    public dialog: MatDialog,
    public router: Router,
    private sanitizer: DomSanitizer,
    private infoService: InfoService,
    private nodesService: NodesService,
    private socketService: SocketService,
    private meService: MeService,
    private snackBar: MatSnackBar
  ) {
  }

  ngOnInit() {
    this.meService.getMe().subscribe(me => {
        this.me = me
        this.startSocket();
        this.getInfos();
        this.getItems(0);
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
      let build = infos.find(e => e.type == "build");
      if (build) {
        this.build = build.text;
      }
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

  getItems(from: number): void {
    if (this.query.length > 0) {
      this.nodesService.searchNodes(this.query)
        .subscribe(items => {
          this.total = -1;
          this.items = items;
        });
    }
    else {
      this.nodesService.getNodes(from, this.pageSize)
        .subscribe(resp => {
          this.total = this.nodesService.getTotal(resp);
          this.items = resp.body;
        });
    }
  }

  goPage(event: PageEvent): void {
    this.pageSize = event.pageSize;
    this.getItems(event.pageIndex * event.pageSize);
  }

  setUpstream(): void {
    let data = { upstream: null, upstreamPubKey: null, upstreamMirror: false };
    for (let i of this.infos) {
      if (i.type == "upstream") {
        data.upstream = i.text;
      }
      else if (i.type == "upstreamPubKey") {
        data.upstreamPubKey = i.text;
      }
      else if (i.type == "upstreamMirror") {
        data.upstreamMirror = i.text == "true";
      }
    }
    this.dialog.open(SetUpstreamDialogComponent, {
        width: '400px',
        data: data
    }).afterClosed().subscribe(result => {
      if (result) {
        if (result.upstream == null || result.upstream.length == 0) {
          if (result.upstreamPubKey == null || result.upstream.length == 0) {
            this.dialog.open(ConfirmComponent, {
                width: '400px',
                data: { title: "Orphan node", description: "Are you sure you want to make this node have no upstream?" }
            }).afterClosed().subscribe(success => {
              if (success) {
                this.infoService.updateInfos({ upstream: "none", upstreamPubKey: "none" })
                  .subscribe(infos => this.infos = infos);
              }
            });
          }
        }
        else if (result.upstreamPubKey == null) {
            this.dialog.open(ConfirmComponent, {
                width: '400px',
                data: { title: "Error", description: "You must set a public key for the upstream node." }
            }).afterClosed().subscribe(success => {});
        }
        else {
          this.infoService.updateInfos(result)
            .subscribe(infos => this.infos = infos);
        }
      }
    });
  }

  add(): void {
    this.dialog.open(AddNodeDialogComponent).afterClosed().subscribe(result => {
      if (result) {
        this.nodesService.addNode(result as Node).subscribe(() => {
            this.getItems(0);
        });
      }
    });
  }

  deleteNode(node: Node): void {
    this.dialog.open(ConfirmComponent, {
        width: '400px',
        data: { title: "Delete Node", description: "Are you sure you want to remove the node permanently?" }
    }).afterClosed().subscribe(success => {
      if (success) {
        this.nodesService.deleteNode(node).subscribe(success => {
          if (success) {
            this.items = this.items.filter(t => t !== node);
          }
        });
      }
    });
  }

  edit(node: Node): void {
    this.router.navigateByUrl(`nodes/${node._id}`);
  }

  search(query) {
    this.query = query;
    this.getItems(0);
  }

  bgColor(item: any): string {
    return item.streamBgColor ? item.streamBgColor : 'white';
  }

  private isBlack(item): boolean {
    return item.streamBgColor && (item.streamBgColor == "#000000" || item.streamBgColor == "black");
  }

  fgColor(item: any): string {
    return this.isBlack(item) ? 'white' : 'black';
  }

  private isOldBuild(item: any): boolean {
    return !(item.build == this.build || parseInt(item.build) > parseInt(this.build));
  }

  private isOldTime(time: string): boolean {
    return moment().diff(moment(time), 'seconds') > 20;
  }

  fgColorLastSeenTime(time: string): string {
    return this.isOldTime(time) ? "red" : "black";
  }

  fgColorLastSeen(item: any): string {
    return this.isOldTime(item.lastSeen) ? "red" : "black";
  }

  fgColorBuild(item: any): string {
    return this.isOldBuild(item) ? "orange" : "black";
  }

  urlIsOurs(url: string): boolean {
    return url == 'https://local.visualops.com';
  }

  private startSocket(): void {
    this.onNodeSeen.subscribe(status => {
      if (status.upstreamLastSeen) {
        this.upstreamLastSeen = status.upstreamLastSeen;
      }
      else {
        var node = this.items.filter(e => e.serverId == status.serverId);
        if (node.length > 0) {
          node[0].lastSeen = status.lastSeen;
        }
      }
    });
    this.socketService.registerNodeSeen("nodes", this.onNodeSeen);
  }

}
