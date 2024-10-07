import { Component, OnInit, Input } from '@angular/core';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { ActivatedRoute } from '@angular/router';
import { Location } from '@angular/common';
import { MatSnackBar } from '@angular/material/snack-bar';

import { IconService }  from '../icon.service';
import { InfoService }  from '../info.service';
import { Info } from '../info';
import { MeService }  from '../me.service';
import { Me }  from '../me';
import { ConfirmComponent } from '../confirm/confirm.component';
import { NodesService }  from '../nodes.service';
import { Node }  from '../node';
import { UserService }  from '../user.service';
import { User }  from '../user';

@Component({
  selector: 'app-node-detail',
  templateUrl: './node-detail.component.html',
  styleUrls: ['./node-detail.component.css']
})
export class NodeDetailComponent implements OnInit {

  @Input() node: Node;

  private me: Me;
  private serverId: string;

  constructor(
    public dialog: MatDialog,
    private route: ActivatedRoute,
    private nodesService: NodesService,
    private meService: MeService,
    private userService: UserService,
    private infoService: InfoService,
    private location: Location,
    private snackBar: MatSnackBar
  ) {}

  ngOnInit() {
    this.meService.getMe().subscribe(me => {
        this.me = me;
        this.getInfos();
        this.getNode();
     });
  }

  hasAdmin(): boolean {
    return this.me.admin;
  }

  private getNode(): void {
    const id = this.route.snapshot.paramMap.get('id');
    this.nodesService.getNode(id).subscribe(node => {
      this.node = node
      this.node.userobjs = [];
      if (this.node.users) {
        this.node.users.forEach(u => {
          this.userService.getUserNoError(u).subscribe(user => {
            if (user) {
              this.node.userobjs.push(user);
            }
            else {
              this.node.userobjs.push({ _id: u, fullname: "Unknown" });
            }
          });
        });
      }
    });
  }

  private getInfos(): void {
    this.infoService.getInfos().subscribe(infos => {
      var serverId = infos.filter(e => e.type == "serverId");
      this.serverId = serverId.length > 0 ? serverId[0].text : null;
    });
  }

}
