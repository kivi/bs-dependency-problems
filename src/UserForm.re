open Formality;

let errorClass = " is-invalid";
module ErrorMessage = {
    let loginRequired = {j|Bitte geben Sie einen Benutzernamen ein.|j}
    let loginInvalid = {j|Bitte geben Sie einen Benutzernamen ein.|j}
    let emailInvalid = {j|Bitte geben Sie eine E-Mailadresse ein.|j}
    let emailRequired = {j|Bitte geben Sie eine E-Mailadresse ein.|j}
    let passwordRequired = {j|Bitte geben Sie ein Passwort ein.|j}
    let passwordInvalid = {j|Ihr Passwort muss mindestens 8 Zeichen lang sein, aus Groß- und Kleinbuchstaben bestehen und mindestens eine Zahl enthalten.|j}
    let terms = {j|Um mit der Registrierung fortzufahren, müssen Sie die --- Nutzungsbedingungen bestätigen.|j}
    let general = {j|Unerwarteter Fehler. Bitte versuchen Sie es später erneut.|j}
}

module Regex {
  let login = [%bs.re {|/^.{2,50}$/|}];
  let email = [%bs.re {|/^.+@.{2,}\.[a-zA-Z]{2,}$/|}];
  let password = [%bs.re {|/^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)[a-zA-Z\d]{8,}$/|}];
}

type response = {
//   data: option(data),
  errors: option(array(errors)),
  tid: option(string)
} and data = {
    id: option(string),
    attributes: option(attributes)
} and attributes = {
    id: option(string)
} and errors = {
    id: option(string),
    code: option(int),
    title: option(string),
    detail: option(string),
    source: option(source)
} and source = {
    parameter: option(string),
    pointer: option(string)
};

module Decode = {
    open Json.Decode;

    let data = json => {
    }
    let source = json => {
        parameter: json |> optional(field("parameter", string)),
        pointer: json |> optional(field("pointer", string)),
    }
    let errors = json => {
        id: json |> optional(field("id", string)),
        code: json |> optional(field("code", int)),
        title: json |> optional(field("title", string)),
        detail: json |> optional(field("detail", string)),
        source: json |> optional(field("source", source)),
    };
    let response = json => {
        // data: json |> optional(field("data", data)),
        errors: json |> optional(field("errors", array(errors))),
        tid: json |> optional(field("tid", string)),
    };

    // bs-decode
    /* // start with a simple constructor function for our type
    let make = (errors, tid) => { errors, tid };


    let errors = Decode.AsResult.OfParseError.Pipeline.(
        succeed(make)
        |> field("id", string)
        |> run(json)
    );

    let response = Decode.AsResult.OfParseError.Pipeline.(
        succeed(make)
        /* |> field("data", data), */
        |> field("errors", array(errors))
        |> field("tid", string)
        |> run(json)
    ); */
};



module IsInvalid = {
  [@react.component]
  let make = (~msg) => {
    <div className="mb-2 invalid-feedback" style=(
            ReactDOMRe.Style.make(~display="block", ()))
    > msg->React.string</div>
  };
};


module Login = {
    type field =
      | Login
      | Password
      | RememberMe;
    
    type state = {
        login: string,
        password: string,
        rememberMe: bool,
    };
    
    type message = string;
    type submissionError = 
      | BadPassword(option(string))
      | UnexpectedServerError(option(string));

    module LoginField = {
        let update = (state, value) => {...state, login: value};
        
        let validator = {
            field: Login,
            strategy: Strategy.OnFirstSuccessOrFirstBlur,
            dependents: None,
            validate: ({login}) => {
                switch (login) {
                | _ as value when !Regex.login->Js.Re.test_(value) =>
                    Error(ErrorMessage.loginInvalid)
                | _ => Ok(Valid)
                };
            },
        };
    };

    module PasswordField = {
        let update = (state, value) => {...state, password: value};
        
        let validator = {
            field: Password,
            strategy: Strategy.OnFirstBlur,
            dependents: None,
            validate: ({password}) =>
            switch (password) {
            | "" => Error(ErrorMessage.passwordRequired)
            | _ => Ok(Valid)
            },
        };
    };

    module RememberMeField = {
        let update = (state, value) => {...state, rememberMe: value};
    };

    let validators = [LoginField.validator, PasswordField.validator]
};

