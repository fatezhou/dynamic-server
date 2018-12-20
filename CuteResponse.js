function CuteResponse(){    
    this.OnSucc = function(obj){
        var res = {};
        res.code = 0;
        res.res = obj;
        return JSON.stringify(res);
    };
    this.OnFail = function(obj, reason, code){
        var res = {};
        res.code = code || 1;
        res.res = obj;
        res.reason = reason;
        return JSON.stringify(res);
    };

    this.BadParamCode = 2;
    this.OnBadParam = function(obj, reason){
        var res = {};
        res.code = this.BadParamCode;
        res.reason = reason || "";
        res.res = obj;
        return JSON.stringify(res);
    }

    this.BadSQLCode = 3;
    this.OnBadSQL = function(obj, reason){
        var res = {};
        res.code = this.BadSQLCode;
        res.res = obj;
        res.reason = reason || "";
        return JSON.stringify(res);
    }

    this.BadResquestCode = 4;
    this.OnBadResquest = function(obj, reason){
        var res = {};
        res.code = this.BadResquestCode;
        res.reason = reason || "";
        res.res = obj;
        return JSON.stringify(res);
    }
}

module.exports = CuteResponse;