import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { HttpClient, HttpHeaders, HttpResponse } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';

import * as _ from 'lodash';

import { BackendService } from './backend.service';
import { Idea }  from './idea';
import { SocketService }  from './socket.service';
import { Stream }  from './stream';
import { Me }  from './me';
import { UpService }  from './up.service';
import { RestErrorComponent } from './rest-error/rest-error.component';

@Injectable()
export class IdeaService extends BackendService {

  private ideaUrl = '/rest/1.0/ideas'

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  private handleSecurityError<Idea> (idea: string, token: string) {
    return (error: any): Observable<Idea> => {

      if (error.status == 401) {
        var url = "/apps/login/#/login?app=chat&idea=" + idea;
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
          data: { err: error.error.err, message: error.message }
        });
        dialogRef.afterClosed().subscribe(success => {});
      }

      // Let the app keep running by returning an empty result.
      return of({} as Idea);
    };
  }

  getIdea(id: string, token: string = null): Observable<Idea> {
    const url = `${this.ideaUrl}/${id}`;
    return this.http.get<Idea>(url).pipe(
      catchError(this.handleSecurityError<Idea>(id, token))
    );
  }

  addIdea(idea: Idea): Observable<Idea> {

    var url = `${this.ideaUrl}`;

    return this.http.post<Idea>(url, idea, { headers: this.httpHeaders() }).pipe(
      catchError(this.handleError<Idea>('addIdea'))
    );
  }

}
