import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

import { ConversationComponent } from './conversation/conversation.component';
import { ImageUploadComponent } from './image-upload/image-upload.component';

const routes: Routes = [
  { path: 'streams/:id', component: ConversationComponent },
  { path: 'image-upload/:id', component: ImageUploadComponent }
];

@NgModule({
  imports: [RouterModule.forRoot(routes, { useHash: true })],
  exports: [RouterModule]
})
export class AppRoutingModule {}
