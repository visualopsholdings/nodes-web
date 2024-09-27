
import { TeamMember } from './team-member';

export class Team {

  _id: number;
  name: string;
  image: string;
  policy: string;
  deleted: boolean;
  members: TeamMember[];

}
