import { Component, OnInit, Inject, ViewChild } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';
import { MatSelectionList } from '@angular/material/list';

@Component({
  selector: 'app-team-query-result-dialog',
  templateUrl: './team-query-result-dialog.component.html',
  styleUrls: ['./team-query-result-dialog.component.css']
})
export class TeamQueryResultDialogComponent implements OnInit {

  @ViewChild('list') list: MatSelectionList;

  form: FormGroup;
  teams: any[];

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<TeamQueryResultDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: any
  ) {
    this.teams = data.teams;
  }

  ngOnInit() {
    this.form = this.formBuilder.group({})
  }

  submit(form) {
    form.value = this.list.selectedOptions.selected.map(e => e.value);
    this.dialogRef.close(form.value);
  }

}
