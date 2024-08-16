import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AddUpstreamUserDialogComponent } from './add-upstream-user-dialog.component';

describe('AddUpstreamUserDialogComponent', () => {
  let component: AddUpstreamUserDialogComponent;
  let fixture: ComponentFixture<AddUpstreamUserDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [AddUpstreamUserDialogComponent]
    });
    fixture = TestBed.createComponent(AddUpstreamUserDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
