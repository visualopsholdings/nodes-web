import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { HttpClient, HttpResponse, HttpHeaders } from '@angular/common/http';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';
import { catchError, map, tap } from 'rxjs/operators';

import { RestErrorComponent } from './rest-error/rest-error.component';
import { SecurityErrorComponent } from './security-error/security-error.component';
import { SocketService }  from './socket.service';
import { UpService }  from './up.service';

@Injectable()
export class BackendService {

  http: HttpClient;
  dialog: MatDialog;

  constructor(
    dialog: MatDialog,
    private socketService: SocketService,
    private upService: UpService,
    http: HttpClient
  ) {
    this.http = http;
    this.dialog = dialog;
  }

  public getTotal<T>(resp: HttpResponse<T>): number {
      let range = resp.headers.get("content-range");
      if (range) {
        let parts = range.split(/\s|-|\//);
        if (parts) {
          return parseInt(parts[2]);
        }
      }
      return 0;
  }

  /**
   * Handle Http operation that failed.
   * Let the app continue.
   * @param operation - name of the operation that failed
   * @param result - optional value to return as the observable result
   */
  public handleError<T> (operation = 'operation', result?: T) {
    return (error: any): Observable<T> => {

      if (error.status == 401) {
         this.dialog.open(SecurityErrorComponent, {
          width: '400px',
          data: { message: error.error.err }
        }).afterClosed().subscribe(success => {
          if (success) {
            window.location.href = "/apps/login/#/login";
          }
        });
      }
      else if (error.status == 0 || error.status == 504) {
        this.upService.down();
      }
      else {
         let dialogRef = this.dialog.open(RestErrorComponent, {
          width: '400px',
          data: { message: error.error.err }
        });
        dialogRef.afterClosed().subscribe(success => {});
      }

      // Let the app keep running by returning an empty result.
      return of(result as T);
    };
  }

  public passError<T> (operation = 'operation', result?: T) {
    return (error: any): Observable<T> => {
      // Let the app keep running by returning an empty result.
      return of(result as T);
    };
  }

  /**
   * Handle Http operation that failed returning a response
   * Let the app continue.
   * @param operation - name of the operation that failed
   * @param result - optional value to return as the observable result
   */
  public handleResponseError<T> (operation = 'operation', result?: T) {
    return (error: any): Observable<HttpResponse<T>> => {

      if (error.status == 401) {
         this.dialog.open(SecurityErrorComponent, {
          width: '400px',
          data: { message: error.error.err }
        }).afterClosed().subscribe(success => {
          if (success) {
            window.location.href = "/apps/login/#/login";
          }
        });
      }
      else if (error.status == 0 || error.status == 504) {
        this.upService.down();
      }
      else {
        this.dialog.open(RestErrorComponent, {
          width: '400px',
          data: { message: error.status == 400 ? error.error.err : error.message }
        }).afterClosed().subscribe(success => {});
      }

      // Let the app keep running by returning an empty result.
      return of(new HttpResponse<T>({ body: result }));
    };
  }

}

