import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SiteDetailComponent } from './site-detail.component';

describe('SiteDetailComponent', () => {
  let component: SiteDetailComponent;
  let fixture: ComponentFixture<SiteDetailComponent>;

  beforeEach(() => {
    TestBed.configureTestingModule({
      declarations: [SiteDetailComponent]
    });
    fixture = TestBed.createComponent(SiteDetailComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
