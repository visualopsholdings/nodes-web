import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AddNodeDialogComponent } from './add-node-dialog.component';

describe('AddNodeDialogComponent', () => {
  let component: AddNodeDialogComponent;
  let fixture: ComponentFixture<AddNodeDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [AddNodeDialogComponent]
    });
    fixture = TestBed.createComponent(AddNodeDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
