package hm.edu.smartgardening.controller;

import hm.edu.smartgardening.controller.dto.ConfigDto;
import hm.edu.smartgardening.exceptions.ResourceNotFoundException;
import hm.edu.smartgardening.model.Device;
import hm.edu.smartgardening.repository.DeviceRepository;
import org.modelmapper.ModelMapper;
import org.springframework.web.bind.annotation.*;

import java.util.UUID;


@RestController
@RequestMapping("/edge/devices")
@CrossOrigin
public class DeviceEdgeController {

    private final DeviceRepository deviceRepository;
    private final ModelMapper mapper;

    public DeviceEdgeController(DeviceRepository deviceRepository, ModelMapper mapper) {
        this.deviceRepository = deviceRepository;
        this.mapper = mapper;
    }

    @GetMapping("{uuid}/config")
    public ConfigDto getDeviceConfig(@PathVariable UUID uuid) {
        final Device device = getDeviceByIdOrThrow(uuid);
        return mapper.map(device.getConfig(), ConfigDto.class);
    }

    private Device getDeviceByIdOrThrow(UUID uuid) {
        return deviceRepository.findById(uuid).orElseThrow(ResourceNotFoundException::new);
    }

}
