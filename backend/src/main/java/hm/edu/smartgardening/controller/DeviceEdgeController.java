package hm.edu.smartgardening.controller;

import hm.edu.smartgardening.controller.dto.*;
import hm.edu.smartgardening.model.Device;
import hm.edu.smartgardening.model.Measurement;
import hm.edu.smartgardening.model.Status;
import hm.edu.smartgardening.service.DeviceService;
import org.modelmapper.ModelMapper;
import org.springframework.web.bind.annotation.*;

import java.util.Date;
import java.util.UUID;


@RestController
@CrossOrigin
@RequestMapping("/edge/devices")
public class DeviceEdgeController {

    private final DeviceService devices;
    private final ModelMapper mapper;

    public DeviceEdgeController(DeviceService devices, ModelMapper mapper) {
        this.devices = devices;
        this.mapper = mapper;
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

    @PostMapping("{uuid}/measures")
    public AddMeasurementDto addMeasurement(@PathVariable UUID uuid, @RequestBody AddMeasurementDto measurement) {
        final Device device = devices.getByUuidOrThrow(uuid);
        final Measurement newMeasurement = mapper.map(measurement, Measurement.class);
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
