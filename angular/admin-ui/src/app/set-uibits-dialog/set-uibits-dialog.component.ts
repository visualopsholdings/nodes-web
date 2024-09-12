import { Component, OnInit, Inject } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';

@Component({
  selector: 'app-set-uibits-dialog',
  templateUrl: './set-uibits-dialog.component.html',
  styleUrls: ['./set-uibits-dialog.component.css']
})
export class SetUibitsDialogComponent implements OnInit {

  form: FormGroup;

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<SetUibitsDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: any
  ) {}

  ngOnInit() {
    this.form = this.formBuilder.group({
      uibits: this.data.uibits
    })
  }

  submit(form) {
    this.dialogRef.close(form.value);
  }

}
