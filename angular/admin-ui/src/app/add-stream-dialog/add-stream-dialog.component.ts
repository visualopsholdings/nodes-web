import { Component, OnInit } from '@angular/core';
import { MatDialogRef } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';

import { MeService }  from '../me.service';
import { Me }  from '../me';

@Component({
  selector: 'app-add-stream-dialog',
  templateUrl: './add-stream-dialog.component.html',
  styleUrls: ['./add-stream-dialog.component.css']
})
export class AddStreamDialogComponent implements OnInit {

  private me: Me;
  form: FormGroup;

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<AddStreamDialogComponent>,
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
