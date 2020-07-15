export interface Config {
  outdoor: boolean;
  zipCode?: string;
  minHumidity: number;
  maxHumidity: number;
  minWateringSeconds: number;
  maxWateringSeconds: number;
}
