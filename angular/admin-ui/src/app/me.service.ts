import { Injectable } from '@angular/core';
import { Observable ,  of } from 'rxjs';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';

import { BackendService } from './backend.service';
import { SocketService }  from './socket.service';
import { UpService }  from './up.service';
import { Me }  from './me';

const httpOptions = {
  headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};

@Injectable()
export class MeService extends BackendService {

  private meUrl = '/rest/1.0/users/me';

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  getMe(): Observable<Me> {
    return this.http.get<Me>(this.meUrl).pipe(
      catchError(this.handleError<Me>("getMe"))
    );
  }

}
