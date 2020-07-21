package hm.edu.smartgardening.service;

import hm.edu.smartgardening.model.Device;
import hm.edu.smartgardening.model.Measurement;

import java.util.Date;
import java.util.List;
import java.util.UUID;
import java.util.stream.Stream;

public interface DeviceService {
    Stream<Device> getAll();

    Device createAndSaveDevice();

    Device getByUuidOrThrow(UUID uuid);

    Device deleteByUuidOrThrow(UUID uuid);

    Device updateDeviceOrThrow(Device device);

    List<Measurement> getMeasurementsByUuidSince(UUID uuid, Date since);
}
