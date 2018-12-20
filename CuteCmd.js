exports.GetCmd = function(option, def){
    var arguments = process.argv.splice(2);
    try{
        for(var i = 0; i < arguments.length; i++){
            if(arguments[i] == option){
                console.info("cmd->[", option, "]:[", arguments[i + 1], "]");
                return arguments[i + 1];
            }
        }
    }catch(err){

    }
    console.info("no found option, ouput the def:", def);
    return def;
}