package hm.edu.smartgardening.repository;

import hm.edu.smartgardening.model.Weather;
import hm.edu.smartgardening.model.WeatherId;
import org.springframework.data.jpa.repository.JpaRepository;

public interface WeatherRepository extends JpaRepository<Weather, WeatherId> {
}
