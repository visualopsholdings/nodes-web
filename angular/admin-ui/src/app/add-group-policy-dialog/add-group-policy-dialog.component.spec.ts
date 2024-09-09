import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AddGroupPolicyDialogComponent } from './add-group-policy-dialog.component';

describe('AddGroupPolicyDialogComponent', () => {
  let component: AddGroupPolicyDialogComponent;
  let fixture: ComponentFixture<AddGroupPolicyDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [AddGroupPolicyDialogComponent]
    });
    fixture = TestBed.createComponent(AddGroupPolicyDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
