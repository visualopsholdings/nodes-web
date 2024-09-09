import { ComponentFixture, TestBed } from '@angular/core/testing';

import { PolicyIdDialogComponent } from './policy-id-dialog.component';

describe('PolicyIdDialogComponent', () => {
  let component: PolicyIdDialogComponent;
  let fixture: ComponentFixture<PolicyIdDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [PolicyIdDialogComponent]
    });
    fixture = TestBed.createComponent(PolicyIdDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
