import { Component, OnInit, OnDestroy, Input, SimpleChange, ViewChild, ElementRef, EventEmitter } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { MatDialog } from '@angular/material/dialog';
import { interval, Observable } from 'rxjs';
import { debounce } from 'rxjs/operators';
import { BreakpointObserver, Breakpoints } from '@angular/cdk/layout';
import { HostListener } from '@angular/core';
import { FormControl } from '@angular/forms';

import { ConversationComponent } from '../conversation/conversation.component';

import { Stream }  from '../stream';
import { Me }  from '../me';
import { User }  from '../user';
import { Team }  from '../team';

import { UserListService } from '../user-list.service';
import { BootstrapService } from '../bootstrap.service';
import { SocketService }  from '../socket.service';
import { StreamService }  from '../stream.service';
import { IconService }  from '../icon.service';
import { StreamOpenService }  from '../stream-open.service';
import { StreamFlags } from '../../../../shared-ui/streamflags';
import { Site }  from '../site';

@Component({
  selector: 'app-people',
  templateUrl: './people.component.html',
  styleUrls: ['./people.component.css']
})
export class PeopleComponent implements OnInit {

  @Input('parent') parent: ConversationComponent;
  @Input('stream') stream: Stream;

  site: Site;
  me: Me;
  listheight = "160px";

  users: User[];

  constructor(
    public dialog: MatDialog,
    private router: Router,
    private route: ActivatedRoute,
    private breakpointObserver: BreakpointObserver,
    private bootstrapService: BootstrapService,
    private userListService: UserListService,
    private streamService: StreamService,
    private iconService: IconService,
    private streamOpenService: StreamOpenService,
  ) {
  }

  ngOnInit(): void {
    this.userListService.usersChanged$.subscribe(result => {
      this.users = result[0];
    });
    this.me = this.bootstrapService.getMe();
    this.users = this.userListService.allUsers();
    // it's possible that people is created before bootstrap has occured.
    this.bootstrapService.meChanged$.subscribe(me => {
      [this.me, this.site] = me;
    });
    this.onResize(null);
  }

  onResize(event: any) {
    let fullheight = window.innerHeight-68;
    this.listheight = (fullheight-40) + "px";
  }

  private isSmall(): boolean {
    return this.breakpointObserver.isMatched("(max-width: 960px)");
  }

  peopleOpen(): boolean {
    return this.parent.peopleOpen();
  }

  close() {
    this.parent.hidePeople();
  }

  getIcon(item: User): string {
    return this.iconService.getIcon({ icon: "internal:user" });
  }

  initials(name: string): string {
    return name ? name.split(" ").map(w => w.substring(0, 1).toUpperCase()).join("") : "";
  }

  private hasAdmin(): boolean {
    return this.me && this.me.admin;
  }

}
