package hm.edu.smartgardening.model;

import lombok.*;

import javax.persistence.EmbeddedId;
import javax.persistence.Entity;

@Entity
@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
@EqualsAndHashCode
public class Weather {

    @EmbeddedId
    private WeatherId id;
    private Float tempMin;
    private Float tempDay;
    private Float tempMax;
    private Float rain;
    private Integer clouds;

    public Weather add(Weather other) {
        return new Weather(
                id,
                tempMin + other.tempMin,
                tempDay + other.tempDay,
                tempMax + other.tempMax,
                rain + other.rain,
                clouds + other.clouds
        );
    }

    public Weather calcAverage(int numOfAggregations) {
        return new Weather(
                id,
                tempMin / numOfAggregations,
                tempDay / numOfAggregations,
                tempMax / numOfAggregations,
                rain,
                clouds / numOfAggregations
        );
    }

}
