import { Component, OnInit, OnDestroy, ViewChild, ElementRef, EventEmitter, Input, NgZone } from '@angular/core';
import { Router, ActivatedRoute } from '@angular/router';
import { MatDialog } from '@angular/material/dialog';
import { CdkScrollable } from '@angular/cdk/scrolling';
import { BreakpointObserver, Breakpoints } from '@angular/cdk/layout';
import { CdkTextareaAutosize } from '@angular/cdk/text-field';
import { take, map, debounceTime, throttleTime, tap } from 'rxjs/operators';
import { pipe, zip, Observable, of, Subject, Subscription } from 'rxjs';
import { MatPaginator } from '@angular/material/paginator';
import { HostListener } from '@angular/core';
import { Clipboard } from '@angular/cdk/clipboard';

import * as _ from 'lodash';
import * as moment from 'moment';

import { StreamService }  from '../stream.service';
import { Stream }  from '../stream';
import { User }  from '../user';
import { Item }  from '../item';
import { ConversationService }  from '../conversation.service';
import { IdeaService }  from '../idea.service';
import { NewIdeaService }  from '../new-idea.service';
import { Idea }  from '../idea';
import { MeService }  from '../me.service';
import { Me }  from '../me';
import { SocketService }  from '../socket.service';
import { IconService }  from '../icon.service';
import { StreamOpenService }  from '../stream-open.service';
import { ConfirmComponent } from '../confirm/confirm.component';
import { BootstrapService } from '../bootstrap.service';
import { StreamFlags } from '../../../../shared-ui/streamflags';
import { UpService }  from '../up.service';
import { UserListService } from '../user-list.service';
import { Site }  from '../site';
import { ShareStreamDialogComponent } from '../share-stream-dialog/share-stream-dialog.component';

@Component({
  selector: 'app-conversation',
  templateUrl: './conversation.component.html',
  styleUrls: ['./conversation.component.css']
})
export class ConversationComponent implements OnInit, OnDestroy {

  @ViewChild(CdkScrollable) scrollable: CdkScrollable;
  @ViewChild('autosize') autosize: CdkTextareaAutosize;
  @ViewChild('paginator') paginator: MatPaginator;
  @ViewChild('listContent') listContent;

