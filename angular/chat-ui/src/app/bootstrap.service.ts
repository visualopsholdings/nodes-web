import { Injectable } from '@angular/core';
import { Subject }    from 'rxjs';

import { Me }  from './me';
import { Site }  from './site';
import { MeService } from './me.service';
import { SiteService }  from './site.service';

@Injectable({
  providedIn: 'root'
})
export class BootstrapService {

  private meChangedSource = new Subject<[Me, Site]>();
  meChanged$ = this.meChangedSource.asObservable();

  site: Site;
  me: Me;

  constructor(
    private meService: MeService,
    private siteService: SiteService,
  ) {
    this.meService.getMe().subscribe(me => {
      this.siteService.getSite().subscribe(site => {
        this.site = site;
        this.me = me;
        this.meChangedSource.next([this.me, this.site]);
      });
    });
  }

  getMe(): Me {
    return this.me;
  }

  getSite(): Site {
    return this.site;
  }

}
