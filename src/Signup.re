open Utils;
open UserForm;
open Belt.Option;
module SignupForm = Formality.Make(UserForm.Signup);

let initialState = UserForm.Signup.{email: "", password: "", login: "", accepted_agbs: false, optin: false,
  firstname: "", lastname: "", street: "", addresssuffix: "", zip_code: "", city: "", phone: ""
};

let fakeJsonRespone = {| "errors": [{"code":4102,"source":{"pointer":"login"},"detail":"Bitte Loginfeld aus."},{"code":4102,"source":{"pointer":"password"},"detail":"Bitte Passwordfeld aus."},{"code":4102,"source":{"pointer":"email"},"detail":"Bitte Emailfeld aus."}]} |};
let fakeJsonResponsePromise = Js.Promise.make((~resolve, ~reject) => resolve(. fakeJsonRespone));

[@react.component]
let make = () => {
  let form =
    SignupForm.useForm(
      ~initialState,
      ~onSubmit=(state, form) => {

        let payload = Js.Dict.empty()
        Js.Dict.set(payload, "email", Js.Json.string(state.email));
        Js.Dict.set(payload, "login", Js.Json.string(state.login));
        Js.Dict.set(payload, "password", Js.Json.string(state.password));
        Js.Dict.set(payload, "accepted_agb", Js.Json.boolean(state.accepted_agbs));
        Js.Dict.set(payload, "optin", Js.Json.boolean(state.optin));
        Js.Dict.set(payload, "firstname", Js.Json.string(state.firstname));
        Js.Dict.set(payload, "lastname", Js.Json.string(state.lastname));
        Js.Dict.set(payload, "street", Js.Json.string(state.street));
        Js.Dict.set(payload, "addresssuffix", Js.Json.string(state.addresssuffix));
        Js.Dict.set(payload, "zip_code", Js.Json.string(state.zip_code));
        Js.Dict.set(payload, "city", Js.Json.string(state.city));
        Js.Dict.set(payload, "phone", Js.Json.string(state.phone));

        Js.Promise.(
          Fetch.fetchWithInit( 
            Config.Urls.signup,
            Fetch.RequestInit.make(
              ~method_=Post,
              ~mode=CORS,
              ~credentials=Include,
              ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
              ~body=Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(payload))),
              ()
            )
          )
          |> Js.Promise.then_(Fetch.Response.json)
          |> Js.Promise.then_(json => {
            Js.log2("json", json)
            let decodedResponse = UserForm.Decode.response(json);
            Js.log2("decodedResponse", decodedResponse)

            switch (decodedResponse.errors) {
            | None => 
                Js.log2 ("ok ", decodedResponse.tid);
                form.notifyOnSuccess(None);
                resolve(());
            | Some(errors) => 
                Js.log2("errors", errors)
                form.notifyOnFailure(ServerValidation(Some(errors)))
                resolve(())
            };
          })
          |> catch(err => {
            Js.log2("err", err)

            form.notifyOnFailure(UnexpectedServerError(None));
            resolve(())
          })
          |> ignore
        );
      },
    );
    
    <div>
    <form className="lg" onSubmit={form.submit->Formality.Dom.preventDefault}>
      <div className="form-messages-area form-messages-area-lg"></div>
      <input
        placeholder="E-Mail Adresse"
        name="email"
        id="email"
        type_="text"
        className={"mb-2 form-control" ++ switch (Email->(form.result)) {
          | Some(Error(message)) => UserForm.errorClass
          | Some(Ok(Valid | NoValue))
          | None => ""
          } 
        }
        onBlur={_ => form.blur(Email)}
        onChange={event =>
          form.change(
            Email,
            UserForm.Signup.EmailField.update(
              form.state,
              event->ReactEvent.Form.target##value,
            ),
          )
        }
      />
      {switch (Email->(form.result)) {
      | Some(Error(message)) => <UserForm.IsInvalid msg=message />
      | Some(Ok(Valid | NoValue))
      | None => emptyElement
      }}
      <input
        placeholder="Benutzername"
        name="login"
        id="login"
        type_="text"
        className={"mb-2 form-control" ++ switch (Login->(form.result)) {
          | Some(Error(message)) => UserForm.errorClass
          | Some(Ok(Valid | NoValue))
          | None => ""
          } 
        }
        onBlur={_ => form.blur(Login)}
        onChange={event =>
          form.change(
            Login,
            UserForm.Signup.Login.update(
              form.state,
              event->ReactEvent.Form.target##value,
            ),
          )
        } 
      />
      {switch (Login->(form.result)) {
      | Some(Error(message)) => <UserForm.IsInvalid msg=message />
      | Some(Ok(Valid | NoValue))
      | None => emptyElement
      }}
      <input
        placeholder="Passwort"
        name="password"
        id="password"
        type_="password"
        className={"mb-2 form-control" ++ switch (Password->(form.result)) {
          | Some(Error(message)) => UserForm.errorClass
          | Some(Ok(Valid | NoValue))
          | None => ""
          } 
        }
        onBlur={_ => form.blur(Password)}
        onChange={event =>
          form.change(
            Password,
            UserForm.Signup.PasswordField.update(
              form.state,
              event->ReactEvent.Form.target##value,
            ),
          )
        } 
      />
      {switch (Password->(form.result)) {
      | Some(Error(message)) => <UserForm.IsInvalid msg=message />
      | Some(Ok(Valid | NoValue))
      | None => emptyElement
      }}
        <label className={"rounded"++ switch (Terms->(form.result)) {
                | Some(Error(message)) => UserForm.errorClass ++ " text-danger"
                | _ | Some(Ok(Valid)) => ""
                }
              }>
          <input name="accepted_agbs" type_="checkbox"
            className={"rounded"++ switch (Terms->(form.result)) {
                | Some(Error(message)) => UserForm.errorClass ++ " text-danger"
                | _ | Some(Ok(Valid)) => ""
                }
              }          
            checked={form.state.accepted_agbs}
            disabled={form.submitting}
            onBlur={_ => form.blur(Terms)}
            onChange={event =>
              form.change(
                Terms,
                UserForm.Signup.TermsCheckox.update(
                  form.state,
                  event->ReactEvent.Form.target##checked,
                ),
              )
            }
           />
          " Ich bin mit den "->str
          <a target="_blank" href="https://www.company.com/de/teilnahmebedingungen">"Nutzungsbedingungen"->str</a>
          {j|
            von company einverstanden und stimme zu, dass mit meiner Registrierung
            die oben stehenden Daten an company übergeben werden.
          |j}->str
        </label>
          {switch (Terms->(form.result)) {
          | Some(Ok(Valid)) | None => emptyElement
          | Some(Error(message)) => <UserForm.IsInvalid msg=message />
          }}
        <label className="checkbox">
          <input name="optin" type_="checkbox"
            className="rounded"
            checked={form.state.optin}
            disabled={form.submitting}
            onBlur={_ => form.blur(OptinCheckox)}
            onChange={event =>
              form.change(
                OptinCheckox,
                UserForm.Signup.OptinCheckox.update(
                  form.state,
                  event->ReactEvent.Form.target##checked,
                ),
              )
            }
           />
          {j|
            Ich bin jederzeit widerruflich damit einverstanden, dass mich die Westdeutsche Zeitung GmbH & Co. KG
            auch durch ihre Dienstleister per E-Mail über ihre Verlagsprodukte und -angebote informiert.
          |j}->str
          <strong>"Als Dank erhalte ich 10 Freiminuten."->str</strong>
        </label>

        {switch (form.status) {
        | Submitted => {
          clearModalAndAfterLoginHook();
          <div className="success">
            {j|Logged In|j}->str
          </div>
        }
        | SubmissionFailed(submissionError) => 
          switch(submissionError) {
          | ServerValidation(Some(errors)) => {
            // validation
            // errors |> Belt.Array.filter(_, x.code = 4102 => x);
            let validationErrors = errors -> Belt.Array.keep(x => x.code->getWithDefault(0) == 4102);

            Js.log(validationErrors);
            let mapd = validationErrors
            |> Belt.Array.map(_, (x) => <li key={(x.detail->getWithDefault(""))}>(x.detail->getWithDefault(""))->str</li>);

            let errorsWithSource = errors->Belt.Array.keep(error => error.source->flatMap(source => source.pointer) != None);
            Js.log2("errorsWithSource", errorsWithSource);

            Js.log(form.result);


            <ul className="invalid-feedback" style=(ReactDOMRe.Style.make(~display="block", ()))>
              (mapd |> ReasonReact.array)

              // ReasonReact.array(Array.of_list(mapd))
            </ul>
          }
          | UnexpectedServerError(msg) => 
            <div className="alert alert-danger fade in">
              (steDefault(msg, ErrorMessage.general))
            </div>
          }
        | _ => emptyElement
        }}


      <input placeholder="Vorname" name="firstname" id="firstname" type_="text" className="mb-2 form-control"
        onBlur={_ => form.blur(Firstname)}
        onChange={event =>
          form.change(
            Firstname, UserForm.Signup.Firstname.update(form.state, event->ReactEvent.Form.target##value)
          )
        }
      />
      <input placeholder="Nachname" name="lastname" id="lastname" type_="text" className="mb-2 form-control" 
        onBlur={_ => form.blur(Lastname)}
        onChange={event =>
          form.change(
            Lastname, UserForm.Signup.Lastname.update(form.state, event->ReactEvent.Form.target##value),
          )
        }
      />
      <input placeholder={j|Straße|j} name="street" id="street" type_="text" className="mb-2 form-control" 
        onBlur={_ => form.blur(Street)}
        onChange={event =>
          form.change(
            Street, UserForm.Signup.Street.update(form.state, event->ReactEvent.Form.target##value),
          )
        }
      />

      <input placeholder="Adresszusatz" name="addresssuffix" id="addresssuffix" type_="text" className="mb-2 form-control" 
        onBlur={_ => form.blur(Addresssuffix)}
        onChange={event =>
          form.change(
            Addresssuffix, UserForm.Signup.Addresssuffix.update(form.state, event->ReactEvent.Form.target##value),
          )
        }
      />
      <input placeholder="PLZ" name="zip_code" id="zip_code" type_="text" className="mb-2 form-control" 
        onBlur={_ => form.blur(Zip_code)}
        onChange={event =>
          form.change(
            Zip_code, UserForm.Signup.Zip_code.update(form.state, event->ReactEvent.Form.target##value),
          )
        }
      />

      <input placeholder="Ort" name="city" id="city" type_="text" className="mb-2 form-control" 
        onBlur={_ => form.blur(City)}
        onChange={event =>
          form.change(
            City, UserForm.Signup.City.update(form.state, event->ReactEvent.Form.target##value),
          )
        }
      />

      <input placeholder="Handynummer" name="phone" id="phone" type_="text" className="mb-2 form-control" 
        onBlur={_ => form.blur(Phone)}
        onChange={event =>
          form.change(
            Phone, UserForm.Signup.Phone.update(form.state, event->ReactEvent.Form.target##value),
          )
        }
      />

        <button className="mb-4 btn btn-primary form-button">
          (form.submitting ? "Wird bearbeitet..." : "Jetzt Kostenlos registrieren")->str
        </button>
      </form>
    </div>
};
