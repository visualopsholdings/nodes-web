import { Component, OnInit, Input, EventEmitter } from '@angular/core';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { ActivatedRoute } from '@angular/router';
import { Location } from '@angular/common';
import { MatSnackBar } from '@angular/material/snack-bar';

import { Site } from '../site';
import { SiteService }  from '../site.service';
import { IconService }  from '../icon.service';
import { MeService }  from '../me.service';
import { Me }  from '../me';
import { ConfirmComponent } from '../confirm/confirm.component';

@Component({
  selector: 'app-site-detail',
  templateUrl: './site-detail.component.html',
  styleUrls: ['./site-detail.component.css']
})
export class SiteDetailComponent implements OnInit {

  @Input() site: Site;

  private me: Me;

  constructor(
    public dialog: MatDialog,
    private route: ActivatedRoute,
    private siteService: SiteService,
    private iconService: IconService,
    private meService: MeService,
    private location: Location,
    private snackBar: MatSnackBar
  ) {}

  ngOnInit() {
    this.meService.getMe().subscribe(me => {
        this.me = me;
        this.getSite();
     });
  }

  hasAdmin(): boolean {
    return this.me.admin;
  }

  getSite(): void {
    this.siteService.getSite()
      .subscribe(site => this.site = site);
  }

  save(): void {
    this.siteService.updateSite(this.site)
      .subscribe(() => {});
  }

  getIcon(site: Site): string {
    return this.iconService.getIcon(site);
  }

}
