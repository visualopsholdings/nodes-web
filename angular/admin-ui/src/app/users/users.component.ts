import { Component, OnInit, Inject } from '@angular/core';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { PageEvent } from '@angular/material/paginator';
import { Router } from '@angular/router';
import { FormGroup, FormBuilder, FormControl, Validators } from '@angular/forms';

import { User } from '../user';
import { UserService } from '../user.service';
import { IconService } from '../icon.service';

@Component({
  selector: 'app-users',
  templateUrl: './users.component.html',
  styleUrls: ['./users.component.css']
})
export class UsersComponent implements OnInit {

  items: any[];
  pageSizeOptions = [9, 36, 72];
  pageSize = 9;
  total: number = 0;
  displayedColumns: string[] = [ "icon", "id", "name", "actions" ];

  constructor(
    public dialog: MatDialog,
    public router: Router,
    private formBuilder: FormBuilder,
    private userService: UserService,
    private iconService: IconService,
    )
  {}

  ngOnInit() {
    this.getItems(0);
  }

  getItems(from: number): void {
    this.userService.getUsers(from, this.pageSize)
      .subscribe(resp => {
        this.total = this.userService.getTotal(resp);
        this.items = resp.body;
      });
  }

  goPage(event: PageEvent): void {
    this.pageSize = event.pageSize;
    this.getItems(event.pageIndex * event.pageSize);
  }

  getIcon(item: any): string {
    return this.iconService.getIcon({ icon: "internal:user" });
  }

}
