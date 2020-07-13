package hm.edu.smartgardening.controller;

import hm.edu.smartgardening.controller.dto.ConfigDto;
import hm.edu.smartgardening.controller.dto.DeviceDto;
import hm.edu.smartgardening.exceptions.ResourceNotFoundException;
import hm.edu.smartgardening.model.Device;
import hm.edu.smartgardening.repository.DeviceRepository;
import org.modelmapper.ModelMapper;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.UUID;
import java.util.stream.Collectors;

@RestController
@RequestMapping("/api/devices")
@CrossOrigin
public class DeviceApiController {

    private final DeviceRepository deviceRepository;
    private final ModelMapper mapper;

    public DeviceApiController(DeviceRepository deviceRepository, ModelMapper mapper) {
        this.deviceRepository = deviceRepository;
        this.mapper = mapper;
    }

    @GetMapping()
    public List<DeviceDto> getAllDevices() {
        return deviceRepository.findAll()
                .stream()
                .map(device -> mapper.map(device, DeviceDto.class))
                .collect(Collectors.toList());
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
