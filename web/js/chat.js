var countID = 0;
$(document).ready(function () {
	var userName = JSON.parse(sessionStorage.getItem('userName'));
	console.log(userName);
	$('.internetName').html(userName);
	var toName = $('.headName').html();
	var x = $.ajax({
		url: '/web/getHistory',
		type: 'post',
		data: 'from=' + userName + '&to=' + toName,
		dataType: 'json',
		success: function (data) {

		}
	});
});

var isGettingMsg = false;

function addMsg(msg) {
	if (msg == '') {
		alert('不能为空');
	} else {
		$('#dope').val('');
		var str = '';
		str += '<li>' +
			'<div class="answerHead"><img src="img/tou.jpg"/></div>' +
			'<div class="answers"><img class="jiao" src="img/20170926103645_03_02.jpg">' + msg + '</div>' +
			'</li>';
		$('.newsList').append(str);
		$('.conLeft').find('li.bg').children('.liRight').children('.infor').text(msg);
		$('.RightCont').scrollTop($('.RightCont')[0].scrollHeight);
	}
}


$('.conLeft li').on('click', function () {
	$(this).addClass('bg').siblings().removeClass('bg');
	var intername = $(this).children('.liRight').children('.intername').text();
	$('.headName').text(intername);
	$('.newsList').html('');
});

$('.sendBtn').on('click', function () {
	var from_name = $('.internetName').html();
	var news = $('#dope').val();
	var to_name = $('.headName').html();
	var x = $.ajax({
		url: '/web/recvMsg',
		type: 'post',
		data: 'id=' + countID + '&from=' + from_name + '&msg=' + news + '&to=' + to_name,
		dataType: 'json',
		success: function (data) {
			console.log("isGettingMsg:"+isGettingMsg);
			countID = Number(data.id) + 1;
			console.log("countID=" + countID);
		}
	});
	if (!isGettingMsg) {
		getMsg();
	}
	// addMsg(news);
	// getMsg();

});

function answers(arr) {
	var answer = '';
	answer += '<li>' +
		'<div class="nesHead"><img src="img/6.jpg"/></div>' +
		'<div class="news"><img class="jiao" src="img/jiao.jpg">' + arr + '</div>' +
		'</li>';
	$('.newsList').append(answer);
	$('.RightCont').scrollTop($('.RightCont')[0].scrollHeight);
};

$('.ExP').on('mouseenter', function () {
	$('.emjon').show();
});

$('.emjon').on('mouseleave', function () {
	$('.emjon').hide();
});

$('.emjon li').on('click', function () {
	var imgSrc = $(this).children('img').attr('src');
	var str = "";
	str += '<li>' +
		'<div class="nesHead"><img src="img/6.jpg"/></div>' +
		'<div class="news"><img class="jiao" src="img/20170926103645_03_02.jpg"><img class="Expr" src="' + imgSrc + '"></div>' +
		'</li>';
	$('.newsList').append(str);
	$('.emjon').hide();
	$('.RightCont').scrollTop($('.RightCont')[0].scrollHeight);
});

/*	 定时拉取信息	 */
function getMsg() {
	// var from_name = $('.internetName').html();
	isGettingMsg = true;
	var to_name = $('.headName').html();
	var x = $.ajax({
		url: '/web/getMsg',
		timeout: 100000,
		type: 'post',
		data: 'to=' + to_name + '&id=' + countID,
		dataType: 'json',
		success: function (data) {
			console.log(data);
			$.each(data.msg, function (i, item) {
				if (item.from == $('.internetName').html()) {
					addMsg(item.msg);
				} else {
					answers(item.msg);
				}
				countID = Number(item.id);
			});
		},
		complete: function (status) {
			if (status.statusText = 'timeout') {
				alert("超时");
				isGettingMsg = false;
				// getMsg();
			}
		}
	});
	// setTimeout(2000);
}


getMsg();
// function test() {
// 	var x = $.ajax({
// 		url: '/web/aaaa',
// 		timeout: 1000,

// 	});
// }


// getMsg();
// function doGetMsg() {
// 	getMsg();
// 	window.setInterval("getMsg()", 12000);
// }
// doGetMsg();
