import { Component, OnInit, Inject, AfterViewInit, ViewChild, ElementRef } from '@angular/core';
import { Location } from '@angular/common';
import { FormGroup, FormBuilder, FormControl } from '@angular/forms';
import { ActivatedRoute, Router } from '@angular/router';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { Clipboard } from '@angular/cdk/clipboard';

import { LoginService } from '../login.service';
import { Login } from '../login';
import { WINDOW } from "../window.service";
import { UserService } from '../user.service';
import { User } from '../user';
import { CopyVopsidDialogComponent } from '../copy-vopsid-dialog/copy-vopsid-dialog.component';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit, AfterViewInit {

  @ViewChild("vidBrowserUpload") vidBrowserUpload: ElementRef;

  form: FormGroup;
  usernameCtrl = new FormControl("", []);
  passwordCtrl = new FormControl("", []);
  fullnameCtrl = new FormControl("", []);
  token: string;
  msg = "";
  errmsg = "";
  canRegister = false;

  constructor(
    private dialog: MatDialog,
    private route: ActivatedRoute,
    private router: Router,
    private formBuilder: FormBuilder,
    private clipboard: Clipboard,
    private loginService: LoginService,
    private userService: UserService,
    @Inject(WINDOW) private window: Window
  ) {
  }

  ngOnInit() {

    this.form = this.formBuilder.group({})
    this.usernameCtrl.disable();
    this.usernameCtrl.setValue("VID");

  }

  ngAfterViewInit(): void {
  }

  hasOpen(): boolean {
    return (window as any).showOpenFilePicker != null;
  }

	private _charsToHex(password: string) {

		var hexp = "";
		for (var i=0; i<password.length; i++) {
			hexp += password.charCodeAt(i).toString(16);
		}
		return hexp;

	}

	private isVopsID(id: string): boolean {
		if (!id) {
			return false;
		}
		try {
      var hex = this._charsToHex(atob(id));
      if (hex.length <= 31) {
        return false;
      }
      return hex.substr(0, 4) == "564f";
		}
		catch {
		  return false;
		}
	}

  existingUser() {
    this.canRegister = false;
  }

  private login(password: string) {

    this.loginService.login({ name: "", password: password } as Login).subscribe(result => {
      if (result) {
        if (result.redirect) {
          this.window.history.back();
        }
        else {
          var url = this.window.location.protocol + "//" + this.window.location.hostname;
          if (this.window.location.port) {
            url += ":" + this.window.location.port;
          }
          this.window.location.href = "/apps/chat";
        }
      }
    });
  }

  doLogin() {
    this.errmsg = "";
    if (this.passwordCtrl.value) {
      this.login(this.passwordCtrl.value);
      return;
    }
    if (this.canRegister && this.fullnameCtrl.value) {
      this.msg = "Please wait while we generate your Vops ID...";
      this.userService.newUser({ fullname: this.fullnameCtrl.value, vopsidtoken: this.token } as User).subscribe(result => {
        if (result) {
          var vopsid = result.vopsid;
          this.passwordCtrl.setValue(vopsid);
          this.msg = "";
          this.clipboard.copy(vopsid);
          this.dialog.open(CopyVopsidDialogComponent, { data: { vopsid: vopsid, fullname: result.fullname }}).afterClosed().subscribe(result => {
            this.login(vopsid);
          });
        }
      });
      return;
    }

  }

  uploadVID(): void {
    const vidBrowserUpload = this.vidBrowserUpload.nativeElement;
    vidBrowserUpload.onchange = () => {
      var reader = new FileReader();
      reader.onload = (e => {
        this.passwordCtrl.setValue(<string>e.target.result);
      });
      reader.readAsText(vidBrowserUpload.files[0]);
    };
    vidBrowserUpload.click();
  }

}
