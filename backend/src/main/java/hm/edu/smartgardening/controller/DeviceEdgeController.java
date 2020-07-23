package hm.edu.smartgardening.controller;

import hm.edu.smartgardening.controller.dto.*;
import hm.edu.smartgardening.exceptions.ResourceNotFoundException;
import hm.edu.smartgardening.model.*;
import hm.edu.smartgardening.repository.WeatherRepository;
import hm.edu.smartgardening.service.DeviceService;
import org.apache.commons.lang3.time.DateUtils;
import org.modelmapper.ModelMapper;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.client.RestTemplate;

import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Collectors;

import static hm.edu.smartgardening.controller.WeatherController.RELEVANT_RAINFALL;


@RestController
@CrossOrigin
@RequestMapping("/edge/devices")
public class DeviceEdgeController {

    private final DeviceService devices;
    private final ModelMapper mapper;
    private final WeatherRepository weatherRepository;
    private final RestTemplate rest;

    @Value("${app.config.country-code}")
    private String countryCode;
    @Value("${app.config.api-key}")
    private String apiKey;

    public DeviceEdgeController(WeatherRepository repo, DeviceService devices, ModelMapper mapper, RestTemplate template) {
        this.weatherRepository = repo;
        this.devices = devices;
        this.mapper = mapper;
        this.rest = template;
    }

    @PostMapping("register")
    public DeviceBriefDto registerDevice() {
        final Device newDevice = devices.createAndSaveDevice();
        return mapper.map(newDevice, DeviceBriefDto.class);
    }

    @GetMapping("{uuid}/config")
    public ConfigDto getDeviceConfig(@PathVariable UUID uuid) {
        final Device device = devices.getByUuidOrThrow(uuid);
        final ConfigDto config = mapper.map(device.getConfig(), ConfigDto.class);
        config.setActivated(device.isActivated());
        return config;
    }

    @GetMapping("{uuid}/weather")
    public RainDto rainToday(@PathVariable UUID uuid) {
        final Device device = devices.getByUuidOrThrow(uuid);
        final String zipCode = device.getConfig().getZipCode();
        final Date today = new Date();
        final WeatherId searchId = new WeatherId(zipCode, today);
        final Optional<Weather> match = weatherRepository.findById(searchId);
        if (match.isPresent()) {
            return mapper.map(match.get().getRain() > RELEVANT_RAINFALL, RainDto.class);
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
                        .filter(forecast -> !weatherRepository.existsById(forecast.getId()))
                        .collect(Collectors.groupingBy(weather -> DateUtils.truncate(weather.getId().getDay(), Calendar.DAY_OF_MONTH)));

                weatherByDay.forEach((key, value) -> {
                    final AtomicInteger counter = new AtomicInteger(1);
                    final Optional<Weather> combined = value.stream().reduce((a, b) -> {
                        counter.incrementAndGet();
                        a.getId().setDay(key);
                        return a.add(b);
                    });
                    combined.ifPresent(weather -> weatherRepository.save(weather.calcAverage(counter.get())));
                });
            }
        }
        return mapper.map(weatherRepository.findById(searchId).orElseThrow(ResourceNotFoundException::new).getRain() > RELEVANT_RAINFALL, RainDto.class);
    }

    @PostMapping("{uuid}/measures")
    public AddMeasurementDto addMeasurement(@PathVariable UUID uuid, @RequestBody AddMeasurementDto measurement) {
        final Device device = devices.getByUuidOrThrow(uuid);
        final Measurement newMeasurement = mapper.map(measurement, Measurement.class);
        newMeasurement.setDevice(device);
        device.addMeasurement(newMeasurement);
        device.getStatus().setHumidity(newMeasurement.getValue());
        devices.updateDeviceOrThrow(device);
        return mapper.map(newMeasurement, AddMeasurementDto.class);
    }

    @PutMapping("{uuid}/status")
    public StatusBriefDto updateDeviceStatus(@PathVariable UUID uuid, @RequestBody StatusBriefDto updateStatus) {
        final Device device = devices.getByUuidOrThrow(uuid);

        final Status newStatus = mapper.map(updateStatus, Status.class);
        newStatus.setHumidity(device.getStatus().getHumidity());
        newStatus.setId(device.getStatus().getId());
        if(device.getStatus().isPumping() && !newStatus.isPumping())
            // If the device stops watering, save the date and time
            newStatus.setLastWatering(new Date());
        else
            newStatus.setLastWatering(device.getStatus().getLastWatering());

        device.setStatus(newStatus);
        devices.updateDeviceOrThrow(device);
        return mapper.map(device.getStatus(), StatusBriefDto.class);
    }

}
