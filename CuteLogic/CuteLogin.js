const nodeCookie = require('node-cookie')
var query = require('./../CuteMySql.js')
const CuteResponse = require('./../CuteResponse.js')
console.info(CuteResponse);
const MD5 = require('./../CuteMd5.js')

function CuteLogin(version, paramObject, callback){
    var response = new CuteResponse;
        if(!paramObject.user || !paramObject.password){            
            callback(response.OnBadParam({}, "param is error"));
        }else{
            query("select id from my_user where user =? and password =?", [paramObject.user, paramObject.password], function(res){
                if(res.error){
                    callback(response.OnBadSQL());
                }else{
                    if(res.res.length > 0){
                        let tokenBase = "" + Math.random();
                        let token = MD5(tokenBase);
                        let expireTime = (new Date()).getTime() + 36000;

                        query("update my_user set token=? ,expire_time=? where id=?", [token, expireTime, res.res[0].id], function(insertTokenRes){
                            console.info(insertTokenRes);
                            if(insertTokenRes.error){
                                callback(response.OnBadSQL({}));
                            }else{
                                callback(response.OnSucc({token:token}));
                            }
                        });
                    }
                }
            })
        }
}

module.exports = CuteLogin;

CuteLogin("", {user:"fatezhou", password:"123456"}, function(res){
    console.info(res)
})