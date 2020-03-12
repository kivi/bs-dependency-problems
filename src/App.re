open Utils;
 open Webapi.Dom;

type state =
  | NotAsked
  | Loading
  | LoggedIn(user)
  | LoggedOut;

type action =
  | CheckLoggedIn
  | Login(user)
  | LoginFailed;

let createLoginTokenAndGetProductId = (productId) => {
  Js.Promise.(
    loginToken()
    |> then_(loginToken => {
      productToken(productId)
      |> then_(productToken => {
        plChekout(loginToken, productToken);
        resolve()
      })
    })
  );
}

let fetchLoginStatus = () => Js.Promise.(
  Fetch.fetchWithInit(
    Config.Urls.loginStatus,
    Fetch.RequestInit.make(~credentials=Include, ()),
  )
  |> then_(Fetch.Response.json)
  |> then_(json => json |> Decode.user |> (user => Some(user) |> resolve))
  |> catch(_err => resolve(None))
);

module Route = {
  type t =
    | Login
    | Order(string)
    | PasswordRecover
    | PasswordRecoverConfirmation
    | Signup
    | SignupConfirmation
    // | SignupTerms
    | None;

  let parseTid = [%raw {| 
function (j) { return JSON.parse(j).tid } 
|}];


  let fromUrl = (url: ReasonReactRouter.url) => {
    let search = Js.String.split("&", url.search);
    switch (search[0]) {
    | Some("ptfblogin=1") => {
        let ptud = Belt.Option.getWithDefault(search[1], "no tid") |> Js.String.split("=")
        Belt.Option.getWithDefault(ptud[1], "{}") |> 
        Utils.decodeURIComponent |>
          Utils.atob |>
          Utils.parseTid 
        clearModalAndAfterLoginHook();
        ();
      }
    | _ => ();
    };



    let hash = Js.String.split("=", url.hash);
    switch (hash) {
    | ([|"login"|]) => Login
    // | ([|"order", id|]) => Js.Promise.(loginToken() |> then_(Js.log)); Order(id);
    | ([|"offorder", id|]) => createLoginTokenAndGetProductId(id); Order(id);
    | ([|"recover"|]) => PasswordRecover
    | ([|"recoverconfirmation"|]) => PasswordRecoverConfirmation
    | ([|"signup"|]) => Signup
    | ([|"signupconfirmation"|]) => SignupConfirmation
    | _ => None
    };
  };
};


[@react.component]
let make = () => {

  let buttonElements = ReactDOMRe._getElementsByClassName("somebutton")
  |> Belt.Array.map(_, (x) => HtmlElement(x) );
  Js.log2("orderbutton", buttonElements);

  let route = ReasonReactRouter.useUrl()->Route.fromUrl;
  switch(route) {
  | Order(id) => 
    <div>("Order ID: " ++ id)->str</div>
    // emptyElement;
  | None => emptyElement
  | Login | Signup | SignupConfirmation | PasswordRecover | PasswordRecoverConfirmation =>
    <div>
      <div
        className="modal fade show"
        role="dialog"
        tabIndex=(-1)
        style=(ReactDOMRe.Style.make(~display="block", ()))
      >
        <div className="modal-dialog" role="document">
          <div className="modal-content">
              <div className="modal-header">
                <div className="container header-padding">
                  <div className="wzlogo"></div>
                </div> 
                <div className="modal-title headline-side header-padding">
                  (switch(route) {
                  | Login => "Anmelden"->str
                  | PasswordRecover => "Haben Sie Ihr Passwort vergessen?"->str
                  | Signup => "Registrieren"->str
                  | SignupConfirmation => {j|Vielen Dank für Ihre Registrierung|j}->str
                  | None | PasswordRecoverConfirmation => emptyElement
                  })
                </div>
                <div>
                  <CloseButton />
                </div>
              </div>
              <div className="modal-body">
                (switch(route) {
                | Login => {
                  <div>                 
                    <Login />
                    <button
                      className="btn button-light form-button"
                      onClick={_ => ReasonReactRouter.push("#signup")}>
                      {j|Noch kein Kunde? Bitte hier registrieren.|j}->str
                    </button>  
                  </div>                 
                }
                | Signup => {
                  <div>                
                    <Signup /> 
                    <Divider />
                    <button
                      className="btn-secondary btn form-button"
                      onClick={_ => ReasonReactRouter.push("#login")}>
                      "Anmelden"->str
                    </button>
                  </div>
                }
                | PasswordRecover => <PasswordRecover />
                | PasswordRecoverConfirmation => <PasswordRecoverConfirmation />
                | SignupConfirmation => <SignupConfirmation />
                | None => emptyElement
                })
              </div>
              <div className="modal-bottom">
              </div>
          </div>
        </div>
      </div>
      <div className="modal-backdrop fade show"></div>
    </div>
  }
};
