
import { StreamBits } from './streambits';

export class StreamFlags {

  constructor(bits: StreamBits) {
    this.update(bits);
  }

  update(bits: StreamBits): void {
  }

  bits(): StreamBits {
    var bits = StreamBits.none;
    return bits;
  }

}
