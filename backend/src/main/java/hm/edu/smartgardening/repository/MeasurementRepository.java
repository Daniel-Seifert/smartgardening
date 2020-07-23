package hm.edu.smartgardening.repository;

import hm.edu.smartgardening.model.Measurement;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;

import java.util.Date;
import java.util.List;
import java.util.UUID;

public interface MeasurementRepository extends JpaRepository<Measurement, Long> {

    @Query("SELECT m FROM Device d INNER JOIN Measurement m ON d.id = m.device.id WHERE d.id = :uuid AND m.createDate >= :since")
   List<Measurement> getMeasurementsByUuidSince (@Param("uuid")UUID uuid, @Param("since") Date since);

}
