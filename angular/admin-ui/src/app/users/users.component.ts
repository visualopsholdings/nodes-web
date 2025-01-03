import { Component, OnInit, Inject, EventEmitter } from '@angular/core';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { PageEvent } from '@angular/material/paginator';
import { Router } from '@angular/router';
import { FormGroup, FormBuilder, FormControl, Validators } from '@angular/forms';

import { User } from '../user';
import { UserService } from '../user.service';
import { IconService } from '../icon.service';
import { MeService }  from '../me.service';
import { Me }  from '../me';
import { InfoService }  from '../info.service';
import { Info }  from '../info';
import { AddUpstreamUserDialogComponent } from '../add-upstream-user-dialog/add-upstream-user-dialog.component';
import { SocketService }  from '../socket.service';
import { ConfirmComponent } from '../confirm/confirm.component';

@Component({
  selector: 'app-users',
  templateUrl: './users.component.html',
  styleUrls: ['./users.component.css']
})
export class UsersComponent implements OnInit {

  private me: Me;
  items: any[];
  pageSizeOptions = [9, 36, 72];
  pageSize = 9;
  total: number = 0;
  displayedColumns: string[] = [ "icon", "id", "name", "active", "upstream", "actions" ];
  hasUpstream = false;
  purgeCount = 0;

  private onStatus = new EventEmitter<any>();

  constructor(
    public dialog: MatDialog,
    public router: Router,
    private formBuilder: FormBuilder,
    private userService: UserService,
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
        this.infoService.getInfos().subscribe(infos => {
          this.hasUpstream = infos.filter(e => e.type == "upstream").length > 0;
        });
        this.startSocket();
        this.updatePurgeCount();
     });
  }

  private startSocket(): void {
    this.onStatus.subscribe(status => {
      this.getItems(0);
    });
    this.socketService.registerStatus("users", this.onStatus);
  }

  hasAdmin(): boolean {
    return this.me ? this.me.admin : false;
  }

  getItems(from: number): void {
    this.userService.getUsers(this.hasAdmin(), from, this.pageSize)
      .subscribe(resp => {
        this.total = this.userService.getTotal(resp);
        this.items = resp.body;
      });
  }

  goPage(event: PageEvent): void {
    this.pageSize = event.pageSize;
    this.getItems(event.pageIndex * event.pageSize);
  }

  edit(item: any): void {
    this.router.navigateByUrl(`users/${item._id}`);
  }

  getIcon(item: any): string {
    return this.iconService.getIcon({ icon: "internal:user" });
  }

  delete(item: any): void {
    this.dialog.open(ConfirmComponent, {
        width: '400px',
        data: { title: "Delete User", description: "Are you sure you want to remove the user? " +
          "If the user is upstream, it will only be removed from this node." }
    }).afterClosed().subscribe(success => {
      if (success) {
        this.userService.deleteUser(item).subscribe(success => {
          if (success) {
            this.items = this.items.filter(t => t !== item);
            this.updatePurgeCount();
          }
        });
      }
    });
  }

  queryUpstream(): void {
    this.dialog.open(AddUpstreamUserDialogComponent).afterClosed().subscribe(result => {
        if (result) {
          result.query = true;
          this.userService.addUser(result as User).subscribe(() => {
            // need to refresh later
          });
        }
    });
  }

  private updatePurgeCount() {
    if (this.hasAdmin()) {
      this.userService.getPurgeCount().subscribe(count => {
        this.purgeCount = count.count;
      });
    }
  }

  purge(): void {
    this.dialog.open(ConfirmComponent, { width: '400px',
        data: { title: "Purge Users", description: "Are you sure you want to purge " + this.purgeCount + " users permanently?" }
    }).afterClosed().subscribe(success => {
      if (success) {
        this.userService.purge().subscribe(() => {
          this.updatePurgeCount();
        });
      }
    });
  }

}
