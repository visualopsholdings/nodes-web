import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SecurityErrorComponent } from './security-error.component';

describe('SecurityErrorComponent', () => {
  let component: SecurityErrorComponent;
  let fixture: ComponentFixture<SecurityErrorComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [SecurityErrorComponent]
    });
    fixture = TestBed.createComponent(SecurityErrorComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
