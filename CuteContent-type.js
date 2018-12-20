exports.GetContentType = function(ext){
    var obj = {
        ".css":"text/css",
        ".doc":"application/msword" ,
        ".docx":"application/vnd.openxmlformats-officedocument.wordprocessingml.document" ,
        ".gif":"image/gif" ,
        ".htm":"text/html" ,
        ".html":"text/html" ,
        ".ico":"image/x-icon" ,
        ".jpe":"image/jpeg" ,
        ".jpeg":"image/jpeg" ,
        ".jpg":"image/jpeg" ,
        ".jpz":"image/jpeg" ,
        ".js":"application/x-javascript" ,
        ".mp3":"audio/mpeg" ,
        ".mp4":"video/mp4" ,
        ".pdf":"application/pdf" ,
        ".ppt":"application/vnd.ms-powerpoint" ,
        ".pptx":"application/vnd.openxmlformats-officedocument.presentationml.presentation" ,
        ".ttf":"application/octet-stream" ,
        ".wav":"audio/x-wav" ,
        ".wpt":"x-lml/x-gps" ,
        ".xhtm":"application/xhtml+xml" ,
        ".xhtml":"application/xhtml+xml" ,
        ".xls":"application/vnd.ms-excel" ,
        ".xlsx":"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet" ,
        ".zip":"application/zip" ,
        ".json":"application/json"
    }
    return obj[ext] || "" ;
}

