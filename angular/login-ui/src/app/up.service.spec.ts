import { TestBed } from '@angular/core/testing';

import { UpService } from './up.service';

describe('UpService', () => {
  let service: UpService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(UpService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
