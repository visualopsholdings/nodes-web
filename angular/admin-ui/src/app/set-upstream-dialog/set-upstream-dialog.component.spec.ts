import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SetUpstreamDialogComponent } from './set-upstream-dialog.component';

describe('SetUpstreamDialogComponent', () => {
  let component: SetUpstreamDialogComponent;
  let fixture: ComponentFixture<SetUpstreamDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [SetUpstreamDialogComponent]
    });
    fixture = TestBed.createComponent(SetUpstreamDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