module Signup = {
    type field =
      | Email
      | Login
      | Password
      | Terms
      | OptinCheckox
      | Firstname
      | Lastname
      | Street
      | Addresssuffix
      | Zip_code
      | City
      | Phone;
    
    type state = {
        email: string,
        login: string,
        password: string,
        accepted_agbs: bool,
        optin: bool,
        // optin: bool,

        firstname: string,
        lastname: string, // lastname: string,
        street: string,
        // street_no: string, 
        addresssuffix: string,
        zip_code: string,
        city: string,
        phone: string,

// g-recaptcha-response: xxxx
// captcha-response: xxxx
// g-recaptcha-response
// captcha-response
    };
    
    type message = string;
    type submissionError = 
      | ServerValidation(option(array(errors)))
      | EmailTaken(option(string))
      | UnexpectedServerError(option(string));

    module EmailField = {
        let update = (state, value) => {...state, email: value};
        
        let validator = {
            field: Email,
            strategy: Strategy.OnFirstSuccessOrFirstBlur,
            dependents: None,
            validate: ({email}) => {
                switch (email) {
                | "" => Error(ErrorMessage.emailRequired)
                | _ as value when !Regex.email->Js.Re.test_(value) =>
                    Error(ErrorMessage.emailInvalid)
                | _ => Ok(Valid)
                };
            },
        };
    };

    module Login = {
        let update = (state, value) => {...state, login: value};
        
        let validator = {
            field: Login,
            strategy: Strategy.OnFirstSuccessOrFirstBlur,
            dependents: None,
            validate: ({login}) => {
                switch (login) {
                | "" => Error(ErrorMessage.loginRequired)
                | _ as value when !Regex.login->Js.Re.test_(value) =>
                Error(ErrorMessage.loginInvalid)
                | _ => Ok(Valid)
                };
            },
        };
    };

    module PasswordField = {
        let update = (state, value) => {...state, password: value};
        
        let validator = {
            field: Password,
            strategy: Strategy.OnFirstBlur,
            dependents: None,
            validate: ({password}) =>
            switch (password) {
            | _ as value when !Regex.password->Js.Re.test_(value) => Error(ErrorMessage.passwordInvalid)
            | _ => Ok(Valid)
            },
        };
    };

    module TermsCheckox = {
        let update = (state, value) => {...state, accepted_agbs: value};
        let validator = {
            field: Terms,
            strategy: Strategy.OnSubmit,
            dependents: None,
            validate: ({accepted_agbs}) => {
                switch (accepted_agbs) {
                | true => Ok(Valid)
                | false => Error(ErrorMessage.terms)
                }}
        }
    }

    module OptinCheckox = {
        let update = (state, value) => {...state, optin: value};
        let validator = {
            field: Terms,
            strategy: Strategy.OnFirstBlur,
            dependents: None,
            validate: ({optin}) => 
                switch (optin) {
                | true => Ok(Valid)
                | false => Error("")
                }
        }
    }

    module Firstname = {
        let update = (state, value) => {...state, firstname: value};
    };
    module Lastname = {
        let update = (state, value) => {...state, lastname: value};
    };
    module Street = {
        let update = (state, value) => {...state, street: value};
    };
    module Addresssuffix = {
        let update = (state, value) => {...state, addresssuffix: value};
    };
    module Zip_code = {
        let update = (state, value) => {...state, zip_code: value};
    };
    module City = {
        let update = (state, value) => {...state, city: value};
    };
    module Phone = {
        let update = (state, value) => {...state, phone: value};
    };

    let validators = [EmailField.validator, Login.validator, PasswordField.validator, TermsCheckox.validator]
}


module PasswordRecover = {
    type field =
      | Email;
    
    type state = {
        email: string,
    };
    
    type message = string;
    type submissionError = 
      | ServerValidation(option(array(errors)))
      | UnexpectedServerError(option(string));

    module EmailField = {
        let update = (state, value) => {...state, email: value};
        
        let validator = {
            field: Email,
            strategy: Strategy.OnFirstSuccessOrFirstBlur,
            dependents: None,
            validate: ({email}) => {
                switch (email) {
                | "" => Error(ErrorMessage.emailRequired)
                | _ as value when !Regex.email->Js.Re.test_(value) =>
                Error(ErrorMessage.emailInvalid)
                | _ => Ok(Valid)
                };
            },
        };
    };
    let validators = [EmailField.validator,]
}

