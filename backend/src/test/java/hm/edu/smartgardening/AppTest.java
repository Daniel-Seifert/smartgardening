package hm.edu.smartgardening;

import com.openpojo.reflection.PojoClass;
import com.openpojo.reflection.impl.PojoClassFactory;
import com.openpojo.validation.Validator;
import com.openpojo.validation.ValidatorBuilder;
import com.openpojo.validation.rule.impl.GetterMustExistRule;
import com.openpojo.validation.rule.impl.SetterMustExistRule;
import com.openpojo.validation.test.impl.GetterTester;
import com.openpojo.validation.test.impl.SetterTester;
import hm.edu.smartgardening.model.Device;
import org.junit.Test;
import org.springframework.test.context.ActiveProfiles;

@ActiveProfiles("dev")
public class AppTest {

  @Test
  public void validateSettersAndGetters() {


    PojoClass activityPojo = PojoClassFactory.getPojoClass(Device.class);


    Validator validator = ValidatorBuilder.create()
            // Lets make sure that we have a getter and a setter for every field defined.
            .with(new SetterMustExistRule()).with(new GetterMustExistRule())

            // Lets also validate that they are behaving as expected
            .with(new SetterTester()).with(new GetterTester()).build();

    // Start the Test
    validator.validate(activityPojo);
  }
}
