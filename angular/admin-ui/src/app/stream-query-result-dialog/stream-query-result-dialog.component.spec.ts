import { ComponentFixture, TestBed } from '@angular/core/testing';

import { StreamQueryResultDialogComponent } from './stream-query-result-dialog.component';

describe('StreamQueryResultDialogComponent', () => {
  let component: StreamQueryResultDialogComponent;
  let fixture: ComponentFixture<StreamQueryResultDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [StreamQueryResultDialogComponent]
    });
    fixture = TestBed.createComponent(StreamQueryResultDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
