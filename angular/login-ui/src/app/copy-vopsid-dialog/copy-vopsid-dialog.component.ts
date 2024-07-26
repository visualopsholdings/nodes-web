import { Component, OnInit, Inject } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';

@Component({
  selector: 'app-copy-vopsid-dialog',
  templateUrl: './copy-vopsid-dialog.component.html',
  styleUrls: ['./copy-vopsid-dialog.component.css']
})
export class CopyVopsidDialogComponent implements OnInit {

  form: FormGroup;
  showVID = false;

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<CopyVopsidDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: any,
  ) {}

  ngOnInit() {
    this.form = this.formBuilder.group({
    })
  }

  submit(form) {
    this.dialogRef.close(form.value);
  }

}
