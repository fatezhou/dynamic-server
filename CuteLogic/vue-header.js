var vue = new Vue(
    {
        el:"#header-html",
        data(){
            return {
                list:[
                    {
                        text:"one",
                        clickParam:"/one"
                    },
                    {
                        text:"two",
                        clickParam:"/two"
                    },
                    {
                        text:"three",
                        clickParam:"/three"
                    }
                ],
                show: false
            }
        },
        methods:{
            click_vue_header:function(i){
                console.info(i);
            }
        }
    }
)