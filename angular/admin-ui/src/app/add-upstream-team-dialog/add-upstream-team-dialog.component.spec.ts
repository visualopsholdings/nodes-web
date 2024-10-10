import { ComponentFixture, TestBed } from '@angular/core/testing';

import { AddUpstreamTeamDialogComponent } from './add-upstream-team-dialog.component';

describe('AddUpstreamTeamDialogComponent', () => {
  let component: AddUpstreamTeamDialogComponent;
  let fixture: ComponentFixture<AddUpstreamTeamDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [AddUpstreamTeamDialogComponent]
    });
    fixture = TestBed.createComponent(AddUpstreamTeamDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
