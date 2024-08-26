import { Component, OnInit } from '@angular/core';
import { MatDialogRef } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';

import { MeService }  from '../me.service';
import { Me }  from '../me';

@Component({
  selector: 'app-add-team-dialog',
  templateUrl: './add-team-dialog.component.html',
  styleUrls: ['./add-team-dialog.component.css']
})
export class AddTeamDialogComponent implements OnInit {

  private me: Me;
  form: FormGroup;

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<AddTeamDialogComponent>,
    private meService: MeService
  ) {}

  ngOnInit() {
    this.form = this.formBuilder.group({
      _id: '',
      name: ''
    })
    this.meService.getMe().subscribe(me => this.me = me);
  }

  submit(form) {
    this.dialogRef.close(form.value);
  }

  hasAdmin(): boolean {
    return this.me && this.me.admin;
  }

}
