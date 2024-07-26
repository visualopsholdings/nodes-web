import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EditIdeaDialogComponent } from './edit-idea-dialog.component';

describe('EditIdeaDialogComponent', () => {
  let component: EditIdeaDialogComponent;
  let fixture: ComponentFixture<EditIdeaDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [EditIdeaDialogComponent]
    });
    fixture = TestBed.createComponent(EditIdeaDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
