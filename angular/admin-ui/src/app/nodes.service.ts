import { Injectable } from '@angular/core';
import { Observable ,  of } from 'rxjs';
import { HttpClient, HttpHeaders, HttpResponse } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';

import { BackendService } from './backend.service';
import { Node } from './node';
import { SocketService }  from './socket.service';
import { UpService }  from './up.service';

const httpOptions = {
  headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};

@Injectable()
export class NodesService extends BackendService {

  private nodesUrl = '/rest/1.0/nodes';
  private logsUrl = '/rest/1.0/logs';

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  getNodes(offset: number, limit: number): Observable<HttpResponse<Node[]>> {
    const url = `${this.nodesUrl}?offset=${offset}&limit=${limit}`;
    return this.http.get<Node[]>(url, { observe: 'response' })
    .pipe(
      catchError(this.handleResponseError<Node[]>('getNodes', []))
    );
  }

  getNode(id: string): Observable<Node> {
    const url = `${this.nodesUrl}/${id}`;
    return this.http.get<Node>(url).pipe(
      catchError(this.handleError<Node>(`getNode id=${id}`))
    );
  }

  searchNodes(term: string): Observable<Node[]> {
    if (!term.trim()) {
      // if not search term, return empty site array.
      return of([]);
    }
    return this.http.get<Node[]>(`${this.nodesUrl}/?q=${term}`).pipe(
      catchError(this.handleError<Node[]>('searchNodes', []))
    );
  }

  deleteNode (node: Node): Observable<Node> {
    return this.http.delete<Node>(`${this.nodesUrl}/${node._id}`, httpOptions).pipe(
      catchError(this.handleError<Node>('deleteNode'))
    );
  }

}
