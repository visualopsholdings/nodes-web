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
  app: string;
  stream: string;
  idea: string;
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
    route.queryParams.subscribe(params => {
      this.app = params['app'];
      this.stream = params['stream'];
      this.idea = params['idea'];
      this.token = params['token'];
      console.log("got token on url", this.token);
    });
  }

  ngOnInit() {

    this.form = this.formBuilder.group({})
    this.usernameCtrl.disable();
    this.usernameCtrl.setValue("VID");

    if (this.token) {
      console.log("Has token", this.token);
      this.userService.canRegister(this.token).subscribe(result => {
        if (result.newVopsID) {
          this.userService.newUser({ vopsidtoken: this.token } as User).subscribe(result => {
            if (result) {
              var vopsid = result.vopsid;
              this.clipboard.copy(vopsid);
              this.dialog.open(CopyVopsidDialogComponent, { data: { vopsid: vopsid, fullname: result.fullname }}).afterClosed().subscribe(result => {
                this.passwordCtrl.setValue(vopsid);
                this.login(vopsid);
              });
            }
          });
        }
        else {
          this.canRegister = result.canRegister;
        }
      });
    }
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

    this.loginService.login({ name: "", password: password, insecure: !this.isVopsID(password) } as Login).subscribe(result => {
      if (result) {
        if (result.redirect) {
          this.window.history.back();
        }
        else {
          var url = this.window.location.protocol + "//" + this.window.location.hostname;
          if (this.window.location.port) {
            url += ":" + this.window.location.port;
          }
          url += "/apps/";
          if (this.app) {
            url += this.app;
            if (this.idea) {
              url += "/#/ideas/" + this.idea;
            }
            else if (this.stream) {
              url += "/#/streams/" + this.stream;
            }
          }
          else {
            url += "chat";
          }
          this.window.location.href = url;
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
