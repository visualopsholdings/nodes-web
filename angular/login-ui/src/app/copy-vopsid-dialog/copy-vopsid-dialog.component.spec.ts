import { ComponentFixture, TestBed } from '@angular/core/testing';

import { CopyVopsidDialogComponent } from './copy-vopsid-dialog.component';

describe('CopyVopsidDialogComponent', () => {
  let component: CopyVopsidDialogComponent;
  let fixture: ComponentFixture<CopyVopsidDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [CopyVopsidDialogComponent]
    });
    fixture = TestBed.createComponent(CopyVopsidDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
