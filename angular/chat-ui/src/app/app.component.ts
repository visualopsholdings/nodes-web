import { Component, OnInit, EventEmitter, ViewChild } from '@angular/core';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { Router } from '@angular/router';

import { Me }  from './me';
import { MeService }  from './me.service';
import { BootstrapService } from './bootstrap.service';
import { Site }  from './site';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {

  site: Site;
  me: Me;
  height = "300px";
  title = 'chat-ui';
  convName = "Chat";

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
        [this.me, this.site] = me;
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

  version(): string {
    return "0.1";
  }

  logoff() {
    window.open("/logout", "_self");
  }

  gotoApp(name: string, newwindow = false) {
    window.open("../" + name, newwindow ? "_blank" : "_self");
  }

  headerTitle(): string {
    return this.site ? (this.site.headerTitle ? this.site.headerTitle : "Visual Ops") : "";
  }

  streamBgColor(): string {
    return this.site ? (this.site.streamBgColor ? this.site.streamBgColor : "#7096cc") : "#FFFFFF";
  }

}
