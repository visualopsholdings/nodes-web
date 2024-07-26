import { TestBed } from '@angular/core/testing';

import { StreamOpenService } from './stream-open.service';

describe('StreamOpenService', () => {
  let service: StreamOpenService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(StreamOpenService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
