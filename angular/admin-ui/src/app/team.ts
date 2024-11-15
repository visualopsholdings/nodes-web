
import { TeamMember } from './team-member';

export class Team {

  _id: number;
  modifyDate?: string;
  name: string;
  image: string;
  policy: string;
  upstream: boolean;
  deleted: boolean;
  members: TeamMember[];

}
