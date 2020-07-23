package hm.edu.smartgardening.service;

import hm.edu.smartgardening.exceptions.ResourceNotFoundException;
import hm.edu.smartgardening.model.Config;
import hm.edu.smartgardening.model.Device;
import hm.edu.smartgardening.model.Measurement;
import hm.edu.smartgardening.repository.DeviceRepository;
import hm.edu.smartgardening.repository.MeasurementRepository;
import org.springframework.stereotype.Service;

import java.util.Date;
import java.util.List;
import java.util.UUID;
import java.util.stream.Stream;

@Service
public class DeviceServiceImpl implements DeviceService {

    private final DeviceRepository deviceRepository;
    private final MeasurementRepository measurementRepository;

    public DeviceServiceImpl(DeviceRepository deviceRepository, MeasurementRepository measurementRepository) {
        this.deviceRepository = deviceRepository;
        this.measurementRepository = measurementRepository;
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
    public Device createAndSaveDevice() {
        final UUID uuid = UUID.randomUUID();
        final Config configuration = new Config();
        final Device newDevice = new Device();

        newDevice.setId(uuid);
        newDevice.setConfig(configuration);

        deviceRepository.save(newDevice);
        return newDevice;
    }

    @Override
    public Device deleteByUuidOrThrow(UUID uuid) {
        final Device device = this.getByUuidOrThrow(uuid);
        deviceRepository.delete(device);
        return device;
    }

    @Override
    public Device updateDeviceOrThrow(Device device) {
        if (deviceRepository.existsById(device.getId()))
            deviceRepository.save(device);
        else
            throw new ResourceNotFoundException();
        return device;
    }

    @Override
    public List<Measurement> getMeasurementsByUuidSince(UUID uuid, Date since) {
        return since == null ? this.getByUuidOrThrow(uuid).getMeasurements() :
                measurementRepository.getMeasurementsByUuidSince(uuid, since);
    }
}
