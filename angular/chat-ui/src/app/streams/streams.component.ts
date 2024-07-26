import { Component, OnInit, OnDestroy, Input, SimpleChange, ViewChild, ElementRef, EventEmitter } from '@angular/core';
import { Router, ActivatedRoute, NavigationEnd } from '@angular/router';
import { MatDialog } from '@angular/material/dialog';
import { interval, Observable } from 'rxjs';
import { debounce } from 'rxjs/operators';
import { BreakpointObserver, Breakpoints } from '@angular/cdk/layout';
import { HostListener } from '@angular/core';
import { FormControl } from '@angular/forms';

import * as _ from 'lodash';

import { Me }  from '../me';
import { Stream }  from '../stream';
import { User }  from '../user';

import { ConfirmComponent } from '../confirm/confirm.component';
import { AppComponent } from '../app.component';
import { IconService }  from '../icon.service';
import { StreamService }  from '../stream.service';
import { SocketService }  from '../socket.service';
import { StreamOpenService }  from '../stream-open.service';
import { BootstrapService } from '../bootstrap.service';
import { StreamFlags } from '../../../../shared-ui/streamflags';

@Component({
  selector: 'app-streams',
  templateUrl: './streams.component.html',
  styleUrls: ['./streams.component.css']
})
export class StreamsComponent implements OnInit, OnDestroy {

  @Input('main') main: AppComponent;

  me: Me;
  streams: Stream[];
  listheight = "160px";
  inFront = true;
  opened = false;
  selectedStream: Stream;

  private maxStreams = 1000;

  @HostListener('window:focus', ['$event'])
  onFocus(event: FocusEvent): void {
    this.nowInFront();
  }

  @HostListener('window:blur', ['$event'])
  onBlur(event: FocusEvent): void {
     this.nowBehind();
  }

  @HostListener('document:visibilitychange', ['$event'])
  onVisibilityChange(event: any): void {
    if (document.visibilityState == "hidden") {
      this.nowBehind();
    }
    else if (document.visibilityState == "visible") {
      this.nowInFront();
    }
  }

  private nowBehind() {
    this.inFront = false;
  }

  private nowInFront() {
    if (!this.inFront) {
      this.inFront = true;
      this.getStreams();
    }
  }

  constructor(
    public dialog: MatDialog,
    private router: Router,
    private route: ActivatedRoute,
    private breakpointObserver: BreakpointObserver,
    private iconService: IconService,
    private streamService: StreamService,
    private socketService: SocketService,
    private bootstrapService: BootstrapService,
    private streamOpenService: StreamOpenService,
  ) {
    router.events.subscribe(change => {
      if (change instanceof NavigationEnd) {
        this.onResize(null);
      }
    });
  }

  ngOnInit() {
    this.setTitle();
    this.streamOpenService.openChanged$.subscribe(change => {
      if (change.refresh) {
        this.getStreams();
        if (change.clear) {
          this.selectedStream = null;
        }
        this.onResize(null);
      }
      else {
        this.opened = change.open && this.shouldShowAtAll();
        if (this.opened) {
          var stream = change.streams && change.streams.length > 0 ? change.streams[0] : null;
          if (stream) {
            this.showStream(stream);
            this.onResize(null);
          }
          else {
            this.selectedStream = null;
            this.getStreams();
          }
        }
      }
    });
    this.bootstrapService.meChanged$.subscribe(me => {
      [this.me] = me;
      this.getStreams();
      this.onResize(null);
    });
  }

  ngOnDestroy() {
  }

  private showStream(id: string): void {
    if (id) {
      this.streamService.getStream(id).subscribe(stream => {
        this.selectedStream = stream;
        this.main.convName = stream.name;
        this.onResize(null);
      });
    }
  }

  isSelected(stream: Stream): boolean {
    return this.selectedStream && stream._id == this.selectedStream._id;
  }

  private shouldShowAtAll(): boolean {
    return this.me != null;
  }

  private shouldShowAtStart(): boolean {
    return this.me != null;
  }

  selectStream(stream: Stream) {
    this.selectedStream = stream;
    this.setTitle();
    this.resetStreams();
    this.router.navigateByUrl(`/streams/${stream._id}`);
  }

  private resetStreams() {
    var oldstreams = [];
    this.streams.forEach(s => oldstreams.push(s));
    this.streams = oldstreams;
  }

  tabTitle(): string {
    return "Chat";
  }

  headerTitle(): string {
    return "ZMQ Chat";
  }

  private setTitle(): void {
    document.title = this.headerTitle();
  }

  private getMinStreamsForMarket(): number {
    return this.maxStreams;
  }

  private getStreams(): void {

    if (!this.me) {
      return;
    }

    this.streamService.getStreams(this.maxStreams).subscribe(streams => {
      this.loadStreams(streams);
    });

  }

  private loadStreams(streams: Stream[]) {
    this.setTitle();
    this.streams = streams;
  }

  getIcon(item: any): string {
    return this.iconService.getIcon({ icon: "internal:stream" });
  }

  onDrag(event: any) {
    console.log(event);
  }

  onResize(event: any) {

    this.opened = this.shouldShowAtAll();

    let fullheight = window.innerHeight-68;
    if (this.selectedStream) {
      this.listheight = (fullheight - 24) + "px";
    }
    else {
      this.listheight = (fullheight - 42) + "px";
    }
  }

  close() {
    this.opened = false;
  }

  canClose(): boolean {
    return this.selectedStream != null;
  }

}
