package hm.edu.smartgardening.model;

import lombok.*;

import javax.persistence.*;


@Entity
@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
@EqualsAndHashCode
public class Config {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @EqualsAndHashCode.Exclude
    private Long id;

    // Flags
    private boolean activated;

    // Weather
    private boolean outdoor;
    @Column(length = 10)
    private String zipCode;

    // Watering
    private float minHumidity;
    private float maxHumidity;
    private int minWateringSeconds;
    private int maxWateringSeconds;

}
