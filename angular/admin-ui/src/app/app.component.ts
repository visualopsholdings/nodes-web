import { Component, OnInit, EventEmitter, ViewChild } from '@angular/core';
import { MatSidenav } from '@angular/material/sidenav';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { Router } from '@angular/router';
import { MatSnackBar } from '@angular/material/snack-bar';

import { Me }  from './me';
import { MeService }  from './me.service';
import { BootstrapService } from './bootstrap.service';
import { SocketService }  from './socket.service';
import { User }  from './user';
import { StreamQueryResultDialogComponent } from './stream-query-result-dialog/stream-query-result-dialog.component';
import { UserQueryResultDialogComponent } from './user-query-result-dialog/user-query-result-dialog.component';
import { TeamQueryResultDialogComponent } from './team-query-result-dialog/team-query-result-dialog.component';
import { UserService }  from './user.service';
import { TeamService }  from './team.service';
import { Team }  from './team';
import { ConfirmComponent } from './confirm/confirm.component';
import { StreamService }  from './stream.service';
import { Stream }  from './stream';

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
  private onStatus = new EventEmitter<any>();

  constructor(
    private dialog: MatDialog,
    private router: Router,
    private snackBar: MatSnackBar,
    private meService: MeService,
    private userService: UserService,
    private teamService: TeamService,
    private streamService: StreamService,
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
      if (result.objtype == "user") {
        if (result.result.length == 0) {
          this.dialog.open(ConfirmComponent, {
              data: { title: "No users", description: "No users were found." }
          });
        }
        else {
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
      }
      else if (result.objtype == "group") {
        if (result.result.length == 0) {
          this.dialog.open(ConfirmComponent, {
              data: { title: "No groups", description: "No groups were found." }
          });
        }
        else {
          this.dialog.open(TeamQueryResultDialogComponent, { data: { teams: result.result } }).afterClosed().subscribe(result => {
            if (result) {
              result.forEach(team => {
                this.teamService.addTeam({ _id: team, upstream: true } as Team).subscribe(() => {
                  // need to refresh later
                });
              });
              }
          });
        }
      }
      else if (result.objtype == "stream") {
        if (result.result.length == 0) {
          this.dialog.open(ConfirmComponent, {
              data: { title: "No streams", description: "No teams were found." }
          });
        }
        else {
          this.dialog.open(StreamQueryResultDialogComponent, { data: { streams: result.result } }).afterClosed().subscribe(result => {
            if (result) {
              result.forEach(stream => {
                this.streamService.addStream({ _id: stream, upstream: true } as Stream).subscribe(() => {
                  // need to refresh later
                });
              });
              }
          });
        }
      }
      else {
        console.log("Unknown query result type", result.objtype);
      }
    });
    this.onStatus.subscribe(status => {
      this.snackBar.open(status.text, "Hide", { duration: 3000 });
    });
    this.socketService.registerQR("app", this.onQR);
    this.socketService.registerStatus("app", this.onStatus);
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
