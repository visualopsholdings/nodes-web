import { Injectable } from '@angular/core';
import { Observable ,  of } from 'rxjs';
import { HttpClient, HttpHeaders, HttpResponse } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';

import { Policy } from './policy';

import { BackendService } from './backend.service';
import { SocketService }  from './socket.service';
import { UpService }  from './up.service';

const httpOptions = {
  headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};

@Injectable()
export class PolicyService extends BackendService {

  private baseUrl = '/rest/1.0';

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  getPolicies(obj: string, id: string): Observable<Policy[]> {
    const url = `${this.baseUrl}/${obj}/${id}/policy`;
    return this.http.get<Policy[]>(url)
    .pipe(
      catchError(this.handleError('getPolicies', []))
    );
  }

  updatePolicy(obj: string, id: string, changes: any): Observable<any> {
    const url = `${this.baseUrl}/${obj}/${id}/policy`;
    return this.http.patch(url, changes, httpOptions).pipe(
      catchError(this.handleError<any>('updatePolicy'))
    );
  }

}
