import { Component, OnInit } from '@angular/core';
import { MatDialogRef } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';

@Component({
  selector: 'app-add-upstream-team-dialog',
  templateUrl: './add-upstream-team-dialog.component.html',
  styleUrls: ['./add-upstream-team-dialog.component.css']
})
export class AddUpstreamTeamDialogComponent implements OnInit {

  form: FormGroup;

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<AddUpstreamTeamDialogComponent>
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
