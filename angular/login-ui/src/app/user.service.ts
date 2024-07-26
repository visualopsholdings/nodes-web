import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { HttpClient, HttpHeaders, HttpResponse } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';

import { BackendService } from './backend.service';
import { User } from './user';
import { Message } from './message';
import { SocketService }  from './socket.service';
import { UpService }  from './up.service';

const httpOptions = {
  headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};

@Injectable()
export class UserService extends BackendService {

  private usersUrl = '/rest/1.0/users';

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  newUser (user: User): Observable<Message> {
    return this.http.post<Message>(`${this.usersUrl}/new`, user, httpOptions).pipe(
      catchError(this.handleError<Message>('newUser'))
    );
  }

  getUsers(offset: number, limit: number): Observable<HttpResponse<User[]>> {
    const url = `${this.usersUrl}?offset=${offset}&limit=${limit}`;
    return this.http.get<User[]>(url, { observe: 'response' })
    .pipe(
     catchError(this.handleResponseError<User[]>('getUsers', []))
    );
  }

}
