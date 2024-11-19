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

@Injectable()
export class UserService extends BackendService {

  private usersUrl = '/rest/1.0/users';
  private adminUsersUrl = '/rest/1.0/rawusers';

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  getUsers(admin: boolean, offset: number, limit: number): Observable<HttpResponse<User[]>> {
    const url = `${admin ? this.adminUsersUrl : this.usersUrl}?offset=${offset}&limit=${limit}`;
    return this.http.get<User[]>(url, { observe: 'response' })
    .pipe(
      catchError(this.handleResponseError<User[]>('getUsers', []))
    );
  }

  getUser(admin: boolean, id: string): Observable<User> {
    const url = `${admin ? this.adminUsersUrl : this.usersUrl}/${id}`;
    return this.http.get<User>(url).pipe(
      catchError(this.handleError<User>(`getUser id=${id}`))
    );
  }

  getUserNoError(id: string): Observable<User> {
    const url = `${this.adminUsersUrl}/${id}`;
    return this.http.get<User>(url).pipe(
      catchError(this.passError<User>(`getUserNoError id=${id}`))
    );
  }

  updateUser (user: User): Observable<any> {
    const url = `${this.usersUrl}/${user._id}`;
    return this.http.put<User>(url, user, { headers: this.httpHeaders() }).pipe(
      catchError(this.handleError<User>('updateUser'))
    );
  }

  addUser (user: User): Observable<User> {
    return this.http.post<User>(this.usersUrl, user, { headers: this.httpHeaders() } ).pipe(
      catchError(this.handleError<User>('addUser'))
    );
  }

  deleteUser (user: User | string): Observable<User> {
    const id = typeof user === 'string' ? user : user._id;
    const url = `${this.adminUsersUrl}/${id}`;

    return this.http.delete<User>(url, { headers: this.httpHeaders() }).pipe(
      catchError(this.handleError<User>('deleteUser'))
    );
  }

  searchUsers(term: string): Observable<User[]> {
    if (!term.trim()) {
      // if not search term, return empty user array.
      return of([]);
    }
    return this.http.get<User[]>(`${this.usersUrl}/?q=${term}`).pipe(
      catchError(this.handleError<User[]>('searchUsers', []))
    );
  }

  getPurgeCount(): Observable<any> {
    const url = `${this.adminUsersUrl}/purgecount`;
    return this.http.get<any>(url).pipe(
      catchError(this.handleError<any>(`getPurgeCount`))
    );
  }

  purge(): Observable<any> {
    return this.http.post<any>(this.adminUsersUrl, { purge: true }, { headers: this.httpHeaders() } ).pipe(
      catchError(this.handleError<any>('purge'))
    );
  }

}