  private urlRegex = /((((ftp|https|http?):\/\/)|(w{3}\.))[\-\w@:%_\+.~#?,&\/\/=]+)/gi;

  site: Site;
  me: Me;
  stream: Stream;
  streamflags: StreamFlags;
  items: Item[];
  focusIdea: boolean;
  private focusIdeaStream: string;

  private mutationObserver: MutationObserver;
  private onUpdate = new EventEmitter<any>();
  private lastInFront = null;

  length = 0;
  start = 0;
  pageSize = 30;
  pageSizeOptions: number[] = [10, 30, 60];
  innerHeight = 0;
  height = "700px";
  inFront = true;
  allLoaded = false;
  loading = false;
  message = "";

  private onKeySubject: Subject<boolean> = new Subject();
  private onClickSubject: Subject<boolean> = new Subject();
  private subscriptions: Subscription[] = [];
  private token: string;
  private ideaToLoad: string;
  private streamToLoad: string;
  private peopleOpened = false;

  constructor(
    public dialog: MatDialog,
    private router: Router,
    private route: ActivatedRoute,
    private ngZone: NgZone,
    private breakpointObserver: BreakpointObserver,
    private streamService: StreamService,
    private conversationService: ConversationService,
    private meService: MeService,
    private socketService: SocketService,
    private iconService: IconService,
    private ideaService: IdeaService,
    private newIdeaService: NewIdeaService,
    private streamOpenService: StreamOpenService,
    private bootstrapService: BootstrapService,
    private clipboard: Clipboard,
    private upService: UpService,
    private userListService: UserListService,
  ) {

    // it's possible that convo is created before bootstrap has occured.
    this.me = this.bootstrapService.getMe();
    this.site = this.bootstrapService.getSite();
    this.subscriptions.push(this.bootstrapService.meChanged$.subscribe(me => {
      [this.me, this.site] = me;
      if (this.ideaToLoad) {
          this.getSingleIdea(this.ideaToLoad, true);
          this.ideaToLoad = null;
      }
      else if (this.streamToLoad) {
        this.streamOpenService.select(this.streamToLoad);
        this.getStream(this.streamToLoad, true);
        this.streamToLoad = null;
      }
      else if (this.stream) {
        this.getStream(this.stream._id, true);
      }
    }));

    this.subscriptions.push(route.queryParams.subscribe(params => {
      const originalURL = params['originalURL'];
      if (originalURL) {
        window.history.replaceState({}, originalURL, originalURL);
      }
      this.token = params['token'];
      if (this.stream) {
        this.getStream(this.stream._id, true);
      }
    }));
    this.subscriptions.push(route.params.subscribe(params => {
      console.log("route changed", params);
      this.onClick();
//      console.log("load from params");
      if (params["idea"]) {
        if (this.me) {
          this.getSingleIdea(params["idea"], true);
        }
        else {
          this.ideaToLoad = params["idea"];
        }
      }
      else if (params["id"]) {
        if (this.me) {
          // only when we don't already have a stream we do this.
          if (!this.stream) {
            this.streamOpenService.select(params["id"]);
          }
          else {
            this.streamOpenService.open([params["id"]]);
          }
          this.getStream(params["id"], true);
        }
        else {
          this.streamToLoad = params["id"];
        }
      }
    }));

    this.subscriptions.push(this.onUpdate.subscribe(data => {
      this._onUpdate(data);
    }));
    this.socketService.registerUpdate(this.onUpdate);

    this.subscriptions.push(this.onKeySubject.pipe(
      throttleTime(3000)
    ).subscribe(() => {
      this.meService.typing().subscribe(result => {
        if (typeof(result) != "undefined") {
          this.upService.up();
        }
      });
    }));

    this.subscriptions.push(this.onClickSubject.pipe(
//      throttleTime(100)
    ).subscribe(() => {
      this.meService.clicking().subscribe(result => {
        if (typeof(result) != "undefined") {
          this.upService.up();
        }
      });
    }));

  }

  @HostListener('window:focus', ['$event'])
  onFocus(event: any): void {
    console.log("windowFocus");
    this.nowInFront();
  }

  @HostListener('window:blur', ['$event'])
  onBlur(event: any): void {
    console.log("windowBlur");
    this.nowBehind();
  }

  private nowBehind() {
    this.inFront = false;
    this.lastInFront = moment();
  }

  private nowInFront() {
    this.onClick();
    if (!this.inFront) {
      var reload = false;
      if (this.lastInFront) {
        let down = moment.duration(moment().diff(this.lastInFront)).asSeconds();
        if (down > 3) {
          reload = true;
       }
      }
      if (reload) {
        if (this.stream) {
          this.getStream(this.stream._id, false);
        }
      }
      else {
        this.readIdeas();
      }
      this.inFront = true;
    }
  }

  ngOnDestroy() {
    this.subscriptions.forEach(s => s.unsubscribe());
  }

  private scrollToInitial() {
    this.scrollable.scrollTo({ bottom: 0 });
  }

  ngOnInit() {
    this.onResize(null);
  }

  ngAfterViewInit(): void {

    this.mutationObserver = new MutationObserver((mutations) => {
      this.scrollToInitial();
    });
    this.mutationObserver.observe(this.scrollable.getElementRef().nativeElement, { childList: true });

  }

  private _onUpdate(data: any) {
//    console.log("conversation", "onUpdate", data);
    if (this.stream && this.stream._id && data.objtype == "stream" && data.id == this.stream._id) {
      if (data.idea) {
        if (!this.hasIdea(data.idea)) {
          this.ideaService.getIdea(data.idea, this.token).subscribe(idea => {
            this._addItem("idea", idea, idea.modifyDate);
          });
        }
      }
      else {
        this._itemsChanged();
      }
    }
  }


  public breakAt(size: number): boolean {
    return this.breakpointObserver.isMatched("(max-width: " + size + "px)");
  }

  public isSmall(): boolean {
    return this.breakAt(960);
  }

  private hasIdea(id: String): boolean {
    return this.items ? this.items.filter(e => e.type == "idea" && e.object._id == id).length == 1 : false;
  }

  displayActionsSpacer(): string {
    return this.listContent.nativeElement.clientWidth > 590 ? 'block' : 'none';
  }

  triggerResize() {
    // Wait for changes to be applied, then trigger textarea resize.
    this.ngZone.onStable.pipe(take(1))
        .subscribe(() => this.autosize.resizeToFitContent(true));
  }

  onPage(event: any) {
    this.pageSize = event.pageSize;
    this.start = event.pageIndex * event.pageSize;
    this.getConversation();
  }

  onChanged(event: any) {
    if (event.changed) {
      this._itemsChanged();
    }
    else if (event.removed) {
      let index = this.items.findIndex(e => e.object._id == event.removed);
      if (index >= 0) {
        this.items.splice(index, 1);
        this.length -= 1;
      }
      else {
        this._itemsChanged();
      }
    }
  }

  getStream(stream: string, reset: boolean): void {

//    console.log("getStream", stream);

    this.start = 0;

    if (this.paginator && reset) {
      this.paginator.pageIndex = 0;
    }
    this.loading = true;
    this.streamService.getStream(stream, this.token).subscribe(stream => {

        this.loading = false;
        this.stream = stream;
        this.streamflags = new StreamFlags(this.stream.streambits);

        if (reset) {
          this.pageSize =  30;
          this.pageSizeOptions = [10, 30, 60];
        }
        this.getConversation();
        this.onResize(null);
      });
  }

  private getSingleIdea(id: string, hasstreams: boolean) {

    console.log("getSingleIdea");

    this.items = [];

    if (id == "none") {
      return;
    }

    this.loading = true;
    this.ideaService.getIdea(id, this.token).subscribe(idea => {
      this.loading = false;
      this.focusIdea = true;
      this.focusIdeaStream = idea.stream;
      this.start = 0;
      this.pageSize = 1;
      this.pageSizeOptions = [1];
      if (hasstreams) {
        this.streamOpenService.open([idea.stream]);
      }
    });
  }

  private getConversation(): void {
//    console.log("getConversation");
    this.items = [];
    this.allLoaded = false;
    this.conversationService.getConversation(this.stream._id, this.start, this.pageSize)
      .subscribe(resp => {

        if (resp && resp.body) {
          this.length = this.ideaService.getTotal(resp);
          var newstreams = [];
          resp.body.forEach(e => {
            this._addItem(e.itemType, e, e.itemType == "message" ? e.date : e.modifyDate);
          });
          this.readIdeas();
          if (this.items.length == 0) {
            this.items = null;
          }
          else {
            this.calcSeps();
          }
        }

      });
  }

  private calcSeps(): void {
//    console.log("calcSeps");
    var lastDay = null;
    this.items.forEach(e => {
      let d = moment(e.object.modifyDate).format("DD MM YYYY");
      e.showSep = lastDay ? lastDay != d : true;
      lastDay = d;
    });
  }

  private readIdeas() {
    // TBD
  }

  private unreadIdea(idea: any): boolean {
    return false;
    // TBD
  }

  private _itemsChanged(): void {
    if (this.stream) {
      console.log("items changed");
      this.getStream(this.stream._id, false);
    }
  }

  private _addItem(type: string, object: any, date: Date): void {
//    console.log("_addItem");
    if (this.items) {
      this.items = this.items.filter(e => e.object._id != object._id);
    }
    else {
      this.items = [];
    }
    let item = { type: type, date: date, object: object } as Item;
    this.items.push(item);
  }

  getUserIcon(message: Idea): string {
    return "??";
  }

  getIcon(obj: any): string {
    return this.iconService.getIcon(obj);
  }

  private finishAddMessage(message: Idea): void {

    this._addItem("message", message, message.date);
    this.calcSeps();
    this.message = "";

    message.stream = this.stream._id;

    this.ideaService.addIdea(message).subscribe(i => {
      message.date = i.date;
      message._id = i._id;
      message.needsSend = false;
    });
  }

  addMessage(): void {

    if (this.message.length > 0) {
      let message = new Idea();
      // not needed when sending, but needed on the client
      // side
      message.user = this.me._id;
      message.needsSend = true;
      message.text = this.message;
      this.finishAddMessage(message);
    }
  }

  addImage(): void {

    let message = new Idea();
    message.user = this.me._id;
    message.needsSend = true;
    message.text = "Image";

    this._addItem("message", message, message.date);
    this.calcSeps();
    this.message = "";

    message.stream = this.stream._id;

    this.ideaService.addIdea(message).subscribe(i => {
      message.modifyDate = i.modifyDate;
      message._id = i._id;
      message.needsSend = false;
      this.router.navigateByUrl("/image-upload/" + message._id);
    });

  }

  showKeyboard(event): void {
   this.innerHeight = window.innerHeight;
  }

  hideKeyboard(event): void {
    if (this.innerHeight != window.innerHeight) {
      (window as any).innerHeight = this.innerHeight;
    }
  }

  onResize(event: any) {
//    console.log("onResize");
    this.height = (window.innerHeight-(this.isSmall() ? 40 : 64)) + "px";
    this.peopleOpened = !this.isSmall();
  }

  footerWidth(): string {
    return "100%";
  }

  private replyWidthNum(): number {
    if (this.listContent.nativeElement.clientWidth < 606) {
      return this.listContent.nativeElement.clientWidth - 48;
    }
    return 528;
  }

  replyWidth(): string {
    return this.replyWidthNum() + "px";
  }

  innerReplyWidth(): string {
    return (this.replyWidthNum()-8) + "px";
  }

  ideaWidth(): string {
    let width = window.innerWidth - 8;
    return (width < 530 ? width : 530) + "px";
  }

  hidePageSize(): boolean {
    return true;
  }

  hidePaginator(): boolean {
    return this.isSmall();
  }

  displayPaginator(): string {
    return this.hidePaginator() ? 'none': 'block';
  }

  ideaShowSep(item: any): boolean {
    return item.showSep;
  }

  onEnter(event): void {
    event.stopPropagation();
    event.preventDefault();
    this.addMessage();
  }

  onClick(): void {
    // TBD
  }

  streamBgColor(): string {
    return this.site && this.site.streamBgColor ? this.site.streamBgColor : "#7096cc";
  }

  cancelFocus() {
    this.focusIdea = false;
    this.router.navigateByUrl("/streams/" + this.focusIdeaStream);
  }

  private hasAdmin(): boolean {
    return this.me && this.me.admin;
  }

  hasMore(): boolean {
    return this.paginator ? this.paginator.hasNextPage() : false;
  }

  more() {
    this.paginator.pageIndex = this.paginator.pageIndex + 1;
    this.start = this.paginator.pageIndex * this.paginator.pageSize;
    this.getConversation();
  }

  hasLess(): boolean {
    return this.paginator ? this.paginator.hasPreviousPage() : false;
  }

  less() {
    this.paginator.pageIndex = this.paginator.pageIndex - 1;
    this.start = this.paginator.pageIndex * this.paginator.pageSize;
    this.getConversation();
  }

  getUserName(id: string): Observable<[string]> {
    return this.userListService.getUserName(id, this.stream);
  }
  urlify(text: string): string {
    return text.replace(this.urlRegex, url => {
        return '<a href="' + url + '" target=�_blank�>' + url + '</a>';
    });
  }
  onKey(event): void {
    this.onKeySubject.next(true);
  }
  dateDividerColor(): string {
    return "#edf5ff";
  }
  itemColor(me: boolean): string {
    if (me) {
      return "#d6e7ff";
    }
    else {
      return "#ffffff";
    }
  }
  dateColor(): string {
    return "#cfd9e8";
  }

  showStreams(): void {
    this.streamOpenService.open([]);
  }

  share(stream: Stream): void {
    this.dialog.open(ShareStreamDialogComponent, { data: { stream: stream._id, streamflags: this.streamflags }}).afterClosed().subscribe(result => {});
  }

	peopleOpen(): boolean {
	  return this.peopleOpened;
	}
  showPeople(): void {
    this.peopleOpened = true;
  }
	hidePeople(): void {
    this.peopleOpened = false;
	}

}
