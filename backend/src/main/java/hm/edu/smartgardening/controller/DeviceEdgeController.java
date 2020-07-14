package hm.edu.smartgardening.controller;

import hm.edu.smartgardening.controller.dto.ConfigDto;
import hm.edu.smartgardening.model.Device;
import hm.edu.smartgardening.service.DeviceService;
import org.modelmapper.ModelMapper;
import org.springframework.web.bind.annotation.*;

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

    @GetMapping("{uuid}/config")
    public ConfigDto getDeviceConfig(@PathVariable UUID uuid) {
        final Device device = devices.getByUuidOrThrow(uuid);
        return mapper.map(device.getConfig(), ConfigDto.class);
    }

}
