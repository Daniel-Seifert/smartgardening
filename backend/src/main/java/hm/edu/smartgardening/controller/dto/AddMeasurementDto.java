package hm.edu.smartgardening.controller.dto;

import hm.edu.smartgardening.model.MeasureType;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.Date;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class AddMeasurementDto {
    private float value;
    private MeasureType measureType;
}
