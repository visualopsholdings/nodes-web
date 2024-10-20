import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AddUpstreamStreamDialogComponent } from './add-upstream-stream-dialog.component';

describe('AddUpstreamStreamDialogComponent', () => {
  let component: AddUpstreamStreamDialogComponent;
  let fixture: ComponentFixture<AddUpstreamStreamDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [AddUpstreamStreamDialogComponent]
    });
    fixture = TestBed.createComponent(AddUpstreamStreamDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
