export interface Config {
  id: number,
  outdoor: boolean;
  zipCode?: string;
  minHumidity: number;
  maxHumidity: number;
  minWateringSeconds: number;
  maxWateringSeconds: number;
  activated: boolean;
}
