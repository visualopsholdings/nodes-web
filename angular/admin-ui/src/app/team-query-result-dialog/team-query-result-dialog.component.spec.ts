import { ComponentFixture, TestBed } from '@angular/core/testing';

import { TeamQueryResultDialogComponent } from './team-query-result-dialog.component';

describe('TeamQueryResultDialogComponent', () => {
  let component: TeamQueryResultDialogComponent;
  let fixture: ComponentFixture<TeamQueryResultDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [TeamQueryResultDialogComponent]
    });
    fixture = TestBed.createComponent(TeamQueryResultDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
