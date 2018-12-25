const CuteResponse = require('./CuteResponse.js')
var mysql = require('mysql');
var pool = mysql.createPool({
    host: '127.0.0.1',
    user: 'root',
    password: '123456',
    database: 'mysql',
    port: 3306
});

var query = function(sql, param, callback){
    pool.getConnection(function(err, conn){
        let response = new CuteResponse;
        if(err){
            console.info(err);
            callback(response.OnBadSQL({}, "sql error"));
        }else{
            conn.query(sql, param, function(queryErr, res, field){
                conn.release();
                if(!queryErr){                      
                    try{
                        res = JSON.stringify(res);   
                        res = JSON.parse(res);
                    }catch(jsonErr){
                        res["error"] = true;
                    }
                    callback({res:res});
                }else{
                    console.info(queryErr);                    
                    callback({error: true});
                }   
            })
        }
    })
}

module.exports = query;

// query("select * from cats where name=?", ['Jack'], function(object){
//     console.info(object);
// })
