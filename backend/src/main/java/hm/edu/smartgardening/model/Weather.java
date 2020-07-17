package hm.edu.smartgardening.model;

import lombok.*;
import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.annotation.LastModifiedDate;

import javax.persistence.EmbeddedId;
import javax.persistence.Entity;
import java.util.Date;

@Entity
@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
@EqualsAndHashCode
public class Weather {

    public Weather(WeatherId id, Float tempMin, Float tempDay, Float tempMax, Float rain, Integer clouds) {
        this.id = id;
        this.tempMin = tempMin;
        this.tempDay = tempDay;
        this.tempMax = tempMax;
        this.rain = rain;
        this.clouds = clouds;
    }

    @EmbeddedId
    private WeatherId id;
    private Float tempMin;
    private Float tempDay;
    private Float tempMax;
    private Float rain;
    private Integer clouds;

    @CreatedDate
    private Date createDate;
    @LastModifiedDate
    private Date updateDate;

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
