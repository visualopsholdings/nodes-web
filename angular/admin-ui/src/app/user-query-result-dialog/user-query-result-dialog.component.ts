import { Component, OnInit, Inject, ViewChild } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';
import { MatSelectionList } from '@angular/material/list';

@Component({
  selector: 'app-user-query-result-dialog',
  templateUrl: './user-query-result-dialog.component.html',
  styleUrls: ['./user-query-result-dialog.component.css']
})
export class UserQueryResultDialogComponent implements OnInit {

  @ViewChild('list') list: MatSelectionList;

  form: FormGroup;
  users: any[];

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<UserQueryResultDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: any
  ) {
    this.users = data.users;
  }

  ngOnInit() {
    this.form = this.formBuilder.group({})
  }

  submit(form) {
    form.value = this.list.selectedOptions.selected.map(e => e.value);
    this.dialogRef.close(form.value);
  }

}

