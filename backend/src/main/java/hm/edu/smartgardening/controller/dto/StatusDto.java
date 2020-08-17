package hm.edu.smartgardening.controller.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.Date;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class StatusDto {
    private Long id;
    private boolean pumping;
    private float humidity;
    private Boolean rain;
    private Date lastWatering;
}
