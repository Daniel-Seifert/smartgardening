package hm.edu.smartgardening.controller.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class WeatherOutDto {

    private Float tempDay;
    private Float tempMin;
    private Float tempMax;
    private Float rain;
    private Float clouds;

}
