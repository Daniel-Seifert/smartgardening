import { Config } from "./Config";
import { Measurement } from "./Measurement";

export interface Device {
  id: string;
  name: string;
  activated: boolean;
  createDate: Date,
  updateDate: Date,
  config?: Config;
  measurement?: Measurement[]; 
}
