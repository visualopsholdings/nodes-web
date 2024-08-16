import { ComponentFixture, TestBed } from '@angular/core/testing';

import { UserQueryResultDialogComponent } from './user-query-result-dialog.component';

describe('UserQueryResultDialogComponent', () => {
  let component: UserQueryResultDialogComponent;
  let fixture: ComponentFixture<UserQueryResultDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [UserQueryResultDialogComponent]
    });
    fixture = TestBed.createComponent(UserQueryResultDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
