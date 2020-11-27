$(document).ready(function () {

    /*      Form Check      */
    let nameisOkay = false;
    let emailisOkay = false;
    let passwdisOkay = false;
    let proAndCityisOkay = false;
    let phoneNumisOkay = false;
    //昵称,4-16位，不可以数字或特殊符号开头且不包括空格
    $('#name').blur(function () {
        let name = $('#name').val();
        let reg_name = /^[\u4E00-\u9FA5a-zA-Z_]+[^\s]*\w*$/;
        let $e_name = $('#e_name');
        $e_name.empty();
        if(name.length>3&&reg_name.test(name)&&name.length<=16){
            $e_name.append("<span class='okay'>用户名可用</span>");
            nameisOkay = true;
        }else{
            $e_name.append("<span class='error'>用户名无效！(4-16位，不可以数字或特殊符号开头且不包括空格)</span>");
            nameisOkay = false;
        }
    });
    //邮箱
    $('#email').blur(function () {
        let email = $('#email').val();
        let reg_email = /^[a-zA-Z0-9]+([_\.][a-zA-Z0-9]+)*@([a-zA-Z0-9]+\.)+[a-zA-Z]{2,6}$/;
        let $e_email = $('#e_email');
        $e_email.empty();
        if(reg_email.test(email)){
            $e_email.append("<span class='okay'>邮箱可用</span>");
            emailisOkay = true;
        }else {
            $e_email.append("<span class='error'>邮箱无效</span>");
            emailisOkay = false;
        }
    });

    $('#phoneNumber').blur(function () {
        let phoneNum = $('#phoneNumber').val();
        let reg_phoneNum = /^1([34578])\d{9}$/;
        let $e_phoneNum = $('#e_phoneNumber');
        $e_phoneNum.empty();
        if(!reg_phoneNum.test(phoneNum)){
            $e_phoneNum.append("<span class='error'>请填写正确的手机号</span>");
            phoneNumisOkay = false;
        }else {
            phoneNumisOkay = true;
        }
    });

    //密码强度
    // 纯数字:弱，   字母加数字：中，    大写字母+字母+数字：强，   特殊符号+字母+数字：最强大脑
    $('#passwd').blur(function () {
        let passwd = $('#passwd').val();
        let reg_passwd_week = /\d/;
        let reg_passwd_middle = /[a-z]+\d*/;
        let reg_passwd_strong = /[A-Z]+[a-z0-9]*/;
        let reg_passwd_ultra = /[^a-zA-Z0-9]+\w*/;
        let $e_passwd = $('#e_passwd');
        $e_passwd.empty();
        if(passwd.length<9){
            $e_passwd.append("<span class='error'>请输入至少9位密码</span>");
            passwdisOkay = false;
        }else if(reg_passwd_ultra.test(passwd)) {
            $e_passwd.append("<span class='ultra'>您就是最强大脑？</span>");
            passwdisOkay = true;
        }else if(reg_passwd_strong.test(passwd)){
            $e_passwd.append("<span class='okay'>强</span>");
            passwdisOkay = true;
        }else if(reg_passwd_middle.test(passwd)){
            $e_passwd.append("<span class='middle'>中</span>");
            passwdisOkay = true;
        }else if(reg_passwd_week.test(passwd)){
            $e_passwd.append("<span class='error'>弱</span>");
            passwdisOkay = true;
        }
    });
    //重复密码验证
    $('#repetPasswd').blur(function () {
        let passwd = $('#passwd').val();
        let repeat = $('#repetPasswd').val();
        let $e_repeat = $('#e_repeat');
        $e_repeat.empty();
        if(passwd !== repeat){
            $e_repeat.append("<span class='error'>两次输入的密码不相等</span>");
            passwdisOkay = false;
        }else {
            passwdisOkay = true;
        }
    });

    /*      Register        */
    $('#register').blur(function () {
        if($("#province").val()!=="") {
            if($("#city").val()!==""){
                proAndCityisOkay = true;
            }
        }
        if(nameisOkay&&emailisOkay&&passwdisOkay&&proAndCityisOkay&&phoneNumisOkay) {
            let json = $('#register-form').serialize();
            console.log('json : ',decodeURIComponent(json));
        } else {
            alert("请按正确格式填写信息");
        }
    });

    /*      商品排序        */
    $('#sort').change(function () {
        let pageType = "";
        if(isCube){
            pageType = "cube";
        }else {
            pageType = "list";
        }
        let val =  $('#sort').children('option:selected').val();
        let $list = $('.goods-list');
        console.log('刷新中');
        switch (val) {
            case '0':

                $list.load("OrderGoods.jsp?sort=0&pageType="+pageType);
                console.log(pageType);
                break;
            case '1':
                $list.load("OrderGoods.jsp?sort=1&pageType="+pageType);
                console.log(pageType);
                break;
            case '2':
                $list.load("OrderGoods.jsp?&pageType="+pageType);
                console.log(pageType);
                break;
            default:break;
        }
    });

    /*      平铺/列表切换     */
    var isCube = true;      //默认平铺
    $('#cube').click(function () {
        isCube = true;
        $('#list').removeClass("active");
        $(this).addClass("active");
        let sortVal = $('#sort').children('option:selected').val();
        let $list = $('.goods-list');
        console.log('刷新中');
        switch (sortVal) {
            case '0':
                $list.load("OrderGoods.jsp?sort=0&pageType=cube");
                break;
            case '1':
                $list.load("OrderGoods.jsp?sort=1&pageType=cube");
                break;
            case '2':
                $list.load("OrderGoods.jsp?pageType=cube");
                break;
            default:break;
        }
    });

    $('#list').click(function () {
        isCube = false;
        $('#cube').removeClass("active");
        $(this).addClass("active");
        let sortVal = $('#sort').children('option:selected').val();
        let $list = $('.goods-list');
        console.log('刷新中');
        switch (sortVal) {
            case '0':
                $list.load("OrderGoods.jsp?sort=0&pageType=list");
                break;
            case '1':
                $list.load("OrderGoods.jsp?sort=1&pageType=list");
                break;
            case '2':
                $list.load("OrderGoods.jsp?pageType=list");
                break;
            default:break;
        }
    });

    /*  数字增减框   */
    let MAX = 999;
    let MIN = 0;
    $(document).on('click','.quantity-reduce',function () {
        let inputId = "#"+$(this).parent().attr("id")+"input";
        let curPriceId = "#"+$(this).parent().attr("id")+"currentPrice";
        let totalId = "#"+$(this).parent().attr("id")+"total";
        let curPrice = ($(curPriceId).html())*1;
        let oldVal = $(inputId).val();
        let newVal = parseInt(oldVal)-1;
        console.log("newVal:"+newVal);
        console.log("oldVal:"+oldVal);
        console.log("curPrice:"+curPrice);

        if(newVal<MIN||newVal>MAX){
            $(inputId).val(oldVal);
        }else {
            let newTotal = curPrice*newVal;
            $(inputId).val(newVal);
            $(totalId).html(newTotal.toFixed(1));
            $('.cart-sumtotal').html(sum());
        }
    });

    $(document).on('click','.quantity-plus',function () {
        let inputId = "#"+$(this).parent().attr("id")+"input";
        let curPriceId = "#"+$(this).parent().attr("id")+"currentPrice";
        let totalId = "#"+$(this).parent().attr("id")+"total";
        let curPrice = ($(curPriceId).html())*1;
        let oldVal = $(inputId).val();
        let newVal = parseInt(oldVal)+1;
        console.log("newVal:"+newVal);
        console.log("oldVal:"+oldVal);
        console.log("curPrice:"+curPrice);

        if(newVal<MIN||newVal>MAX){
            $(inputId).val(oldVal);
        }else {
            let newTotal = curPrice*newVal;
            $(inputId).val(newVal);
            $(totalId).html(newTotal.toFixed(1));
            $('.cart-sumtotal').html(sum());
        }
    });

    $(document).on('change',"[name='input']",function () {
        let curPriceId = "#"+$(this).parent().attr("id")+"currentPrice";
        let totalId = "#"+$(this).parent().attr("id")+"total";
        let curPrice = ($(curPriceId).html())*1;
        let val = $(this).val();
        if(val>MAX){
            val = MAX;
        }else if(val<MIN){
            val = MIN;
        }else if(val === ""){
            val = 0;
        }
        $(this).val(val);
        $(totalId).html((curPrice*val).toFixed(1));
        $('.cart-sumtotal').html(sum());
    });

    $(document).on('click','.cart-selects-all',function () {
        if($(this).is(':checked')){
            $('.cart-selects').each(function () {
                console.log("全选");
                if(!$(this).is('checked')){
                    $(this).prop('checked',true);
                }
            })
        }else{
            console.log("全不选");
            $('.cart-selects').each(function () {
                $(this).prop('checked',false);
            })
        }
    });

    $(document).on('change','.cart-selects-all',function () {
        $('.cart-sumtotal').html(sum());
    });

    $(document).on('change','.cart-selects',function () {
        $('.cart-sumtotal').html(sum());
    });

    $(document).on('click','.item_remove',function () {
        let $cart = $('.shopping-cart');
        let param = $(this).attr('id');
        let removeId = param.split('_')[0];
        console.log("cart_table.jsp?removeId="+removeId);
        $cart.load("cart_table.jsp?removeId="+removeId);
    });

    $(document).on('click','.cart-delete-btn',function () {
        let params = "";
        let isfirst = true;
        $('.cart-selects').each(function () {
            if($(this).is(':checked')){
                if(isfirst){
                    params+=$(this).val();
                    isfirst = false;
                }else {
                    params+="-"+$(this).val();
                }
            }
        });

        $('.shopping-cart').load("cart_table.jsp?removeArray="+params);
    });

    /*      辅助方法    */
    //计算总价
    let sum = function () {
        let sum = 0;
        $('.cart-selects').each(function () {
            if($(this).is(':checked')){
                let thisTotalId = "#"+$(this).attr('id').split('_')[0]+"total";
                let thisTotal = ($(thisTotalId).html())*1;
                /*console.log(thisTotalId);*/
                sum+=thisTotal;
            }
        });
        return sum.toFixed(1);
    };

    let getGoodsId = function(){
        return $(this).attr('id').split('_')[0];
    };

    $.extend({
        getUrlParams : function () {
            //从url的?之后开始，对&进行分割,得到形如"key=val"的字符串数组
            let paramStrings = window.location.href.slice(window.location.href.indexOf('?')+1).split('&');
            let urlParams= [];
            let keyAndVal;
            for (let i = 0; i < paramStrings.length; i++) {
                //keyAndVal,[0]为key,[1]为val
                keyAndVal = paramStrings[i].split('=');
                urlParams.push(keyAndVal[0]);
                urlParams[keyAndVal[0]] = keyAndVal[1];
            }
            return urlParams;
        },
        getUrlParam : function (url) {
            console.log('调用成功');
            return $.getUrlParams()[url];
        }
    });


});
