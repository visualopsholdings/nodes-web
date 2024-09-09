import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AddStreamDialogComponent } from './add-stream-dialog.component';

describe('AddStreamDialogComponent', () => {
  let component: AddStreamDialogComponent;
  let fixture: ComponentFixture<AddStreamDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [AddStreamDialogComponent]
    });
    fixture = TestBed.createComponent(AddStreamDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
