import { Component, OnInit, EventEmitter, ViewChild } from '@angular/core';
import { MatSidenav } from '@angular/material/sidenav';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { Router } from '@angular/router';

import { Me }  from './me';
import { MeService }  from './me.service';
import { BootstrapService } from './bootstrap.service';

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

  constructor(
    private dialog: MatDialog,
    private router: Router,
    private meService: MeService,
    private bootstrapService: BootstrapService,
  ) {
  }

  ngOnInit() {

    this.bootstrapService.meChanged$.subscribe(me => {

      if (me) {
        [this.me] = me;
        this.onResize();
      }

    });

  }

  onResize() {
    this.height = window.innerHeight + "px";
  }

  canAdmin(): boolean {
    return this.me != null;
  }

  headerTitle(): string {
    return "ZMQChat";
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
