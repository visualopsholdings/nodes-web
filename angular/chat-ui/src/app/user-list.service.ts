import { Injectable, EventEmitter } from '@angular/core';
import { pipe, Observable, of } from 'rxjs';
import { map } from 'rxjs/operators';
import { Subject }    from 'rxjs';

import * as _ from 'lodash';

import { Me }  from './me';
import { User }  from './user';
import { Stream }  from './stream';
import { UserService }  from './user.service';
import { StreamService }  from './stream.service';
import { StreamOpenService } from './stream-open.service';
import { BootstrapService } from './bootstrap.service';
import { StreamFlags } from '../../../shared-ui/streamflags';

@Injectable({
  providedIn: 'root'
})
export class UserListService {

  private usersChangedSource = new Subject<[User[]]>();
  usersChanged$ = this.usersChangedSource.asObservable();

  private me: Me;
  private users: User[];
  private stream: string;

  constructor(
    private userService: UserService,
    private streamService: StreamService,
    private streamOpenService: StreamOpenService,
    private bootstrapService: BootstrapService,
  ) {
    this.bootstrapService.meChanged$.subscribe(me => {
      this.me = me[0];
      if (this.stream) {
        this.getStreamUsers();
      }
    });
    this.streamOpenService.openChanged$.subscribe(change => {
      if (change.streams && change.streams.length > 0) {
        this.stream = change.streams[0];
        if (this.me) {
          this.getStreamUsers();
        }
      }
    });
  }

  allUsers(): User[] {
    return this.users;
  }

  findUser(id: string): User {
    if (!this.users) {
      return null;
    }
    let user = this.users.filter(e => e._id == id);
    if (user.length > 0) {
      return user[0];
    }
    return null;
  }

  getUserName(id: string, stream: Stream): Observable<[string]> {
    let user = this.findUser(id);
    if (user) {
      if (user.fullname) {
        return of([user.fullname]);
      }
      else {
        return of([user.name]);
      }
    }
    return this.userService.getUser(id).pipe(
      map(e => [(e ? e.fullname : "")])
    );
  }

  private getStreamUsers(): void {

    this.streamService.getStreamUsers(this.stream).subscribe(users => {

      if (users) {

        // protect code from a bad users object.
        if (typeof(users.map) == "function") {
          this.users = users;
        }
        else {
          this.users = [];
  //      console.log("users3", typeof(this.users), typeof(this.users.map), this.users.length);
        }
        this.usersChangedSource.next([this.users]);
      }

    });

  }

}
