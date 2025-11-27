import { Injectable } from '@angular/core';
import { Observable ,  of } from 'rxjs';
import { HttpClient, HttpHeaders, HttpResponse } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';

import { Media } from './media';

import { BackendService } from './backend.service';
import { SocketService }  from './socket.service';
import { UpService }  from './up.service';

const httpOptions = {
  headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};

@Injectable()
export class MediaService extends BackendService {

  private admMediaUrl = '/rest/1.0/rawmedia';
  private mediaUrl = '/rest/1.0/media';

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  getMedia(admin: boolean, offset: number, limit: number): Observable<HttpResponse<Media[]>> {
    const url = `${admin ? this.admMediaUrl : this.mediaUrl}?offset=${offset}&limit=${limit}`;
    return this.http.get<Media[]>(url, { observe: 'response' })
    .pipe(
      catchError(this.handleResponseError<Media[]>('getMedia', []))
    );
  }

  deleteMedia (admin: boolean, media: Media | string): Observable<Media> {
    const id = typeof media === 'string' ? media : media._id;
    const url = `${admin ? this.admMediaUrl : this.mediaUrl}/${id}`;
    return this.http.delete<Media>(url, httpOptions).pipe(
//      tap(_ => this.log(`deleted media id=${id}`)),
      catchError(this.handleError<Media>('deleteMedia'))
    );
  }

  getPurgeCount(): Observable<any> {
    const url = `${this.admMediaUrl}/purgecount`;
    return this.http.get<any>(url).pipe(
      catchError(this.handleError<any>(`getPurgeCount`))
    );
  }

  purge(): Observable<any> {
    return this.http.post<any>(this.admMediaUrl, { purge: true }, { headers: this.httpHeaders() } ).pipe(
      catchError(this.handleError<any>('purge'))
    );
  }

}
