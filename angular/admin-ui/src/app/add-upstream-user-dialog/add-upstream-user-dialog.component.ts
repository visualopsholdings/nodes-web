import { Component, OnInit } from '@angular/core';
import { MatDialogRef } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';

@Component({
  selector: 'app-add-upstream-user-dialog',
  templateUrl: './add-upstream-user-dialog.component.html',
  styleUrls: ['./add-upstream-user-dialog.component.css']
})
export class AddUpstreamUserDialogComponent implements OnInit {

  form: FormGroup;

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<AddUpstreamUserDialogComponent>
  ) {}

  ngOnInit() {
    this.form = this.formBuilder.group({
      _id: '',
      email: '',
    })
  }

  submit(form) {
    this.dialogRef.close(form.value);
  }

}
