package hm.edu.smartgardening.controller.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.UUID;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class DeviceDto {

    private UUID id;
    private String name;
    private Date createDate;
    private Date updateDate;
    private ConfigDto config;
    private List<MeasurementDto> measurements = new ArrayList<>();

}
