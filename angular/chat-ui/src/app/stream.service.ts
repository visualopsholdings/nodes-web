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

  private handleSecurityError<Stream> (stream: string) {
    return (error: any): Observable<Stream> => {

      if (error.status == 401) {
        var url = "/apps/login/#/login?app=conversations&stream=" + stream;
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

  getStream(id: string): Observable<Stream> {
    var url = `${this.streamsUrl}/${id}`;
    return this.http.get<Stream>(url).pipe(
      catchError(this.handleSecurityError<Stream>(id)),
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

}
