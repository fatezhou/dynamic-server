var crypto = require('crypto')
var md5 = crypto.createHash('md5')

function MD5(str){
    md5.update(str);
    return md5.digest('hex');
}

module.exports = MD5;
