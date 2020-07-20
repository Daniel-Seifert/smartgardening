package hm.edu.smartgardening.controller;

import hm.edu.smartgardening.controller.dto.DailyForecastResponseDto;
import hm.edu.smartgardening.controller.dto.WeatherOutDto;
import hm.edu.smartgardening.exceptions.ResourceNotFoundException;
import hm.edu.smartgardening.model.Weather;
import hm.edu.smartgardening.model.WeatherId;
import hm.edu.smartgardening.repository.WeatherRepository;
import org.apache.commons.lang3.time.DateUtils;
import org.modelmapper.ModelMapper;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.client.RestTemplate;

import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Collectors;


@RestController
@RequestMapping("/edge/weather")
public class WeatherController {

    private final WeatherRepository repo;
    private final ModelMapper mapper;
    private final RestTemplate rest;

    @Value("${app.config.country-code}")
    private String countryCode;
    @Value("${app.config.api-key}")
    private String apiKey;

    public WeatherController(WeatherRepository repo, ModelMapper mapper, RestTemplate template) {
        this.repo = repo;
        this.mapper = mapper;
        this.rest = template;
    }

    @GetMapping("{zipCode}")
    public WeatherOutDto getDeviceConfig(@PathVariable String zipCode, @RequestParam @DateTimeFormat(pattern = "dd.MM.yyyy") Date day) {
        final WeatherId searchId = new WeatherId(zipCode, day);
        final Optional<Weather> match = repo.findById(searchId);
        if (match.isPresent()) {
            return mapper.map(match.get(), WeatherOutDto.class);
        } else {
            DailyForecastResponseDto response = rest
                    .getForObject("http://api.openweathermap.org/data/2.5/forecast?zip={zipCode},{countryCode}&appid={apiKey}",
                            DailyForecastResponseDto.class,
                            zipCode,
                            countryCode,
                            apiKey
                    );

            if (response != null && response.getList() != null) {
                final Map<Date, List<Weather>> weatherByDay = response.getList().stream()
                        .map(next -> next.toWeather(zipCode))
                        .filter(forecast -> !repo.existsById(forecast.getId()))
                        .collect(Collectors.groupingBy(weather -> DateUtils.truncate(weather.getId().getDay(), Calendar.DAY_OF_MONTH)));

                weatherByDay.forEach((key, value) -> {
                    final AtomicInteger counter = new AtomicInteger(1);
                    final Optional<Weather> combined = value.stream().reduce((a, b) -> {
                        counter.incrementAndGet();
                        a.getId().setDay(key);
                        return a.add(b);
                    });
                    combined.ifPresent(weather -> repo.save(weather.calcAverage(counter.get())));
                });
            }
        }

        return mapper.map(repo.findById(searchId).orElseThrow(ResourceNotFoundException::new), WeatherOutDto.class);
    }

}
