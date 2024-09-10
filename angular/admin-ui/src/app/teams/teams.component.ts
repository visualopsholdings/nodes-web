import { Component, OnInit, Inject } from '@angular/core';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { PageEvent } from '@angular/material/paginator';
import { Router } from '@angular/router';
import { FormGroup, FormBuilder, FormControl, Validators } from '@angular/forms';

import { Team } from '../team';
import { TeamService } from '../team.service';
import { ConfirmComponent } from '../confirm/confirm.component';
import { IconService }  from '../icon.service';
import { MeService }  from '../me.service';
import { Me }  from '../me';
import { InfoService }  from '../info.service';
import { Info }  from '../info';
import { AddTeamDialogComponent } from '../add-team-dialog/add-team-dialog.component';

@Component({
  selector: 'app-teams',
  templateUrl: './teams.component.html',
  styleUrls: ['./teams.component.css']
})
export class TeamsComponent implements OnInit {

  private me: Me;
  items: Team[];
  pageSizeOptions = [9, 36, 72];
  pageSize = 9;
  total: number;
  displayedColumns: string[] = [ "icon", "name", "id", "date", "actions" ];

  constructor(
    public dialog: MatDialog,
    public router: Router,
    private formBuilder: FormBuilder,
    private teamService: TeamService,
    private iconService: IconService,
    private infoService: InfoService,
    private meService: MeService)
  {}

  ngOnInit() {
    this.meService.getMe()
      .subscribe(me => {
        this.me = me;
        this.getItems(0);
     });
  }

  hasAdmin(): boolean {
    return this.me.admin;
  }

  getItems(from: number): void {
    this.teamService.getTeams(this.hasAdmin(), from, this.pageSize)
      .subscribe(resp => {
        this.total = this.teamService.getTotal(resp);
        this.items = resp.body;
      });
  }

  goPage(event: PageEvent): void {
    this.pageSize = event.pageSize;
    this.getItems(event.pageIndex * event.pageSize);
  }

  edit(item: Team): void {
    this.router.navigateByUrl(`teams/${item._id}`);
  }

  getIcon(item: any): string {
    return this.iconService.getIcon({ icon: "internal:group" });
  }

  add(): void {
    this.dialog.open(AddTeamDialogComponent).afterClosed().subscribe(result => {
      if (result) {
        if (result._id == '') {
          result._id = undefined;
        }
        this.teamService.addTeam(result as Team).subscribe(() => {
          this.getItems(0);
        });
      }
    });
  }

  delete(item: Team): void {
    this.dialog.open(ConfirmComponent, {
        width: '400px',
        data: { title: "Delete Team", description: "Are you sure you want to remove the team permanently?" }
    }).afterClosed().subscribe(success => {
      if (success) {
        this.teamService.deleteTeam(item).subscribe(success => {
          if (success) {
            this.items = this.items.filter(t => t !== item);
          }
        });
      }
    });
  }

}
