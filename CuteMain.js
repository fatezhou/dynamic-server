const http = require('http');
const fs = require('fs');
const url = require('url');
const path = require('path');
const ContentType = require("./CuteContent-type.js");
const Cmd = require("./CuteCmd.js");
const CuteApi = require("./CuteApi.js");


http.createServer(function(req, res){
    var reqUrl = req.url;
    var urls = url.parse(reqUrl).pathname;
    var ext = path.extname(urls);
    var contentType = ContentType.GetContentType(ext);
    if(ext == "" && urls != "/"){
        //go to api
        let obj = new CuteApi();
        obj.router(req, res);
        return;
    }else{        
        if(contentType == "" && urls != "/"){
            fs.readFile("./html/404.html", "utf-8", function(err404, data404){
                res.writeHead(404, {"content-type" : "text/html;charset='utf-8'"});
                res.write(data404);
                res.end();
            });
            return;
        }

        if(urls == "/"){
            urls = "/html/index.html";
        }
        fs.readFile("./" + urls, function(err, data){
            if(err){
                fs.readFile("./html/404.html", "utf-8", function(err404, data404){
                    res.writeHead(404, {"content-type" : "text/html;charset='utf-8'"});
                    res.write(data404);
                    res.end();
                });
            }else{
                res.writeHead(200, {"content-type": "" + contentType + ";charset='utf-8'"});
                res.write(data);
                res.end();
            }
        })        
    }
    console.info("req:", urls);
}).listen(Cmd.GetCmd("--port", 12330));