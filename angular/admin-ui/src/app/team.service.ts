import { Injectable } from '@angular/core';
import { Observable ,  of } from 'rxjs';
import { HttpClient, HttpHeaders, HttpResponse } from '@angular/common/http';
import { catchError, map, tap } from 'rxjs/operators';
import { MatDialog, MatDialogRef } from '@angular/material/dialog';

import { BackendService } from './backend.service';
import { Team } from './team';
import { TeamMember } from './team-member';
import { SocketService }  from './socket.service';
import { UpService }  from './up.service';

@Injectable()
export class TeamService extends BackendService {

  private admTeamsUrl = '/rest/1.0/rawgroups';
  private teamsUrl = '/rest/1.0/groups';

  constructor(
    dialog: MatDialog,
    socketService: SocketService,
    upService: UpService,
    http: HttpClient,
  ) {
    super(dialog, socketService, upService, http)
  }

  getTeams(admin: boolean, offset: number, limit: number): Observable<HttpResponse<Team[]>> {
    const url = `${admin ? this.admTeamsUrl : this.teamsUrl}?offset=${offset}&limit=${limit}`;
    return this.http.get<Team[]>(url, { observe: 'response' })
    .pipe(
//      tap(teams => this.log(`fetched teams`)),
      catchError(this.handleResponseError<Team[]>('getTeams', []))
    );
  }

  getTeam(admin: boolean, id: string): Observable<Team> {
    const url = `${admin ? this.admTeamsUrl : this.teamsUrl}/${id}`;
    return this.http.get<Team>(url).pipe(
//      tap(_ => this.log(`fetched team id=${id}`)),
      catchError(this.handleError<Team>(`getTeam id=${id}`))
    );
  }

  getTeamMembers(admin: boolean, id: string): Observable<TeamMember[]> {
    const url = `${admin ? this.admTeamsUrl : this.teamsUrl}/${id}/users`;
    return this.http.get<TeamMember[]>(url)
    .pipe(
//      tap(teams => this.log(`fetched teams`)),
      catchError(this.handleError('getTeamMembers', []))
    );
  }

  addTeamMember(id: string, member: TeamMember): Observable<TeamMember> {
    const url = `${this.teamsUrl}/${id}/users`;
    return this.http.post<TeamMember>(url, member, { headers: this.httpHeaders() })
    .pipe(
//      tap(teams => this.log(`fetched teams`)),
      catchError(this.handleError<TeamMember>('addTeamMember'))
    );
  }

  removeTeamMember(id: string, member: TeamMember): Observable<TeamMember> {
    const url = `${this.teamsUrl}/${id}/users/${member._id}`;
    return this.http.delete<TeamMember>(url, { headers: this.httpHeaders() })
    .pipe(
//      tap(teams => this.log(`fetched teams`)),
      catchError(this.handleError<TeamMember>('removeTeamMember'))
    );
  }
}
