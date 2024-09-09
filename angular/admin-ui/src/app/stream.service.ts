import { Injectable } from '@angular/core';
import { Observable ,  of } from 'rxjs';
import { HttpClient, HttpHeaders, HttpResponse } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';

import { BackendService } from './backend.service';
import { Stream } from './stream';
import { SocketService }  from './socket.service';
import { UpService }  from './up.service';

@Injectable()
export class StreamService extends BackendService {

  private admStreamsUrl = '/rest/1.0/rawstreams';
  private streamsUrl = '/rest/1.0/streams';

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  getStreams(admin: boolean, offset: number, limit: number): Observable<HttpResponse<Stream[]>> {
    const url = `${admin ? this.admStreamsUrl : this.streamsUrl}?v=count&offset=${offset}&limit=${limit}`;
    return this.http.get<Stream[]>(url, { observe: 'response' })
    .pipe(
      catchError(this.handleResponseError<Stream[]>('getStreams', []))
    );
  }

  getStream(admin: boolean, id: string): Observable<Stream> {
    const url = `${admin ? this.admStreamsUrl : this.streamsUrl}/${id}`;
    return this.http.get<Stream>(url).pipe(
//      tap(_ => this.log(`fetched stream id=${id}`)),
      catchError(this.handleError<Stream>(`getStream id=${id}`))
    );
  }

  updateStream (admin: boolean, stream: Stream): Observable<any> {
    const url = `${admin ? this.admStreamsUrl : this.streamsUrl}/${stream._id}`;
    return this.http.patch(url, stream, { headers: this.httpHeaders() }).pipe(
//      tap(_ => this.log(`updated stream id=${stream._id}`)),
      catchError(this.handleError<any>('updateStream'))
    );
  }

  addStream (stream: Stream): Observable<Stream> {
    return this.http.post<Stream>(this.streamsUrl, stream, { headers: this.httpHeaders() }).pipe(
//      tap((stream: Stream) => this.log(`added stream w/ id=${stream._id}`)),
      catchError(this.handleError<Stream>('addStream'))
    );
  }

  deleteStream (stream: Stream | string): Observable<Stream> {
    const id = typeof stream === 'string' ? stream : stream._id;
    const url = `${this.streamsUrl}/${id}`;
    return this.http.delete<Stream>(url, { headers: this.httpHeaders() }).pipe(
//      tap(_ => this.log(`deleted stream id=${id}`)),
      catchError(this.handleError<Stream>('deleteStream'))
    );
  }

  searchStreams(admin: boolean, term: string): Observable<Stream[]> {
    if (!term.trim()) {
      // if not search term, return empty stream array.
      return of([]);
    }
    return this.http.get<Stream[]>(`${admin ? this.admStreamsUrl : this.streamsUrl}/?q=${term}`).pipe(
//      tap(_ => this.log(`found heroes matching "${term}"`)),
      catchError(this.handleError<Stream[]>('searchStreams', []))
    );
  }

}
