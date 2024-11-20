import { Injectable } from '@angular/core';
import { Observable ,  of } from 'rxjs';
import { HttpClient, HttpHeaders, HttpResponse } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';

import { BackendService } from './backend.service';
import { Idea } from './idea';
import { SocketService }  from './socket.service';
import { UpService }  from './up.service';

const httpOptions = {
  headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};

@Injectable()
export class IdeaService extends BackendService {

  private admStreamsUrl = '/rest/1.0/rawstreams';

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  getPurgeCount(stream: string): Observable<any> {
    const url = `${this.admStreamsUrl}/${stream}/purgecount`;
    return this.http.get<any>(url).pipe(
      catchError(this.handleError<any>(`getPurgeCount`))
    );
  }

  purge(stream: string): Observable<any> {
    const url = `${this.admStreamsUrl}/${stream}`;
    return this.http.post<any>(url, { purge: true }, { headers: this.httpHeaders() } ).pipe(
      catchError(this.handleError<any>('purge'))
    );
  }

}
