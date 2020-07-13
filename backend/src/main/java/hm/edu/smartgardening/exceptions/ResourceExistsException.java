package hm.edu.smartgardening.exceptions;

import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;

@ResponseStatus(value = HttpStatus.CONFLICT)
public class ResourceExistsException extends RuntimeException {
    public ResourceExistsException() {
        super("The resource you tried to create already exists!");
    }
}
