package hm.edu.smartgardening.controller;

import hm.edu.smartgardening.controller.dto.*;
import hm.edu.smartgardening.exceptions.UnauthorizedException;
import hm.edu.smartgardening.model.Device;
import hm.edu.smartgardening.model.MeasureType;
import hm.edu.smartgardening.model.Measurement;
import hm.edu.smartgardening.model.Status;
import hm.edu.smartgardening.service.DeviceService;
import org.modelmapper.ModelMapper;
import org.springframework.web.bind.annotation.*;

import java.util.Date;
import java.util.UUID;

@RestController
@RequestMapping("/edge/devices")
public class DeviceEdgeController {

    /**
     * The amount of liters per square meter
     */
    public static final float RELEVANT_RAINFALL = 1.5f;

    private final DeviceService devices;
    private final ModelMapper mapper;
    private final WeatherController weatherController;

    public DeviceEdgeController(DeviceService devices, ModelMapper mapper, WeatherController weatherController) {
        this.devices = devices;
        this.mapper = mapper;
        this.weatherController = weatherController;
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
        if (!device.getConfig().isOutdoor()) {
            RainDto dto = new RainDto();
            dto.setRain(false);
            return dto;
        }

        final String zipCode = device.getConfig().getZipCode();
        final WeatherOutDto currWeather = weatherController.getWeather(zipCode, new Date());
        return new RainDto(currWeather.getRain() > RELEVANT_RAINFALL);
    }

    @PostMapping("{uuid}/measures")
    public AddMeasurementDto addMeasurement(@PathVariable UUID uuid, @RequestBody AddMeasurementDto measurement) {
        final Device device = devices.getByUuidOrThrow(uuid);
        if (!device.isActivated()) {
            throw new UnauthorizedException();
        }

        final Measurement newMeasurement = mapper.map(measurement, Measurement.class);
        newMeasurement.setDevice(device);
        device.addMeasurement(newMeasurement);
        device.getStatus().setHumidity(newMeasurement.getValue());
        devices.updateDeviceOrThrow(device);
        return mapper.map(newMeasurement, AddMeasurementDto.class);
    }

    @PutMapping("{uuid}/status")
    public StatusBriefDto updateDeviceStatus(@PathVariable UUID uuid, @RequestBody StatusBriefDto updateStatus) {
        Device device = devices.getByUuidOrThrow(uuid);
        if (!device.isActivated()) {
            throw new UnauthorizedException();
        }

        // Store pumping status as measurement
        final Measurement measurement = new Measurement(null, MeasureType.PUMPING, new Date(), updateStatus.isPumping() ? 1 : 0, device);
        device.addMeasurement(measurement);
        device = devices.updateDeviceOrThrow(device);

        // Update device status
        final Status newStatus = mapper.map(updateStatus, Status.class);
        newStatus.setHumidity(device.getStatus().getHumidity());
        newStatus.setId(device.getStatus().getId());
        if (device.getStatus().isPumping() && !newStatus.isPumping())
            // If the device stops watering, save the date and time
            newStatus.setLastWatering(new Date());
        else
            newStatus.setLastWatering(device.getStatus().getLastWatering());

        device.setStatus(newStatus);
        devices.updateDeviceOrThrow(device);
        return mapper.map(device.getStatus(), StatusBriefDto.class);
    }

}
