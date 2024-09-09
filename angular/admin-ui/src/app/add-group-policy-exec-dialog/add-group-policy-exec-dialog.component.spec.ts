import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AddGroupPolicyExecDialogComponent } from './add-group-policy-exec-dialog.component';

describe('AddGroupPolicyExecDialogComponent', () => {
  let component: AddGroupPolicyExecDialogComponent;
  let fixture: ComponentFixture<AddGroupPolicyExecDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [AddGroupPolicyExecDialogComponent]
    });
    fixture = TestBed.createComponent(AddGroupPolicyExecDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
