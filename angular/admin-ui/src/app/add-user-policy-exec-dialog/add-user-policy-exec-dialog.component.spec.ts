import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AddUserPolicyExecDialogComponent } from './add-user-policy-exec-dialog.component';

describe('AddUserPolicyExecDialogComponent', () => {
  let component: AddUserPolicyExecDialogComponent;
  let fixture: ComponentFixture<AddUserPolicyExecDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [AddUserPolicyExecDialogComponent]
    });
    fixture = TestBed.createComponent(AddUserPolicyExecDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
