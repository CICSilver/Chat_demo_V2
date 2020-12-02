function login() {
    var name = $('#name-input').val();
    var passwd = $('#passwd-input').val();
    var t = $("#name-lable").html();
    console.log(t);
    var x = $.ajax({
            url:'/web/login',
            type: 'post',
            data:'name='+name+'&passwd='+passwd,
            dataType:'json',
            success: function (data) {
                var item = data.name;

            },
            error:function()
            {
                alert("error...");
            }
        });
}