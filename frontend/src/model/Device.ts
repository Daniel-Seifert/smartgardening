import { Config } from "./Config";
import { Measurement } from "./Measurement";
import { Status } from "./Status";

export interface Device {
  id: string;
  name: string;
  activated: boolean;
  createDate: Date;
  updateDate: Date;
  config?: Config;
  status?: Status;
  measurements?: Measurement[];
}
