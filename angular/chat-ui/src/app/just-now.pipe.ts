import { Pipe, PipeTransform } from '@angular/core';
import { DatePipe } from '@angular/common';

import * as moment from 'moment';

@Pipe({
  name: 'justNow'
})
export class JustNowPipe extends DatePipe implements PipeTransform {

  override transform(value: any, args?: any): any {
    return moment(value).calendar(null, {
        lastDay : '[Yesterday at] LT',
        sameDay : 'LT',
        nextDay : '[Tomorrow at] LT',
        lastWeek: 'Do MMM YYYY',
        sameElse : 'Do MMM YYYY'
    });
  }

}

