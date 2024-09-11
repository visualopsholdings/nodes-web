import { Component, OnInit } from '@angular/core';
import { MatDialogRef } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';

import { Observable ,  Subject ,  of } from 'rxjs';

import {
   debounceTime, distinctUntilChanged, switchMap
 } from 'rxjs/operators';

import { Team } from '../team';
import { TeamService } from '../team.service';
import { MeService }  from '../me.service';
import { Me }  from '../me';

@Component({
  selector: 'app-add-group-policy-dialog',
  templateUrl: './add-group-policy-dialog.component.html',
  styleUrls: ['./add-group-policy-dialog.component.css']
})
export class AddGroupPolicyDialogComponent implements OnInit {

  private me: Me;
  form: FormGroup;
  teams$: Observable<Team[]>;
  private hasSelection = false;
  private searchTerms = new Subject<string>();
  displayedColumns: string[] = [ "name", "view", "edit", "all" ];

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<AddGroupPolicyDialogComponent>,
    private teamService: TeamService,
    private meService: MeService
  ) {}

  ngOnInit(): void {
    this.meService.getMe()
      .subscribe(me => {
        this.me = me;
     });
    this.form = this.formBuilder.group({}, {});
    this.teams$ = this.searchTerms.pipe(
      // wait 300ms after each keystroke before considering the term
      debounceTime(300),

      // ignore new term if same as previous term
      distinctUntilChanged(),

      // switch to new search observable each time the term changes
      switchMap((term: string) => this.teamService.searchTeams(this.hasAdmin(), term)),
    );
  }

  hasAdmin(): boolean {
    return this.me.admin;
  }

  search(term: string): void {
    this.searchTerms.next(term);
  }

  selectionChanged(change): void {
    this.hasSelection = change.source.selectedOptions.selected.length > 0;
  }

  addRole(role: string, team: Team): void {
    this.dialogRef.close({ role: role, team: team });
  }
}
