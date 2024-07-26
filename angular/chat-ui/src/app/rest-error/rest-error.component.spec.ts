import { ComponentFixture, TestBed } from '@angular/core/testing';

import { RestErrorComponent } from './rest-error.component';

describe('RestErrorComponent', () => {
  let component: RestErrorComponent;
  let fixture: ComponentFixture<RestErrorComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [RestErrorComponent]
    });
    fixture = TestBed.createComponent(RestErrorComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
