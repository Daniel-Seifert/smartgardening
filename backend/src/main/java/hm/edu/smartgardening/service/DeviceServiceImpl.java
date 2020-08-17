package hm.edu.smartgardening.service;

import hm.edu.smartgardening.exceptions.ResourceNotFoundException;
import hm.edu.smartgardening.model.*;
import hm.edu.smartgardening.repository.DeviceRepository;
import hm.edu.smartgardening.repository.MeasurementRepository;
import hm.edu.smartgardening.repository.WeatherRepository;
import org.apache.commons.lang3.time.DateUtils;
import org.springframework.stereotype.Service;

import java.util.Date;
import java.util.List;
import java.util.UUID;
import java.util.stream.Stream;

@Service
public class DeviceServiceImpl implements DeviceService {

    private final DeviceRepository deviceRepository;
    private final MeasurementRepository measurementRepository;
    private final WeatherRepository weatherRepository;

    public DeviceServiceImpl(DeviceRepository deviceRepository, MeasurementRepository measurementRepository, WeatherRepository weatherRepository) {
        this.deviceRepository = deviceRepository;
        this.measurementRepository = measurementRepository;
        this.weatherRepository = weatherRepository;
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
        final Device device = this.getByUuidOrThrow(uuid);
        // Load raw measurements
        final List<Measurement> measurements = since == null ? device.getMeasurements() :
                measurementRepository.getMeasurementsByUuidSince(uuid, since);
        // Add weather
        if (since != null && device.getConfig().isOutdoor()) {
            final List<Weather> weather = weatherRepository.getWeatherByZipCodeSince(device.getConfig().getZipCode(), since, DateUtils.addDays(new Date(), 3));
            // Add rain
            for (Weather w : weather) {
                measurements.add(new Measurement(0L, MeasureType.RAIN, w.getId().getDay(), w.getRain(), device));
            }
            // Add clouds
            for (Weather w : weather) {
                measurements.add(new Measurement(0L, MeasureType.CLOUDS, w.getId().getDay(), w.getClouds(), device));
            }
            // Add temp day
            for (Weather w : weather) {
                measurements.add(new Measurement(0L, MeasureType.TEMP_DAY, w.getId().getDay(), w.getTempDay() - 273.15f, device));
            }
        }
        return measurements;
    }

}
