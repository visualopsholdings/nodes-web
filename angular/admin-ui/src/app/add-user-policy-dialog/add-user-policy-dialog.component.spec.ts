import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AddUserPolicyDialogComponent } from './add-user-policy-dialog.component';

describe('AddUserPolicyDialogComponent', () => {
  let component: AddUserPolicyDialogComponent;
  let fixture: ComponentFixture<AddUserPolicyDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [AddUserPolicyDialogComponent]
    });
    fixture = TestBed.createComponent(AddUserPolicyDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
