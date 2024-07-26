import { Injectable } from '@angular/core';

@Injectable()
export class IconService {

  constructor() { }

  getIconByString(icon: string): string {

    if (icon == "internal:org") {
      return "assets/org-button.svg";
    }
    else if (icon == "internal:project") {
      return "assets/project-button.svg";
    }
    else if (icon == "internal:stream") {
      return "assets/stream-button.svg";
    }
    else if (icon == "internal:queue") {
      return "assets/queue-button.svg";
    }
    else if (icon == "internal:group") {
      return "assets/group-icon.svg";
    }
    else if (icon == "internal:media") {
      return "assets/media-button.svg";
    }
    else if (icon == "internal:sequence") {
      return "assets/sequence-button.svg";
    }
    else if (icon == "internal:user") {
      return "assets/user-icon.svg";
    }
    else if (icon == "internal:task") {
      return "assets/task-button.svg";
    }
    else if (icon == "internal:conversation") {
      return "assets/message-button.svg";
    }
    else if (icon == "internal:site") {
      return "assets/site-button.svg";
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
