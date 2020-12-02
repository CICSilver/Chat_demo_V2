$(document).ready(function () {

    /*      Form Check      */
    let nameisOkay = false;
    let emailisOkay = false;
    let passwdisOkay = false;
    let proAndCityisOkay = false;
    let phoneNumisOkay = false;
    let allIdOkay = false;
    //昵称,4-16位，不可以数字或特殊符号开头且不包括空格
    $('#name').blur(function () {
        let name = $('#name').val();
        let reg_name = /^[\u4E00-\u9FA5a-zA-Z_]+[^\s]*\w*$/;
        let $e_name = $('#e_name');
        $e_name.empty();
        if (name.length > 3 && reg_name.test(name) && name.length <= 16) {
            $e_name.append("<span class='okay'>用户名可用</span>");
            nameisOkay = true;
        } else {
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
        if (reg_email.test(email)) {
            $e_email.append("<span class='okay'>邮箱可用</span>");
            emailisOkay = true;
        } else {
            $e_email.append("<span class='error'>邮箱无效</span>");
            emailisOkay = false;
        }
    });

    $('#phoneNumber').blur(function () {
        let phoneNum = $('#phoneNumber').val();
        let reg_phoneNum = /^1([34578])\d{9}$/;
        let $e_phoneNum = $('#e_phoneNumber');
        $e_phoneNum.empty();
        if (!reg_phoneNum.test(phoneNum)) {
            $e_phoneNum.append("<span class='error'>请填写正确的手机号</span>");
            phoneNumisOkay = false;
        } else {
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
        if (passwd.length < 9) {
            $e_passwd.append("<span class='error'>请输入至少9位密码</span>");
            passwdisOkay = false;
        } else if (reg_passwd_ultra.test(passwd)) {
            $e_passwd.append("<span class='ultra'>您就是最强大脑？</span>");
            passwdisOkay = true;
        } else if (reg_passwd_strong.test(passwd)) {
            $e_passwd.append("<span class='okay'>强</span>");
            passwdisOkay = true;
        } else if (reg_passwd_middle.test(passwd)) {
            $e_passwd.append("<span class='middle'>中</span>");
            passwdisOkay = true;
        } else if (reg_passwd_week.test(passwd)) {
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
        if (passwd !== repeat) {
            $e_repeat.append("<span class='error'>两次输入的密码不相等</span>");
            passwdisOkay = false;
        } else {
            passwdisOkay = true;
        }
    });

    /*      Register        */
    $('#register').blur(function () {
        if ($("#province").val() !== "") {
            if ($("#city").val() !== "") {
                proAndCityisOkay = true;
            }
        }
        if (nameisOkay && emailisOkay && passwdisOkay && proAndCityisOkay && phoneNumisOkay) {
            console.log(1);
            allIdOkay = true;
        } else {
            alert("请按正确格式填写信息");
        }
    });

    $('#register').on('click', function () {
        if (nameisOkay && emailisOkay && passwdisOkay && proAndCityisOkay && phoneNumisOkay) {
            console.log(1);
            allIdOkay = true;
        } else {
            alert("请按正确格式填写信息");
        }
        if (allIdOkay) {
            let name = $('#name').val();
            let email = $('#email').val();
            let phoneNum = $('#phoneNumber').val();
            let passwd = $('#passwd').val();
            let province = $('#province').val();
            let city = $('#city').val();
            var x = $.ajax({
                url: "/web/reg",
                type: 'post',
                data: 'userName=' + name
                    + '&userEmail=' + email
                    + '&userPhone=' + phoneNum
                    + '&passwd=' + passwd
                    + '&province=' + province
                    + '&city=' + city,
                dataType: 'json',
                success: function () {
                    window.location.href = '/web/login.html';
                },
                error: function () {
                    alert("error...");
                }
            });
        } else {
            alert("请按正确格式填写信息");
        }
    });

    /*      Login        */
    // "id", "uid", "userName", "userEmail", "userPhone", "passwd", "province", "city"
    $('#login-btn').on('click', function () {
        var name = $('#email').val();
        var passwd = $('#passwd').val();
        var x = $.ajax({
            url: '/web/login',
            type: 'post',
            data: 'userEmail=' + name + '&passwd=' + passwd,
            dataType: 'json',
            success: function (data) {
                sessionStorage.setItem('userName', JSON.stringify(data.userName));
                sessionStorage.setItem('userEmail', JSON.stringify(data.userEmail));
                window.location.href = '/web/index.html';
            },
            error: function () {
                alert("账号或密码错误，请重试");
            }
        });
    })


    /*      辅助方法    */

    $.extend({
        getUrlParams: function () {
            //从url的?之后开始，对&进行分割,得到形如"key=val"的字符串数组
            let paramStrings = window.location.href.slice(window.location.href.indexOf('?') + 1).split('&');
            let urlParams = [];
            let keyAndVal;
            for (let i = 0; i < paramStrings.length; i++) {
                //keyAndVal,[0]为key,[1]为val
                keyAndVal = paramStrings[i].split('=');
                urlParams.push(keyAndVal[0]);
                urlParams[keyAndVal[0]] = keyAndVal[1];
            }
            return urlParams;
        },
        getUrlParam: function (url) {
            console.log('调用成功');
            return $.getUrlParams()[url];
        }
    });


});
