import { TestBed } from '@angular/core/testing';

import { NewIdeaService } from './new-idea.service';

describe('NewIdeaService', () => {
  let service: NewIdeaService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(NewIdeaService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
