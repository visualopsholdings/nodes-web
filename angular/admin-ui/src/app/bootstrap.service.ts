import { Injectable } from '@angular/core';
import { Subject }    from 'rxjs';

import { Me }  from './me';
import { MeService } from './me.service';

@Injectable({
  providedIn: 'root'
})
export class BootstrapService {

  private meChangedSource = new Subject<[Me]>();
  meChanged$ = this.meChangedSource.asObservable();

  me: Me;

  constructor(
    private meService: MeService,
  ) {
    this.meService.getMe().subscribe(me => {

      let url = new URL(window.location.href);
      this.me = me;
      this.meChangedSource.next([this.me]);
    });
  }

  getMe(): Me {
    return this.me;
  }

}
