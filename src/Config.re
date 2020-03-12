let baseUrl = "http://localhost:3000"
let providerId = "456";
let providerId = "123";


let modalClearDelay = 5500;

module Cookie = {
    let domain = "example.de";
    let domain = "admin.pt.localhost";
    let domain = "accesstest.example.com";
    let path = "pt";
    let maxAgeInMinutes = 100;
    let maxAge = maxAgeInMinutes * 60;
}

module Urls = {
    let location = [%raw {|
       function () {
        return (encodeURIComponent(location.origin + location.pathname + location.search));
       }
    |}];
    let login = baseUrl ++ "/login";
    let loginStatus = baseUrl ++ "/info/status";
    let passwordRecover = baseUrl ++ "/password/reset";
    let facebookLogin = baseUrl ++ "/webclient/facebook/login" ++  
        "?redirect_url=" ++ location() ++ "&provider=" ++ providerId
    let signup = baseUrl ++ "/signup";
    let providerUserToken = baseUrl ++ "/provider/userToken";
    let productToken = (id) => baseUrl ++ "/provider/productToken/" ++ id;
    let userToken = baseUrl ++ "/provider/loginToken";
}

