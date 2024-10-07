import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { HttpClientModule } from '@angular/common/http';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { MatProgressBarModule } from '@angular/material/progress-bar';
import { ClipboardModule } from '@angular/cdk/clipboard';

import { MatButtonModule } from '@angular/material/button';
import { MatCheckboxModule } from '@angular/material/checkbox';
import { MatToolbarModule } from '@angular/material/toolbar';
import { MatTabsModule } from '@angular/material/tabs';
import { MatCardModule } from '@angular/material/card';
import { MatGridListModule } from '@angular/material/grid-list';
import { MatDialogModule } from '@angular/material/dialog';
import { MatFormFieldModule } from '@angular/material/form-field';
import { MatInputModule } from '@angular/material/input';
import { MatListModule } from '@angular/material/list';
import { MatRadioModule } from '@angular/material/radio';
import { MatPaginatorModule } from '@angular/material/paginator';
import { MatIconModule } from '@angular/material/icon';
import { MatChipsModule } from '@angular/material/chips';
import { MatSnackBarModule } from '@angular/material/snack-bar';
import { MatTableModule } from '@angular/material/table';
import { MatMenuModule } from '@angular/material/menu';
import { MatSidenavModule } from '@angular/material/sidenav';
import { MatExpansionModule } from '@angular/material/expansion';
import { MatDatepickerModule } from '@angular/material/datepicker';
import { MatNativeDateModule } from "@angular/material/core";

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { UsersComponent } from './users/users.component';
import { SecurityErrorComponent } from './security-error/security-error.component';
import { RestErrorComponent } from './rest-error/rest-error.component';
import { SystemMaintenanceComponent } from './system-maintenance/system-maintenance.component';
import { IconService }  from './icon.service';
import { MeService }  from './me.service';
import { UserService }  from './user.service';
import { InfosComponent } from './infos/infos.component';
import { InfoService }  from './info.service';
import { ConfirmComponent } from './confirm/confirm.component';
import { NodesComponent } from './nodes/nodes.component';
import { SiteDetailComponent } from './site-detail/site-detail.component';
import { SiteService }  from './site.service';
import { AddUpstreamUserDialogComponent } from './add-upstream-user-dialog/add-upstream-user-dialog.component';
import { UserQueryResultDialogComponent } from './user-query-result-dialog/user-query-result-dialog.component';
import { SocketService }  from './socket.service';
import { TeamsComponent } from './teams/teams.component';
import { TeamDetailComponent } from './team-detail/team-detail.component';
import { AddTeamDialogComponent } from './add-team-dialog/add-team-dialog.component';
import { TeamService }  from './team.service';
import { AddMemberDialogComponent } from './add-member-dialog/add-member-dialog.component';
import { UserDetailComponent } from './user-detail/user-detail.component';
import { SetUpstreamDialogComponent } from './set-upstream-dialog/set-upstream-dialog.component';
import { StreamsComponent } from './streams/streams.component';
import { StreamDetailComponent } from './stream-detail/stream-detail.component';
import { StreamService }  from './stream.service';
import { AddStreamDialogComponent } from './add-stream-dialog/add-stream-dialog.component';
import { SetEmojiDialogComponent } from './set-emoji-dialog/set-emoji-dialog.component';
import { SecurityComponent } from './security/security.component';
import { AddGroupPolicyDialogComponent } from './add-group-policy-dialog/add-group-policy-dialog.component';
import { AddUserPolicyDialogComponent } from './add-user-policy-dialog/add-user-policy-dialog.component';
import { AddGroupPolicyExecDialogComponent } from './add-group-policy-exec-dialog/add-group-policy-exec-dialog.component';
import { AddUserPolicyExecDialogComponent } from './add-user-policy-exec-dialog/add-user-policy-exec-dialog.component';
import { PolicyIdDialogComponent } from './policy-id-dialog/policy-id-dialog.component';
import { PolicyService }  from './policy.service';
import { SetUibitsDialogComponent } from './set-uibits-dialog/set-uibits-dialog.component';
import { NodesService }  from './nodes.service';

@NgModule({
  declarations: [
    AppComponent,
    UsersComponent,
    SecurityErrorComponent,
    RestErrorComponent,
    SystemMaintenanceComponent,
    InfosComponent,
    ConfirmComponent,
    NodesComponent,
    SiteDetailComponent,
    AddUpstreamUserDialogComponent,
    UserQueryResultDialogComponent,
    TeamsComponent,
    TeamDetailComponent,
    AddTeamDialogComponent,
    AddMemberDialogComponent,
    UserDetailComponent,
    SetUpstreamDialogComponent,
    StreamsComponent,
    StreamDetailComponent,
    AddStreamDialogComponent,
    SetEmojiDialogComponent,
    SecurityComponent,
    AddGroupPolicyDialogComponent,
    AddUserPolicyDialogComponent,
    AddGroupPolicyExecDialogComponent,
    AddUserPolicyExecDialogComponent,
    PolicyIdDialogComponent,
    SetUibitsDialogComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    ReactiveFormsModule,
    AppRoutingModule,
    HttpClientModule,
    BrowserAnimationsModule,
    MatProgressBarModule,
    ClipboardModule,
    MatButtonModule,
    MatCheckboxModule,
    MatToolbarModule,
    MatTabsModule,
    MatCardModule,
    MatGridListModule,
    MatDialogModule,
    MatFormFieldModule,
    MatInputModule,
    MatListModule,
    MatRadioModule,
    MatPaginatorModule,
    MatIconModule,
    MatChipsModule,
    MatSnackBarModule,
    MatMenuModule,
    MatTableModule,
    MatSidenavModule,
    MatExpansionModule,
    MatDatepickerModule,
    MatNativeDateModule
  ],
  providers: [IconService, MeService, UserService, InfoService, SiteService, SocketService,
    TeamService, StreamService, PolicyService, NodesService],
  bootstrap: [AppComponent]
})
export class AppModule { }
