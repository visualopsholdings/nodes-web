import { Pipe, PipeTransform } from '@angular/core';
import { orderBy } from 'lodash';

@Pipe({
  name: 'sortStream'
})
export class SortStreamPipe implements PipeTransform {

  transform(value: any[], args?: any): any[] {
    if (args && args.name == "public") {
      return orderBy(value, ["name"], ["asc"]);
    }
    return orderBy(value, ["unread", "name"], ["desc", "asc"]);
  }

}
