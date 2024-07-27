import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { Observable, Subject } from 'rxjs';
import { catchError, map } from 'rxjs/operators';
import { HttpEventType, HttpErrorResponse } from '@angular/common/http';
import { of } from 'rxjs';
import { BreakpointObserver, Breakpoints } from '@angular/cdk/layout';

import * as _ from 'lodash';
import * as moment from 'moment';

import { IdeaService }  from '../idea.service';
import { ConversationService }  from '../conversation.service';
import { Me }  from '../me';
import { Idea }  from '../idea';
import { IconService }  from '../icon.service';
import { StreamService }  from '../stream.service';
import { Stream }  from '../stream';
import { UserService }  from '../user.service';
import { User }  from '../user';
import { Item }  from '../item';

import { EditIdeaDialogComponent } from '../edit-idea-dialog/edit-idea-dialog.component';
import { ConfirmComponent } from '../confirm/confirm.component';
import { SocketService }  from '../socket.service';
import { ChooseStreamDialogComponent } from '../choose-stream-dialog/choose-stream-dialog.component';
import { ConversationComponent } from '../conversation/conversation.component';
import { StreamOpenService }  from '../stream-open.service';

@Component({
  selector: 'app-idea',
  templateUrl: './idea.component.html',
  styleUrls: ['./idea.component.css']
})
export class IdeaComponent implements OnInit {

  @Input('me') me: Me;
  @Input('parent') parent: ConversationComponent;
  @Input('showSep') showSep: boolean;
  @Input('item') item: Item;
  @Input('width') width: string;
  @Input('stream') stream: Stream;
  @Input('noPadTop') noPadTop: boolean;

  private _idea = new Idea();

  icon: string;
  createdByName = "xxxx";
  error = "";
  status = "";
  userName = "";

  constructor(
    public dialog: MatDialog,
    private ideaService: IdeaService,
    private conversationService: ConversationService,
    private streamService: StreamService,
    private iconService: IconService,
    private userService: UserService,
    private socketService: SocketService,
    private streamOpenService: StreamOpenService,
    private route: ActivatedRoute,
    private router: Router,
    private breakpointObserver: BreakpointObserver
  ) {
  }

  @Output() changed = new EventEmitter<any>();
  @Input() set idea(value: Idea) {

//    console.log("idea set", value._id, this._idea._id, value.modifyDate, this._idea.modifyDate);

    if (value) {

      if (this._idea) {
        if (this._idea._id == value._id && new Date(this._idea.modifyDate).getTime() == new Date(value.modifyDate).getTime()) {
//          console.log("same idea, skipping");
          return;
        }
      }

      if (value.user) {
        if (this.parent) {
          this.parent.getUserName(value.user).subscribe(name => {
            this.userName = name[0];
          });
        }
        else {
          this.userName = "Unknown";
        }
      }

      var oldid = this._idea._id;
      this._idea = value;

      if (!oldid || this._idea._id != oldid) {
        this.finishLoad();
      }
    }
    else {
      this.itemLoaded();
    }
  }

  private itemLoaded() {
    if (this.item) {
      this.item.loaded = true;
    }
  }

  private finishLoad() {

//    console.log("finishLoad");

    if (!this._idea.modifyDate) {
      this._idea.modifyDate = new Date();
    }

  }

  get idea() : Idea {
    return this._idea;
  }

  ngOnInit() {
    this.finishLoad();
  }

  isMe(idea: Idea = null): boolean {
    if (!idea) {
      idea = this._idea;
    }
    return this.me && idea ? idea.user == this.me._id : false;
  }

  hasAdmin(): boolean {
    return this.me && this.me.admin;
  }

  showData(idea: Idea, table: string): void {
    this.router.navigateByUrl("/data/ideas/" + idea._id + "/" + table);
  }

  focusIdea(idea: Idea): void {
    this.router.navigateByUrl("/ideas/" + idea._id);
  }

  text(): string {
    var text =  this._idea.text ?  this._idea.text.replace('\n', "<br>") : "&nbsp;";
    return this.parent ? this.parent.urlify(text) : text;
  }

  isMessage(): boolean {
    return true;
  }

  onChange(newValue) {
//    console.log("onChange", newValue);
  }

  dateDisplay(): string {
    return "block";
  }

  dateDividerColor(): string {
    return this.parent ? this.parent.dateDividerColor() : "black";
  }

  dateColor(): string {
    return this.parent ? this.parent.dateColor() : "black";
  }

  bgColor(me: boolean): string {
    return this.parent ? this.parent.itemColor(me) : "white";
  }

  itemBGColor(me: boolean): string {
    if (this.stream) {
      return "white";
    }
    return this.parent ? this.parent.itemColor(me) : "white";
  }

  itemTextColor(): string {
    return "black";
  }

  textColor(): string {
    return "black";
  }

  getIdea(id: string): Observable<Idea> {
    return this.ideaService.getIdea(id);
  }
  getStream(id: string): Observable<Stream> {
    return this.streamService.getStream(id);
  }

  getIcon(): string {
    if (!(this.icon.indexOf("https://") == 0 || this.icon.indexOf("http://") == 0)) {
      return "/rest/1.0/media/" + this.icon + "/thumbnail";
    }
    return this.icon;
  }

  private isTiny(): boolean {
//    return this.breakpointObserver.isMatched("(max-width: 413px)"); // 414 for Iphoen 11
    return this.breakpointObserver.isMatched("(max-width: 359px)"); // 360 for galaxy 8
  }

  onDragStart(event: any) {
//    console.log(event);
  }

  padTop(): boolean {
    return !this.noPadTop;
  }

  fullWidth(): string {
    return window.innerWidth + "px";
  }

  public breakAt(size: number): boolean {
    return this.parent.breakAt(size);
  }

}
