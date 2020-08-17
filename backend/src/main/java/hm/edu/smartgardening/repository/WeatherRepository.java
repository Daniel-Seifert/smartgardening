package hm.edu.smartgardening.repository;

import hm.edu.smartgardening.model.Weather;
import hm.edu.smartgardening.model.WeatherId;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;

import java.util.Date;
import java.util.List;

public interface WeatherRepository extends JpaRepository<Weather, WeatherId> {

    @Query("SELECT w FROM Weather w WHERE w.id.zipCode = :zipCode AND w.id.day >= :since AND w.id.day <= :till")
    List<Weather> getWeatherByZipCodeSince (@Param("zipCode") String zipCode, @Param("since") Date since, @Param("till") Date till);
}
