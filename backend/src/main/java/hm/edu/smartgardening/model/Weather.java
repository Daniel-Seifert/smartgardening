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
    private Long sunset;
    private Long sunrise;
    private Float temp;
    private Float rain;
    private Float clouds;

}
