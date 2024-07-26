import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { HttpClient, HttpHeaders, HttpResponse } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';

import { BackendService } from './backend.service';
import { Idea }  from './idea';
import { SocketService }  from './socket.service';
import { UpService }  from './up.service';

@Injectable({
  providedIn: 'root'
})
export class ConversationService extends BackendService {

  private conversationsUrl = '/rest/1.0/conversations';

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  getConversation(conversation: string, offset: number, limit: number): Observable<HttpResponse<Idea[]>> {

    var url = `${this.conversationsUrl}/${conversation}?offset=${offset}&limit=${limit}`;
    return this.http.get<Idea[]>(url, { observe: 'response' })
    .pipe(
      catchError(this.handleResponseError<Idea[]>('getConversation', []))
    );

  }

}
