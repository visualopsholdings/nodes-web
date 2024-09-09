import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SetEmojiDialogComponent } from './set-emoji-dialog.component';

describe('SetEmojiDialogComponent', () => {
  let component: SetEmojiDialogComponent;
  let fixture: ComponentFixture<SetEmojiDialogComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [SetEmojiDialogComponent]
    });
    fixture = TestBed.createComponent(SetEmojiDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
