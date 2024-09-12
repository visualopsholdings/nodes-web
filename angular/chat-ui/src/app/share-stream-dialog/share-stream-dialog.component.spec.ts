import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ShareStreamDialogComponent } from './share-stream-dialog.component';

describe('ShareStreamDialogComponent', () => {
  let component: ShareStreamDialogComponent;
  let fixture: ComponentFixture<ShareStreamDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [ShareStreamDialogComponent]
    });
    fixture = TestBed.createComponent(ShareStreamDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
