import { Component, OnInit, Input } from '@angular/core';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { ActivatedRoute } from '@angular/router';
import { Location } from '@angular/common';
import { MatSnackBar } from '@angular/material/snack-bar';

import { Team } from '../team';
import { TeamService }  from '../team.service';
import { TeamMember } from '../team-member';
import { AddMemberDialogComponent } from '../add-member-dialog/add-member-dialog.component';
import { ConfirmComponent } from '../confirm/confirm.component';
import { IconService }  from '../icon.service';
import { MeService }  from '../me.service';
import { Me }  from '../me';
import { InfoService }  from '../info.service';

@Component({
  selector: 'app-team-detail',
  templateUrl: './team-detail.component.html',
  styleUrls: ['./team-detail.component.css']
})
export class TeamDetailComponent implements OnInit {

  private me: Me;
  private serverId: string;

  team = new Team();
  members: TeamMember[];
  displayedColumns: string[] = [ "id", "fullname", "actions" ];

  constructor(
    public dialog: MatDialog,
    private route: ActivatedRoute,
    private location: Location,
    private snackBar: MatSnackBar,
    private teamService: TeamService,
    private iconService: IconService,
    private meService: MeService,
    private infoService: InfoService,
  ) {}

  ngOnInit() {
    this.meService.getMe()
      .subscribe(me => {
        this.me = me;
        this.getTeam();
        this.getMembers();
     });
  }

  hasAdmin(): boolean {
    return this.me && this.me.admin;
  }

  getMembers(): void {
    const id = this.route.snapshot.paramMap.get('id');
    this.teamService.getTeamMembers(this.hasAdmin(), id)
      .subscribe(members => this.members = members);
  }

  getTeam(): void {
    const id = this.route.snapshot.paramMap.get('id');
    this.teamService.getTeam(this.hasAdmin(), id)
      .subscribe(team => this.team = team);
  }

  removeMember(member: TeamMember): void {
    this.dialog.open(ConfirmComponent, {
        width: '400px',
        data: { title: "Remove Member", description: "Are you sure you want to remove a member from the team?" }
    }).afterClosed().subscribe(success => {
      if (success) {
        const id = this.route.snapshot.paramMap.get('id');
        this.teamService.removeTeamMember(id, member).subscribe(success => {
          this.members = this.members.filter(t => t !== member);
        });
      }
    });
  }

  addMembers(): void {
    this.dialog.open(AddMemberDialogComponent).afterClosed().subscribe(result => {
      if (result) {
        const id = this.route.snapshot.paramMap.get('id');
        this.teamService.addTeamMember(id, result as TeamMember)
          .subscribe(() => {
            this.getMembers();
          });
      }
    });
  }

  getIcon(item: any): string {
    return this.iconService.getIcon({ icon: "internal:group" });
  }

}

