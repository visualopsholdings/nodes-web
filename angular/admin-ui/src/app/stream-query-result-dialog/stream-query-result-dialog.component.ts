import { Component, OnInit, Inject, ViewChild } from '@angular/core';
import { MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';
import { MatSelectionList } from '@angular/material/list';

@Component({
  selector: 'app-stream-query-result-dialog',
  templateUrl: './stream-query-result-dialog.component.html',
  styleUrls: ['./stream-query-result-dialog.component.css']
})
export class StreamQueryResultDialogComponent implements OnInit {

  @ViewChild('list') list: MatSelectionList;

  form: FormGroup;
  streams: any[];

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<StreamQueryResultDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: any
  ) {
    this.streams = data.streams;
  }

  ngOnInit() {
    this.form = this.formBuilder.group({})
  }

  submit(form) {
    form.value = this.list.selectedOptions.selected.map(e => e.value);
    this.dialogRef.close(form.value);
  }

}
