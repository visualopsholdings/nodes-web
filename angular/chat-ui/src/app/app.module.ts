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
import { MatProgressSpinnerModule } from '@angular/material/progress-spinner';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { SecurityErrorComponent } from './security-error/security-error.component';
import { RestErrorComponent } from './rest-error/rest-error.component';
import { SystemMaintenanceComponent } from './system-maintenance/system-maintenance.component';
import { IconService }  from './icon.service';
import { MeService }  from './me.service';
import { StreamService }  from './stream.service';
import { StreamsComponent } from './streams/streams.component';
import { PeopleComponent } from './people/people.component';
import { AddStreamDialogComponent } from './add-stream-dialog/add-stream-dialog.component';
import { ConfirmComponent } from './confirm/confirm.component';
import { SortStreamPipe } from './sort-stream.pipe';
import { ConversationComponent } from './conversation/conversation.component';
import { IdeaComponent } from './idea/idea.component';
import { IdeaService }  from './idea.service';
import { EditIdeaDialogComponent } from './edit-idea-dialog/edit-idea-dialog.component';
import { ChooseStreamDialogComponent } from './choose-stream-dialog/choose-stream-dialog.component';
import { JustNowPipe } from './just-now.pipe';
import { UserListService }  from './user-list.service';
import { UserService }  from './user.service';

@NgModule({
  declarations: [
    AppComponent,
    SecurityErrorComponent,
    RestErrorComponent,
    SystemMaintenanceComponent,
    StreamsComponent,
    PeopleComponent,
    AddStreamDialogComponent,
    ConfirmComponent,
    SortStreamPipe,
    ConversationComponent,
    IdeaComponent,
    EditIdeaDialogComponent,
    ChooseStreamDialogComponent,
    JustNowPipe
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
    MatNativeDateModule,
    MatProgressSpinnerModule
  ],
  providers: [IconService, MeService, StreamService, IdeaService, UserListService, UserService],
  bootstrap: [AppComponent]
})
export class AppModule { }
