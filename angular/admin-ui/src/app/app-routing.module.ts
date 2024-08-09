import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { UsersComponent }  from './users/users.component';
import { InfosComponent } from './infos/infos.component';
import { NodesComponent } from './nodes/nodes.component';

const routes: Routes = [
  { path: '', redirectTo: '/infos', pathMatch: 'full' },
  { path: 'users', component: UsersComponent },
  { path: 'infos', component: InfosComponent },
  { path: 'nodes', component: NodesComponent },
];

@NgModule({
  imports: [RouterModule.forRoot(routes, { useHash: true })],
  exports: [RouterModule]
})
export class AppRoutingModule {}
