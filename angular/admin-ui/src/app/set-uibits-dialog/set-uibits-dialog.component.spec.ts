import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SetUibitsDialogComponent } from './set-ui-bits-dialog.component';

describe('SetUibitsDialogComponent', () => {
  let component: SetUibitsDialogComponent;
  let fixture: ComponentFixture<SetUibitsDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [SetUibitsDialogComponent]
    });
    fixture = TestBed.createComponent(SetUibitsDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
