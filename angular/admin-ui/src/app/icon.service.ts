import { Injectable } from '@angular/core';

@Injectable()
export class IconService {

  constructor() { }

  getIconByString(icon: string): string {

    if (icon == "internal:stream") {
      return "assets/stream-button.svg";
    }
    else if (icon == "internal:group") {
      return "assets/group-icon.svg";
    }
    else if (icon == "internal:user") {
      return "assets/user-icon.svg";
    }
    else {
      // it's an url. Should always be absolute but it's not.
      return "/" + icon;
    }
  }

  getIcon(item: any): string {

      if (!item) {
        return "";
      }

  		if (item.typeIcon && !item.image) {
			  return "assets/" + item.typeIcon.substr(9) + ".svg";
  		}
      else if (item.image) {
				return "/rest/1.0/media/" + item.image + "/thumbnail";
      }
      else {
        return this.getIconByString(item.icon);
      }

    }

}
