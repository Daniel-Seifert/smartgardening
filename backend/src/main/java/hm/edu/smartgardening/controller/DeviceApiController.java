package hm.edu.smartgardening.controller;

import hm.edu.smartgardening.controller.dto.ConfigDto;
import hm.edu.smartgardening.controller.dto.DeviceDto;
import hm.edu.smartgardening.model.Config;
import hm.edu.smartgardening.model.Device;
import hm.edu.smartgardening.service.DeviceService;
import org.modelmapper.ModelMapper;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.UUID;
import java.util.stream.Collectors;

@RestController
@RequestMapping("/api/devices")
public class DeviceApiController {

    private final DeviceService devices;
    private final ModelMapper mapper;

    public DeviceApiController(DeviceService devices, ModelMapper mapper) {
        this.devices = devices;
        this.mapper = mapper;
    }

    @GetMapping()
    public List<DeviceDto> getAllDevices() {
        return devices.getAll()
                .map(device -> mapper.map(device, DeviceDto.class))
                .collect(Collectors.toList());
    }

    @GetMapping("{uuid}/config")
    public ConfigDto getDeviceConfig(@PathVariable UUID uuid) {
        final Device device = devices.getByUuidOrThrow(uuid);
        return mapper.map(device.getConfig(), ConfigDto.class);
    }

    @PutMapping("{uuid}/config")
    public ConfigDto updateDeviceConfig(@PathVariable UUID uuid, @RequestBody ConfigDto updateConfig) {
        final Device device = devices.getByUuidOrThrow(uuid);
        device.setConfig(mapper.map(updateConfig, Config.class));
        return mapper.map(device.getConfig(), ConfigDto.class);
    }

    @DeleteMapping("{uuid}")
    public DeviceDto deleteDevice(@PathVariable UUID uuid) {
        final Device deleted = devices.deleteByUuidOrThrow(uuid);
        return mapper.map(deleted, DeviceDto.class);
    }

}
