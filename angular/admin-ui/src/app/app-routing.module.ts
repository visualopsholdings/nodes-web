import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { TeamsComponent }      from './teams/teams.component';
import { TeamDetailComponent }  from './team-detail/team-detail.component';
import { UsersComponent }  from './users/users.component';
import { UserDetailComponent } from './user-detail/user-detail.component';
import { InfosComponent } from './infos/infos.component';
import { NodesComponent } from './nodes/nodes.component';
import { SiteDetailComponent } from './site-detail/site-detail.component';
import { StreamsComponent } from './streams/streams.component';
import { StreamDetailComponent } from './stream-detail/stream-detail.component';

const routes: Routes = [
  { path: '', redirectTo: '/infos', pathMatch: 'full' },
  { path: 'users', component: UsersComponent },
  { path: 'users', component: UsersComponent },
  { path: 'users/:id', component: UserDetailComponent },
  { path: 'streams', component: StreamsComponent },
  { path: 'streams/:id', component: StreamDetailComponent },
  { path: 'teams', component: TeamsComponent },
  { path: 'teams/:id', component: TeamDetailComponent },
  { path: 'infos', component: InfosComponent },
  { path: 'nodes', component: NodesComponent },
  { path: 'sites', component: SiteDetailComponent },
];

@NgModule({
  imports: [RouterModule.forRoot(routes, { useHash: true })],
  exports: [RouterModule]
})
export class AppRoutingModule {}
