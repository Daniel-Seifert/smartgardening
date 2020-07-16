package hm.edu.smartgardening.controller.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class DevicePatchDto {
    private String name;
    private boolean activated;
}
