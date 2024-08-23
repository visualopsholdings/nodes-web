import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AddMemberDialogComponent } from './add-member-dialog.component';

describe('AddMemberDialogComponent', () => {
  let component: AddMemberDialogComponent;
  let fixture: ComponentFixture<AddMemberDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [AddMemberDialogComponent]
    });
    fixture = TestBed.createComponent(AddMemberDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
