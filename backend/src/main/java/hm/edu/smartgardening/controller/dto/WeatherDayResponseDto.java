package hm.edu.smartgardening.controller.dto;

import com.fasterxml.jackson.annotation.JsonProperty;
import hm.edu.smartgardening.model.Weather;
import hm.edu.smartgardening.model.WeatherId;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.Date;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class WeatherDayResponseDto {

    private long dt;
    private Main main;
    private Rain rain;
    private Clouds clouds;


    @Data
    @AllArgsConstructor
    @NoArgsConstructor
    public static class Main {
        private float temp;
        private float temp_min;
        private float temp_max;
    }

    @Data
    @AllArgsConstructor
    @NoArgsConstructor
    public static class Clouds {
        private int all;
    }

    @Data
    @AllArgsConstructor
    @NoArgsConstructor
    public static class Rain {
        @JsonProperty("3h")
        private float value;
    }

    public Weather toWeather(String zipCode) {
        int cloudValue = clouds != null ? clouds.all : 0;
        float rainValue = rain != null ? rain.value : 0f;

        return new Weather(
                new WeatherId(zipCode, new Date(dt * 1000)),
                main.temp_min,
                main.temp,
                main.temp_max,
                rainValue,
                cloudValue
        );
    }
}

