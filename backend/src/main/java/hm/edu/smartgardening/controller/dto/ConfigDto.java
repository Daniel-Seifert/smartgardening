package hm.edu.smartgardening.controller.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class ConfigDto {

    private boolean outdoor;
    private String zipCode;
    private float minHumidity;
    private float maxHumidity;
    private int minWateringSeconds;
    private int maxWateringSeconds;
    private boolean activated;

}
