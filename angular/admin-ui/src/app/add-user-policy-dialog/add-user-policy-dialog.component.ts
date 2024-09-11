import { Component, OnInit } from '@angular/core';
import { MatDialogRef } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';

import { Observable ,  Subject ,  of } from 'rxjs';

import {
   debounceTime, distinctUntilChanged, switchMap
 } from 'rxjs/operators';

import { User } from '../user';
import { UserService } from '../user.service';

@Component({
  selector: 'app-add-user-policy-dialog',
  templateUrl: './add-user-policy-dialog.component.html',
  styleUrls: ['./add-user-policy-dialog.component.css']
})
export class AddUserPolicyDialogComponent implements OnInit {

  form: FormGroup;
  users$: Observable<User[]>;
  private hasSelection = false;
  private searchTerms = new Subject<string>();
  displayedColumns: string[] = [ "fullname", "view", "edit", "all" ];

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<AddUserPolicyDialogComponent>,
    private userService: UserService
  ) {}

  ngOnInit(): void {
    this.form = this.formBuilder.group({}, {});
    this.users$ = this.searchTerms.pipe(
      // wait 300ms after each keystroke before considering the term
      debounceTime(300),

      // ignore new term if same as previous term
      distinctUntilChanged(),

      // switch to new search observable each time the term changes
      switchMap((term: string) => this.userService.searchUsers(term)),
    );
  }

  search(term: string): void {
    this.searchTerms.next(term);
  }

  selectionChanged(change): void {
    this.hasSelection = change.source.selectedOptions.selected.length > 0;
  }

  addRole(role: string, user: User): void {
    this.dialogRef.close({ role: role, user: user });
  }
}
