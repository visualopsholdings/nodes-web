import { Injectable } from '@angular/core';
import { Observable ,  of } from 'rxjs';
import { HttpClient, HttpHeaders, HttpResponse } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';

import { BackendService } from './backend.service';
import { Site } from './site';
import { SocketService }  from './socket.service';
import { UpService }  from './up.service';

const httpOptions = {
  headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};

@Injectable()
export class SiteService extends BackendService {

  private sitesUrl = '/rest/1.0/sites';

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  getSite(): Observable<Site> {
    const url = `${this.sitesUrl}`;
    return this.http.get<Site>(url).pipe(
      catchError(this.handleError<Site>(`getSite`))
    );
  }

  updateSite (site: Site): Observable<any> {
    const url = `${this.sitesUrl}`;
    return this.http.put(url, site, httpOptions).pipe(
      catchError(this.handleError<any>('updateSite'))
    );
  }

}
