import { Injectable, EventEmitter } from '@angular/core';
import { Observable } from 'rxjs';

import * as moment from 'moment';
//import * as io from 'socket.io-client';

import { User }  from './user';
import { UpService }  from './up.service';

@Injectable()
export class SocketService {

  // Our socket connection
  private socket;
  private _id;

  private statusHandlers = [];
  private qrHandlers = [];
  private nodeSeenHandlers = [];

  private me: User;
  private timeout = null;
  private lastDown = null;

  constructor(
    private upService: UpService
  ) { }

  start(me: User, logger: any, observer: EventEmitter<string>) {

    if (!logger) {
      logger = { log: (s) => {} }
    }

    this.me = me;

    if (!this.socket) {
      this.socket = new WebSocket("/websocket");
    }

    var self = this;
    this.socket.onopen = function(e) {
      console.log("socket open");
    };

    this.socket.onmessage = function(event) {
      var json = JSON.parse(event.data);
      if (json.type == "id") {
        self._id = json.id;
      }
      else if (json.type == "queryResult") {
        self.qrHandlers.forEach(e => {
          e.emitter.emit(json);
        });
      }
      else if (json.type == "status") {
        self.statusHandlers.forEach(e => {
          e.emitter.emit(json);
        });
      }
      else if (json.type == "nodeSeen") {
        self.nodeSeenHandlers.forEach(e => {
          e.emitter.emit(json);
        });
      }
      else {
        console.log("unknown", json.type);
      }
    };

    this.socket.onclose = function(event) {
      if (!event.wasClean) {
        console.log('[close] Connection died');
      }
    };

    this.socket.onerror = function(error) {
      console.log("socket err", error);
    };

  }

  startDown(logger: any, observer: EventEmitter<string>) {

    if (!logger) {
      logger = { log: (s) => {} }
    }

    this.upService.disconnected();
    this.start(null, logger, observer);

  }

  private clearStart() {
    if (this.timeout) {
      clearTimeout(this.timeout);
      this.timeout = null;
    }
  }

  registerQR(ctx: string, emitter: EventEmitter<any>) {
    this.qrHandlers = this.qrHandlers.filter(e => e.ctx != ctx);
    this.qrHandlers.push({ ctx: ctx, emitter: emitter });
  }

  registerStatus(ctx: string, emitter: EventEmitter<any>) {
    this.statusHandlers = this.statusHandlers.filter(e => e.ctx != ctx);
    this.statusHandlers.push({ ctx: ctx, emitter: emitter });
  }

  registerNodeSeen(ctx: string, emitter: EventEmitter<any>) {
    this.nodeSeenHandlers = this.nodeSeenHandlers.filter(e => e.ctx != ctx);
    this.nodeSeenHandlers.push({ ctx: ctx, emitter: emitter });
  }

  id(): string {
    return this.socket ? this._id.toString() : null;
  }

  open(me: User, id: string): void {
//    this.socket.send(JSON.stringify({ type:"openDocument", id: id, userid: me._id }));
	}

}
