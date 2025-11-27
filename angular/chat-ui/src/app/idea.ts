
export class Idea {

  _id: string;
  modifyDate?: Date;
  text: string;
  stream?: string;
  date?: Date;
  user?: string;
  image?: string;

   // computed
  itemType?: string;
  needsSend?: boolean;
}
