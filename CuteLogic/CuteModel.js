var CuteLogin = require("./CuteLogin.js");


function CuteModel(){
    this.Do = function(name, version, paramObject, callback){
        switch(name){
            case "login":
                CuteLogin(version, paramObject, callback);
                break;
            default:
                break;
        }
    }
}

module.exports = CuteModel;