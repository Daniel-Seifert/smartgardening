package hm.edu.smartgardening.controller.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class DailyForecastResponseDto {

    private String cod;
    private int message;
    private int cnt;
    private List<WeatherDayResponseDto> list;

}
