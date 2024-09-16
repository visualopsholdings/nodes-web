import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { HttpClient, HttpHeaders, HttpResponse } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';

import { BackendService } from './backend.service';
import { Stream } from './stream';
import { SocketService }  from './socket.service';
import { Idea } from './idea';
import { User } from './user';
import { UpService }  from './up.service';
import { RestErrorComponent } from './rest-error/rest-error.component';
import { Team } from './team';

@Injectable()
export class StreamService extends BackendService {

  private streamsUrl = '/rest/1.0/streams';

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  getStreams(limit: number): Observable<Stream[]> {

    var url = `${this.streamsUrl}?offset=0&limit=${limit}`;
    return this.get<Stream[]>(url, "getStreams");

  }

  private handleSecurityError<Stream> (stream: string, token: string) {
    return (error: any): Observable<Stream> => {

      if (error.status == 401) {
        var url = "/apps/login/#/login?app=chat&stream=" + stream;
        if (token) {
          url += "&token=" + token;
        }
        window.location.href = url;
      }
      else if (error.status == 0 || error.status == 504) {
        this.upService.down();
      }
      else {
         let dialogRef = this.dialog.open(RestErrorComponent, {
          width: '400px',
          data: { err: (error.error ? error.error.err : error.status), message: error.message }
        });
        dialogRef.afterClosed().subscribe(success => {});
      }

      // Let the app keep running by returning an empty result.
      return of({} as Stream);
    };
  }

  getStream(id: string, token: string = null): Observable<Stream> {
    var url = `${this.streamsUrl}/${id}?v=security`;
    return this.http.get<Stream>(url).pipe(
      catchError(this.handleSecurityError<Stream>(id, token)),
    );
  }

  moveIdea(stream: Stream, idea: string): Observable<Stream> {
    let url = `${this.streamsUrl}/${stream._id}/ideas`;
    return this.http.post<Stream>(url, { idea: idea }, { headers: this.httpHeaders() }).pipe(
      catchError(this.handleError<Stream>('moveIdea'))
    );
  }

  getStreamUsers(stream: string): Observable<User[]> {

    return this.get<User[]>(`${this.streamsUrl}/${stream}/policy/users`, "getStreamUsers");

  }

  getStreamTeams(stream: string): Observable<Team[]> {

    return this.get<Team[]>(`${this.streamsUrl}/${stream}/policy/groups`, "getStreamTeams");

  }

  getShareLink(id: string, group: string, expires: string): Observable<any> {
    var url = `${this.streamsUrl}/${id}/sharelink?group=${group}&expires=${expires}`;
    let cachebuster = Math.round(new Date().getTime() / 1000);
    url += "&cb=" + cachebuster;

    return this.http.get<any>(url).pipe(
      catchError(this.handleError<any>(`getShareLink id=${id}`)),
    );
  }

}
