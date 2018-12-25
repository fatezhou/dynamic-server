const url = require('url');
const CuteResponse = require('./CuteResponse.js')
const nodeCookie = require('node-cookie')
const CuteModel = require('./CuteLogic/CuteModel.js')

function CuteApi(){
    this.router = function(req, res){
        var response = new CuteResponse;
        let urls = url.parse(req.url).pathname;
        let arr = urls.split("/");// /api/v1.0/xxxxx
        console.info(arr);
        if(arr.length != 4){
            res.writeHead(404, {"content-type" : "text/html;charset='utf-8'"});
            res.write(response.OnBadResquest({}));
            res.end();
            return;
        }

        // console.info(nodeCookie.parse(req));
        // nodeCookie.create(res, 'user', 'admin');
        // nodeCookie.create(res, 'token', '123456');

        let data = "";
        req.on('data', function(chunk){
            data += chunk;
        });
        req.on('end', function(){
            try{
                let param = JSON.parse(data);

                let cuteModel = new CuteModel;
                cuteModel.Do(arr[3], arr[2], param, function(ModelData){
                    res.writeHead(200, {"content-type" : "text/html;charset='utf-8'"});
                    res.write(JSON.stringify(ModelData));
                    res.end();
                });                         
            }catch(err){
                res.writeHead(500, {"content-type" : "text/html;charset='utf-8'"});
                res.write(response.OnBadParam({}));
                res.end();
            }            
            
        })
    }
}

module.exports = CuteApi;