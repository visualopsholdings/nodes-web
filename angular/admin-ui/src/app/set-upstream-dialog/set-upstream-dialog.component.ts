import { Component, OnInit, Inject } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';

@Component({
  selector: 'app-set-upstream-dialog',
  templateUrl: './set-upstream-dialog.component.html',
  styleUrls: ['./set-upstream-dialog.component.css']
})
export class SetUpstreamDialogComponent implements OnInit {

  form: FormGroup;
  mirror = false;

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<SetUpstreamDialogComponent>,
    @Inject(MAT_DIALOG_DATA) private data: any
  ) {}

  ngOnInit() {
    this.form = this.formBuilder.group(this.data)
  }

  submit(form) {
    this.dialogRef.close(form.value);
  }

  hasMirror(): boolean {
    return this.form.value.upstreamMirror;
  }

}
