package hm.edu.smartgardening.controller;

import hm.edu.smartgardening.controller.dto.*;
import hm.edu.smartgardening.model.Config;
import hm.edu.smartgardening.model.Device;
import hm.edu.smartgardening.service.DeviceService;
import org.modelmapper.ModelMapper;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.UUID;
import java.util.logging.Logger;
import java.util.stream.Collectors;

@RestController
@CrossOrigin
@RequestMapping("/api/devices")
public class DeviceApiController {

    private final DeviceService devices;
    private final ModelMapper mapper;

    public DeviceApiController(DeviceService devices, ModelMapper mapper) {
        this.devices = devices;
        this.mapper = mapper;
    }


    @GetMapping()
    public List<DeviceBriefDto> getAllDevices() {
        return devices.getAll()
                .map(device -> mapper.map(device, DeviceBriefDto.class))
                .collect(Collectors.toList());
    }

    @GetMapping("{uuid}/config")
    public ConfigDto getDeviceConfig(@PathVariable UUID uuid) {
        final Device device = devices.getByUuidOrThrow(uuid);
        final ConfigDto config = mapper.map(device.getConfig(), ConfigDto.class);
        config.setActivated(device.isActivated());
        return config;
    }

    @GetMapping("{uuid}/measurements")
    public List<GetMeasurementDto> getAllMeasurements(@PathVariable UUID uuid) {
        final Device device = devices.getByUuidOrThrow(uuid);
        return device.getMeasurements()
                .stream()
                .map(it -> mapper.map(it, GetMeasurementDto.class))
                .collect(Collectors.toList());
    }

    @GetMapping("{uuid}/status")
    public StatusDto getDeviceStatus(@PathVariable UUID uuid) {
        final Device device = devices.getByUuidOrThrow(uuid);
        return mapper.map(device.getStatus(), StatusDto.class);
    }

    @PutMapping("{uuid}/config")
    public ConfigDto updateDeviceConfig(@PathVariable UUID uuid, @RequestBody ConfigDto updateConfig) {
        final Device device = devices.getByUuidOrThrow(uuid);
        device.setConfig(mapper.map(updateConfig, Config.class));
        device.setActivated(updateConfig.isActivated());
        devices.updateDeviceOrThrow(device);
        final ConfigDto newConfig = mapper.map(device.getConfig(), ConfigDto.class);
        newConfig.setActivated(device.isActivated());
        return newConfig;
    }

    @PatchMapping("{uuid}")
    public DevicePatchDto updateDevice(@PathVariable UUID uuid, @RequestBody DevicePatchDto devicePatch) {
        final Device device = devices.getByUuidOrThrow(uuid);
        device.setName(devicePatch.getName());
        device.setActivated(devicePatch.isActivated());
        devices.updateDeviceOrThrow(device);
        return mapper.map(device, DevicePatchDto.class);
    }

    @DeleteMapping("{uuid}")
    public DeviceDto deleteDevice(@PathVariable UUID uuid) {
        final Device deleted = devices.deleteByUuidOrThrow(uuid);
        return mapper.map(deleted, DeviceDto.class);
    }

}
