var vue = new Vue(
    {
        el:"#header-html",
        data(){
            return {
                list:[
                    {
                        text:"one"
                    },
                    {
                        text:"two"
                    },
                    {
                        text:"three"
                    }
                ],
                show: false
            }
        }
    }
)