import { Component, OnInit, Inject } from '@angular/core';
import { MatDialog, MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';
import { FormGroup, FormBuilder } from '@angular/forms';
import { Clipboard } from '@angular/cdk/clipboard';

import { StreamService }  from '../stream.service';
import { Team }  from '../team';

@Component({
  selector: 'app-share-stream-dialog',
  templateUrl: './share-stream-dialog.component.html',
  styleUrls: ['./share-stream-dialog.component.css']
})
export class ShareStreamDialogComponent implements OnInit {

  form: FormGroup;
  link = "";
  teams: Team[];
  expires = "4";
  showLink = false;

  private _team: string;

  constructor(
    private formBuilder: FormBuilder,
    private dialogRef: MatDialogRef<ShareStreamDialogComponent>,
    @Inject(MAT_DIALOG_DATA) public data: any,
    private clipboard: Clipboard,
    private streamService: StreamService,
  ) {
  }

  get team(): string {
    return this._team;
  }

  set team(val: string) {
    this._team = val;
    this.generateLink();
  }

  ngOnInit() {
    this.form = this.formBuilder.group({});
    if (this.data.streamflags.shareWithNewUsers) {
      this.getTeams();
    }
    else {
      this.generateLink();
    }
  }

  getTeams(): void {
    this.streamService.getStreamTeams(this.data.stream).subscribe(teams => {
      teams = teams.filter(e => e.name != "Admin" && e.name != "Public");
      this._team = teams.length > 0 ? teams[0]._id : undefined;
      this.teams = teams;
      this.generateLink();
    });
  }

  generateLink(): void {
    this.streamService.getShareLink(this.data.stream,
          this._team ? this._team : "none",
          this.expires ? this.expires : "4").subscribe(link => {
      this.link = link.url;
      this.clipboard.copy(link.url);
    });
  }

  gotoLink(): void {
     window.open(this.link, "_blank");
  }

}
