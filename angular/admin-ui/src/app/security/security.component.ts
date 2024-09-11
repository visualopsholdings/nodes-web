import { Component, OnInit, Input } from '@angular/core';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { ActivatedRoute } from '@angular/router';

import * as _ from "lodash";

import { MeService }  from '../me.service';
import { Me }  from '../me';
import { Policy } from '../policy';
import { PolicyLine } from '../policy-line';
import { AddGroupPolicyDialogComponent } from '../add-group-policy-dialog/add-group-policy-dialog.component';
import { AddUserPolicyDialogComponent } from '../add-user-policy-dialog/add-user-policy-dialog.component';
import { AddGroupPolicyExecDialogComponent } from '../add-group-policy-exec-dialog/add-group-policy-exec-dialog.component';
import { AddUserPolicyExecDialogComponent } from '../add-user-policy-exec-dialog/add-user-policy-exec-dialog.component';
import { PolicyService }  from '../policy.service';
import { Info }  from '../info';
import { InfoService }  from '../info.service';
import { PolicyIdDialogComponent } from '../policy-id-dialog/policy-id-dialog.component';

@Component({
  selector: 'app-security',
  templateUrl: './security.component.html',
  styleUrls: ['./security.component.css']
})
export class SecurityComponent implements OnInit {

  @Input() obj = "";
  @Input() exec = true;
  @Input() policy = "";

  private me: Me;
  policies: PolicyLine[];
  policySave = false;
  message = "";
  isMirror = false;

  private id: string;

  constructor(
    public dialog: MatDialog,
    private route: ActivatedRoute,
    private policyService: PolicyService,
    private infoService: InfoService,
    private meService: MeService
  ) {
    this.id = route.snapshot.paramMap.get('id');
  }

  ngOnInit() {
    this.meService.getMe()
      .subscribe(me => {
        this.me = me;
        this.getPolicies();
     });
  }

  hasAdmin(): boolean {
    return this.me ? this.me.admin : false;
  }

  secObj(): string {
    return this.hasAdmin() ? ("raw" + this.obj) : this.obj;
  }

  showExec(): boolean {
    return this.obj == "streams" || this.obj == "sequences";
  }

  getPolicies(): void {
    this.policyService.getPolicies(this.secObj(), this.id).subscribe(policies => {
        this.policies = [];
        var hasExec = false;
        policies.forEach(e => {
          let line = _.find(this.policies, e2 => e._id == e2._id);
          if (!line) {
            line = { _id: e._id, name: e.name, context: e.context } as PolicyLine;
            this.policies.push(line);
          }
          if (e.type == "view") {
            line.viewPath = e.path;
          }
          else if (e.type == "edit") {
            line.editPath = e.path;
          }
          else {
            line.execPath = e.path;
            hasExec = true;
          }
        });
        if (hasExec && !this.exec) {
          // filter out those that have ONLY an exec path
          this.policies = this.policies.filter(e => !(e.execPath && !(e.viewPath || e.editPath)));
        }
      });
  }

  private policySaveDisabled(): void {
    this.policySave = false;
    if (this.policies) {
      this.policies.forEach(e => {
        if (e.editPath == '*' || e.viewPath == '*' || e.execPath == '*' || e.editRemoved || e.viewRemoved || e.execRemoved) {
          this.policySave = true;
          return;
        }
      });
    }
  }

  addUserPolicy(): void {
    let exec = this.showExec();
    let dialogRef = exec ? this.dialog.open(AddUserPolicyExecDialogComponent) : this.dialog.open(AddUserPolicyDialogComponent);
    dialogRef.afterClosed().subscribe(result => {
      if (result) {
        let line = { _id: result.user._id, name: result.user.fullname, context: "user" } as PolicyLine;
        if (result.role == "view") {
          line.viewPath = "*";
        }
        else if (result.role == "edit") {
          line.editPath = "*";
          if (!exec) {
            line.execPath = "*";
          }
        }
        else if (result.role == "exec") {
          line.execPath = "*";
        }
        else {
          line.viewPath = "*";
          line.editPath = "*";
          line.execPath = "*";
        }
        this.policies.push(line);
        this.policySaveDisabled();
      }
    });
  }

