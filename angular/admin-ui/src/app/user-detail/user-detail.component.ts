import { Component, OnInit, Input } from '@angular/core';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { ActivatedRoute } from '@angular/router';
import { Location } from '@angular/common';
import { MatSnackBar } from '@angular/material/snack-bar';

import { User } from '../user';
import { UserService }  from '../user.service';
import { ConfirmComponent } from '../confirm/confirm.component';
import { IconService }  from '../icon.service';
import { MeService }  from '../me.service';
import { Me }  from '../me';

@Component({
  selector: 'app-user-detail',
  templateUrl: './user-detail.component.html',
  styleUrls: ['./user-detail.component.css']
})
export class UserDetailComponent implements OnInit {

  @Input() user: User;

  private me: Me;

  constructor(
    public dialog: MatDialog,
    private route: ActivatedRoute,
    private location: Location,
    private snackBar: MatSnackBar,
    private userService: UserService,
    private iconService: IconService,
    private meService: MeService,
  ) {}

  ngOnInit() {
    this.meService.getMe()
      .subscribe(me => {
        this.me = me;
        this.getUser();
     });
  }

  hasAdmin(): boolean {
    return this.me ? this.me.admin : false;
  }

  getUser(): void {
    const id = this.route.snapshot.paramMap.get('id');
    this.userService.getUser(this.hasAdmin(), id).subscribe(user => {
      this.user = user;
    });
  }

  adminChange(): void {
    let msg = this.user.admin ? "Click ok if you want to set the user to admin." : "Click ok if you want to set the user to non-admin.";
    let dialogRef = this.dialog.open(ConfirmComponent, { data: { title: "Are you sure?", description: msg } });
    dialogRef.afterClosed().subscribe(success => {
        if (success) {
          this.userService.updateUser({ _id: this.user._id, admin: this.user.admin } as User)
            .subscribe(user => {});
        }
        else {
          this.user.admin = !this.user.admin;
        }
      });
  }

  activeChange(): void {
    let msg = this.user.active ? "Click ok if you want to set the user to active." : "Click ok if you want to set the user to inactive.";
    let dialogRef = this.dialog.open(ConfirmComponent, { data: { title: "Are you sure?", description: msg } });
    dialogRef.afterClosed().subscribe(success => {
        if (success) {
          this.userService.updateUser({ _id: this.user._id, active: this.user.active } as User)
            .subscribe(user => {});
        }
        else {
          this.user.active = !this.user.active;
        }
      });
  }

  getIcon(item: any): string {
    return this.iconService.getIcon({ icon: "internal:user" });
  }

  save(): void {
    this.userService.updateUser({ _id: this.user._id, fullname: this.user.fullname, name: this.user.name } as User)
      .subscribe(() => {});
  }

}

