package hm.edu.smartgardening.controller.dto;

import hm.edu.smartgardening.model.MeasureType;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.Date;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class MeasurementDto {

    private Long id;
    private MeasureType measureType;
    private Date createDate;
    private float value;

}
