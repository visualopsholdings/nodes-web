
import { StreamBits } from './streambits';

export class StreamFlags {

  shareWithNewUsers: boolean;

  constructor(bits: StreamBits) {
    this.update(bits);
  }

  update(bits: StreamBits): void {
    this.shareWithNewUsers = (bits & StreamBits.shareWithNewUsers) != 0;
  }

  bits(): StreamBits {
    var bits = StreamBits.none;
    bits |= this.shareWithNewUsers ? StreamBits.shareWithNewUsers : 0;
    return bits;
  }

}
