import { Injectable } from '@angular/core';
import { Subject }    from 'rxjs';

import { StreamOpen }    from './stream-open';

@Injectable({
  providedIn: 'root'
})
export class StreamOpenService {

  private openChangedSource = new Subject<StreamOpen>();
  openChanged$ = this.openChangedSource.asObservable();

  constructor() { }

  refresh(clear: boolean) {
    this.openChangedSource.next({ refresh: true, clear: clear } as StreamOpen);
  }

  select(stream: string) {
    this.openChangedSource.next({ open: true, streams: [stream], select: true } as StreamOpen);
  }

  open(streams: string[]) {
    this.openChangedSource.next({ open: true, streams: streams } as StreamOpen);
  }

  close() {
    this.openChangedSource.next({ open: false } as StreamOpen);
  }

}
