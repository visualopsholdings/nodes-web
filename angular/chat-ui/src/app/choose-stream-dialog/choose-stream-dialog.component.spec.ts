import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ChooseStreamDialogComponent } from './choose-stream-dialog.component';

describe('ChooseStreamDialogComponent', () => {
  let component: ChooseStreamDialogComponent;
  let fixture: ComponentFixture<ChooseStreamDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [ChooseStreamDialogComponent]
    });
    fixture = TestBed.createComponent(ChooseStreamDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
