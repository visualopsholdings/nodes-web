import { Injectable } from '@angular/core';
import { Observable ,  of } from 'rxjs';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';

import { Info } from './info';

import { BackendService } from './backend.service';
import { SocketService }  from './socket.service';
import { UpService }  from './up.service';

@Injectable()
export class InfoService extends BackendService {

  private infosUrl = '/rest/1.0/infos';
  private backupsUrl = '/rest/1.0/backups';
  private workingUrl = '/rest/1.0/working';

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  getInfos(): Observable<Info[]> {
    return this.http.get<Info[]>(this.infosUrl)
    .pipe(
      catchError(this.handleError('getInfos', []))
    );
  }

  updateInfos(infos: any): Observable<Info[]> {
    return this.http.post<Info[]>(this.infosUrl, infos, { headers: this.httpHeaders() }).pipe(
      catchError(this.handleError('updateInfos', []))
    );
  }

}
