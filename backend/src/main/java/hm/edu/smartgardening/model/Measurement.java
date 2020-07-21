package hm.edu.smartgardening.model;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.NoArgsConstructor;
import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.jpa.domain.support.AuditingEntityListener;

import javax.persistence.*;
import java.util.Date;

@Entity
@Data
@AllArgsConstructor
@NoArgsConstructor
@EntityListeners(AuditingEntityListener.class)
@EqualsAndHashCode
public class Measurement {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @EqualsAndHashCode.Exclude
    private Long id;

    @Enumerated(EnumType.ORDINAL)
    @EqualsAndHashCode.Exclude
    private MeasureType measureType = MeasureType.MOISTURE;

    @CreatedDate
    @EqualsAndHashCode.Exclude
    private Date createDate;

    /**
     * Value in range [0.0, 1.0]
     */
    private float value;

    @ManyToOne
    @JoinColumn(name="device_id", nullable = false)
    private Device device;
}
