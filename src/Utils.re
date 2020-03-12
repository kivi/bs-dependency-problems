open Belt.Option;

let str = ReasonReact.string;
let arrayEl = ReasonReact.array;
let emptyElement = ReasonReact.null;
let stro = optionalStr => 
  optionalStr
    -> mapWithDefault (emptyElement, data => ReasonReact.string(data));
let steDefault = (str, default) => 
  str
    -> mapWithDefault (ReasonReact.string(default), str => ReasonReact.string(str));
  
let stD = (s) => getWithDefault(s, "");

let checkBoxToStr = (checked) => 
  switch (checked) {
  | true => "true"
  | false => "false"
  };

let clearModal = () => {
  ReasonReactRouter.push("#");
} 
let clearModalWithDelay = () => Js.Global.setTimeout(clearModal, Config.modalClearDelay);
let afterLogin = [%bs.raw {| function (tid) { onPTAuthComplete(tid) } |}];
let clearModalAndAfterLoginHook = () => {
    clearModalWithDelay();
    "some id" |> afterLogin;
};


[@bs.val] external requireCSS: string => unit = "require";
[@bs.val] external requireAssetURI: string => string = "require";

[@bs.val] external encodeURIComponent : string => string = "" ;
[@bs.val] external decodeURIComponent : string => string = "" ;
[@bs.val] external atob : string => string = "";
[@bs.val] external parseTid : string => string = "";

[@bs.val] [@bs.scope ("document", "location")] external origin : string = "origin";

let id = a => a;