  addGroupPolicy(): void {
    let exec = this.showExec();
    let dialogRef = exec? this.dialog.open(AddGroupPolicyExecDialogComponent) : this.dialog.open(AddGroupPolicyDialogComponent);
    dialogRef.afterClosed().subscribe(result => {
      if (result) {
        let line = { _id: result.team._id, name: result.team.name, context: "group" } as PolicyLine;
        if (result.role == "view") {
          line.viewPath = "*";
        }
        else if (result.role == "edit") {
          line.editPath = "*";
          if (!exec) {
            // keep in sync
            line.execPath = "*";
          }
        }
        else if (result.role == "exec") {
          line.execPath = "*";
        }
        else {
          line.viewPath = "*";
          line.editPath = "*";
          line.execPath = "*";
        }
        this.policies.push(line);
        this.policySaveDisabled();
      }
    });
  }

  removeRole(type: string, line: PolicyLine): void {
    let exec = this.showExec();
    if (type == "view") {
      if (line.viewPath == '*') {
        line.viewPath = null;
      }
      else {
        line.viewRemoved = !line.viewRemoved;
      }
    }
    else if (type == "edit") {
      if (line.editPath == '*') {
        line.editPath = null;
      }
      else {
        line.editRemoved = !line.editRemoved;
      }
      if (!exec) {
        // keep in sync
        line.execPath = line.editPath;
        line.execRemoved = line.editRemoved;
      }
    }
    else {
      if (line.execPath == '*') {
        line.execPath = null;
      }
      else {
        line.execRemoved = !line.execRemoved;
      }
    }
    if (!line.viewPath && !line.editPath && !line.execPath) {
      let policies = [];
      this.policies = _.filter(this.policies, e => e != line);
    }
    this.policySaveDisabled();
  }

  savePolicy(): void {
    let changes = { add: [], remove: [] };
    this.policies.forEach(e => {
      if (e.editPath == '*') {
        changes.add.push({ _id: e._id, context: e.context, type: "edit" });
      }
      if (e.viewPath == '*') {
        changes.add.push({ _id: e._id, context: e.context, type: "view" });
      }
      if (e.execPath == '*') {
        changes.add.push({ _id: e._id, context: e.context, type: "exec" });
      }
      if (e.editRemoved) {
        changes.remove.push(e.editPath);
      }
      if (e.viewRemoved) {
        changes.remove.push(e.viewPath);
      }
      if (e.execRemoved) {
        changes.remove.push(e.execPath);
      }
    });
    this.policyService.updatePolicy(this.secObj(), this.id, changes).subscribe(() => {
        this.message = "Policy saved.";
        this.getPolicies();
      });
  }

  savePolicyWithId(): void {
    this.dialog.open(PolicyIdDialogComponent, { data: { title: "Add with policy id" } }).afterClosed().subscribe(result => {
      if (result) {
        let changes = { _id: result._id, add: [], remove: [] };
        this.policies.forEach(e => {
          if (e.editPath == '*') {
            changes.add.push({ _id: e._id, context: e.context, type: "edit" });
          }
          if (e.viewPath == '*') {
            changes.add.push({ _id: e._id, context: e.context, type: "view" });
          }
          if (e.execPath == '*') {
            changes.add.push({ _id: e._id, context: e.context, type: "exec" });
          }
          if (e.editRemoved) {
            changes.remove.push(e.editPath);
          }
          if (e.viewRemoved) {
            changes.remove.push(e.viewPath);
          }
          if (e.execRemoved) {
            changes.remove.push(e.execPath);
          }
        });
        this.policyService.updatePolicy(this.secObj(), this.id, changes)
          .subscribe(() => {
            this.message = "Policy saved.";
            this.getPolicies();
          });
        }
      });
  }

  usePolicyId(): void {
    this.dialog.open(PolicyIdDialogComponent, { data: { title: "Use policy id directly" } }).afterClosed().subscribe(result => {
      if (result) {
        this.policyService.updatePolicy(this.secObj(), this.id, { _id: result._id, use: true })
          .subscribe(() => {
            this.message = "Policy saved.";
            this.getPolicies();
          });
        }
      });
  }

}
