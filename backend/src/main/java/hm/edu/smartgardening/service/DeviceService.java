package hm.edu.smartgardening.service;

import hm.edu.smartgardening.model.Device;

import java.util.UUID;
import java.util.stream.Stream;

public interface DeviceService {
    Stream<Device> getAll();

    Device createAndSaveDevice();

    Device getByUuidOrThrow(UUID uuid);

    Device deleteByUuidOrThrow(UUID uuid);

    Device updateDeviceOrThrow(Device device);
}
