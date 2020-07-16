package hm.edu.smartgardening.controller.dto;

import com.fasterxml.jackson.annotation.JsonProperty;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class ConfigDto {

    private Long id;
    private boolean outdoor;
    private float minHumidity;
    private float maxHumidity;
    private int minWateringSeconds;
    private int maxWateringSeconds;
    private boolean activated;
    private String zipCode;

}
