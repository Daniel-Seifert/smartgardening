package hm.edu.smartgardening.model;

import lombok.*;
import org.springframework.data.annotation.CreatedDate;
import org.springframework.data.annotation.LastModifiedDate;
import org.springframework.data.jpa.domain.support.AuditingEntityListener;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.UUID;

@Entity
@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
@EntityListeners(AuditingEntityListener.class)
@EqualsAndHashCode
public class Device {

    @Id
    @EqualsAndHashCode.Exclude
    private UUID id;

    @Column(length = 512)
    private String name;

    @CreatedDate
    @EqualsAndHashCode.Exclude
    private Date createDate;
    @LastModifiedDate
    @EqualsAndHashCode.Exclude
    private Date updateDate;

    @OneToOne(cascade = CascadeType.ALL)
    @EqualsAndHashCode.Exclude
    private Config config;

    @OneToMany(
            cascade = CascadeType.ALL,
            orphanRemoval = true
    )
    @EqualsAndHashCode.Exclude
    private List<Measurement> measurements = new ArrayList<>();

    public void addMeasurement(Measurement measurement) {
        measurements.add(measurement);
    }

    public void setMeasurements(List<Measurement> measurements) {
        this.measurements.retainAll(measurements);
        this.measurements.addAll(measurements);
    }
}
