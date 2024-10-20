import { Component, OnInit } from '@angular/core';
import { MatDialogRef } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';

@Component({
  selector: 'app-add-upstream-stream-dialog',
  templateUrl: './add-upstream-stream-dialog.component.html',
  styleUrls: ['./add-upstream-stream-dialog.component.css']
})
export class AddUpstreamStreamDialogComponent implements OnInit {

  form: FormGroup;

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<AddUpstreamStreamDialogComponent>
  ) {}

  ngOnInit() {
    this.form = this.formBuilder.group({
      _id: '',
      name: '',
    })
  }

  submit(form) {
    this.dialogRef.close(form.value);
  }

}
