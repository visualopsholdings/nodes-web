import { Component, OnInit, EventEmitter, ViewChild } from '@angular/core';
import { MatSidenav } from '@angular/material/sidenav';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { Router } from '@angular/router';

import { Me }  from './me';
import { MeService }  from './me.service';
import { BootstrapService } from './bootstrap.service';
import { SocketService }  from './socket.service';
import { User }  from './user';
import { UserQueryResultDialogComponent } from './user-query-result-dialog/user-query-result-dialog.component';
import { UserService }  from './user.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit {

  @ViewChild('sidenav') public sidenav: MatSidenav;

  me: Me;
  title = 'admin-ui';
  height = "300px";
  opened = true;

  private onSocket = new EventEmitter<any>();
  private onQR = new EventEmitter<any>();

  constructor(
    private dialog: MatDialog,
    private router: Router,
    private meService: MeService,
    private userService: UserService,
    private bootstrapService: BootstrapService,
    private socketService: SocketService,
  ) {
  }

  ngOnInit() {

    this.bootstrapService.meChanged$.subscribe(me => {

      if (me) {
        [this.me] = me;
        this.startSocket();
        this.onResize();
      }

    });

  }

  onResize() {
    this.height = window.innerHeight + "px";
  }

  private startSocket(): void {
    if (this.me) {
      this.socketService.start(this.me, console, this.onSocket);
    }
    else {
      this.socketService.startDown(console, this.onSocket);
    }
    this.onQR.subscribe(result => {
      if (result.queryType == "user") {
        this.dialog.open(UserQueryResultDialogComponent, { data: { users: result.result } }).afterClosed().subscribe(result => {
          if (result) {
            result.forEach(user => {
              this.userService.addUser({ _id: user, upstream: true } as User).subscribe(() => {
                // need to refresh later
              });
            });
          }
        });
      }
      else {
        console.log("Unknown query result type", result.queryType);
      }
    });
    this.socketService.registerQR("app", this.onQR);
    this.socketService.open(this.me, "system");
  }

  hasAdmin(): boolean {
    return this.me && this.me.admin;
  }

  canAdmin(): boolean {
    return this.me != null;
  }

  headerTitle(): string {
    return "Nodes";
  }

  version(): string {
    return "0.1";
  }

  logoff() {
    window.open("/logout", "_self");
  }

  close(): void {
    this.opened = false;
    this.sidenav.close();
  }
  open(): void {
    this.opened = true;
    this.sidenav.open();
  }

  gotoApp(name: string, newwindow = false) {
    window.open("../" + name, newwindow ? "_blank" : "_self");
  }

}
