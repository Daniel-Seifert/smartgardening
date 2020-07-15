import { Config } from "./Config";

export interface Device {
  uuid: string;
  name: string;
  activated: boolean;
  config: Config;
}
