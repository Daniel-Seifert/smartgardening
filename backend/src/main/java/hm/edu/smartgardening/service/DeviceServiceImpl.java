package hm.edu.smartgardening.service;

import hm.edu.smartgardening.exceptions.ResourceNotFoundException;
import hm.edu.smartgardening.model.Device;
import hm.edu.smartgardening.repository.DeviceRepository;
import org.springframework.stereotype.Service;

import java.util.UUID;
import java.util.stream.Stream;

@Service
public class DeviceServiceImpl implements DeviceService {

    private final DeviceRepository deviceRepository;

    public DeviceServiceImpl(DeviceRepository deviceRepository) {
        this.deviceRepository = deviceRepository;
    }

    @Override
    public Stream<Device> getAll() {
        return deviceRepository.findAll().stream();
    }

    @Override
    public Device getByUuidOrThrow(UUID uuid) {
        return deviceRepository.findById(uuid).orElseThrow(ResourceNotFoundException::new);
    }

    @Override
    public Device deleteByUuidOrThrow(UUID uuid) {
        final Device device = this.getByUuidOrThrow(uuid);
        deviceRepository.delete(device);
        return device;
    }
}
