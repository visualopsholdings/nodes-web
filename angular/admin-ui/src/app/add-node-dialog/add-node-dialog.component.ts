import { Component, OnInit } from '@angular/core';
import { MatDialogRef } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';

@Component({
  selector: 'app-add-node-dialog',
  templateUrl: './add-node-dialog.component.html',
  styleUrls: ['./add-node-dialog.component.css']
})
export class AddNodeDialogComponent implements OnInit {

  form: FormGroup;

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<AddNodeDialogComponent>
  ) {}

  ngOnInit() {
    this.form = this.formBuilder.group({
      serverId: '',
      pubKey: ''
    })
  }

  submit(form) {
    this.dialogRef.close(form.value);
  }

}
