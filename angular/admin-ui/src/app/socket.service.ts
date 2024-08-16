import { Injectable, EventEmitter } from '@angular/core';
import { Observable } from 'rxjs';

import * as moment from 'moment';
import * as io from 'socket.io-client';

import { User }  from './user';
import { UpService }  from './up.service';

@Injectable()
export class SocketService {

  // Our socket connection
  private socket;

  private qrHandlers = [];

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
      this.socket = io();
    }

    this.socket.on("connect", () => {
      logger.log("connect", this.socket.id);
    });
    this.socket.on("disconnect", (reason) => {
      logger.log("disconnect", reason);
    });

    this.socket.on('id', () => {
      this.upService.connected();
      observer.next("socket ready");
    });

    this.socket.on('queryResult', (data) => {
//      logger.log("socket", "queryResult", data);
      this.qrHandlers.forEach(e => {
        e.emitter.emit(data);
      });
    });

    this.socket.on('connect_error', (err) => {
      logger.log("e." + err.type, err);
        this.upService.down();
    });
    this.socket.on('connect_timeout', () => {
      logger.log("t");
    });
    this.socket.on('reconnect', (n) => {
      this.clearStart();
      let down = moment.duration(moment().diff(this.lastDown)).asSeconds();
      logger.log("r." + n + ".d." + down);
      this.upService.reconnected(n > 2 || down > 10);
    });
    this.socket.on('reconnect_attempt', () => {
      logger.log("ra");
      this.clearStart();
      this.timeout = setTimeout(() => {
        this.start(me, logger, observer);
      }, 1000);
    });
    this.socket.on('reconnecting', () => {
      logger.log("ring");
      this.clearStart();
      this.lastDown = moment();
      this.upService.reconnecting();
    });
    this.socket.on('reconnect_error', (err) => {
      logger.log("re." + err.type, err);
    });
    this.socket.on('reconnect_failed', () => {
      logger.log("rf");
    });
  }

  startDown(logger: any, observer: EventEmitter<string>) {

    if (!logger) {
      logger = { log: (s) => {} }
    }

    this.upService.disconnected();

    if (!this.socket) {
      this.socket = io();
    }

    this.socket.on('reconnect', (n) => {
      this.clearStart();
      let down = moment.duration(moment().diff(this.lastDown)).asSeconds();
      logger.log("dr." + n + ".d." + down);
      this.upService.reconnected(n > 2 || down > 10);
    });
    this.socket.on('reconnect_attempt', () => {
      logger.log("dra");
      this.clearStart();
      this.timeout = setTimeout(() => {
        this.startDown(logger, observer);
      }, 1000);
    });
    this.socket.on('reconnecting', () => {
      logger.log("dring");
      this.clearStart();
      this.lastDown = moment();
      this.upService.reconnecting();
    });
    this.socket.on('reconnect_error', (err) => {
      logger.log("dre." + err.type, err);
    });
    this.socket.on('reconnect_failed', () => {
      logger.log("drf");
    });

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

  id(): string {
    return this.socket ? this.socket.id : null;
  }

  open(me: User, id: string): void {
		this.socket.emit('openDocument', { id: id, userid: me._id });
	}

}
