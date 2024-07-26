
export class Idea {

  _id: string;
  modifyDate?: Date;
  text: string;
  stream?: string;
  date?: Date;
  user?: string;

   // computed
  itemType?: string;
  needsSend?: boolean;
}
