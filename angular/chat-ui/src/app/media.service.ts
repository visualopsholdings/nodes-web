import { Injectable } from '@angular/core';
import { Observable ,  of } from 'rxjs';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { catchError, map, tap } from 'rxjs/operators';
import { HttpClient, HttpHeaders, HttpResponse } from '@angular/common/http';

import { BackendService } from './backend.service';
import { SocketService }  from './socket.service';
import { UpService }  from './up.service';

@Injectable({
  providedIn: 'root'
})
export class MediaService extends BackendService {

  private mediaUrl = '/rest/1.0/media';

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  uploadNewMedia(form: FormData): Observable<any> {

    const url = `${this.mediaUrl}/upload`;
    var args = { reportProgress: true, observe: 'events' } as any;
    var post = this.httpPostHeaders();
    if (post) {
      args.headers = post;
    }
    return this.http.post<any>(url, form, args).pipe(
      catchError(this.handleError<any>('uploadNewMedia', []))
    );

  }

}
