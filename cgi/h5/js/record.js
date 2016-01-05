/**
 * Created by fangwenjuan on 2016/1/4/0004.
 */
$(function(){
    var parameters = getRequest();
    if(!parameters['record_id']){
        $('.error').show();
        $('.error').text('网络连接失败');
        $('.loading').hide();
        $('.all-content').hide();
        return;
    }
    getDetail(parameters['record_id']);
    getComment(parameters['record_id']);

});

//获取url参数
function getRequest() {
    var url = location.search;
    var parameters = new Object();
    if (url.indexOf("?") != -1) {
        var str = url.substr(1);
        var strs = str.split("&");
        for (var i = 0; i < strs.length; i++) {
            parameters[strs[i].split("=")[0]] = strs[i].split("=")[1];
        }
    }
    return parameters;
}

function getDetail(record_id){
    var loading = $('.loading');
    $('.error').hide();
    loading.show();
    $.ajax({
        url: 'http://182.254.220.116/cgi-bin/record.cgi',
        data: {action: 'query_detail', id: record_id},
        type: 'post',
        success: function (data) {
            loading.hide();
            data = $.parseJSON(data);
            if (data.code == 0) {
                var user = data.data.user;
                if (user != null) {
                    $('.avatar').attr('src',user.head);
                    $('.nickname').text(user.nickname);
                }
                var record = data.data.record;
                if (record != null) {
                    $('.weather').attr('src', record.weather);
                    $('.mood').attr('src', record.mood);
                    $('.status').attr('src', record.status);
                    $('.content').text(record.text);
                    $('.picture').empty();
                    var pictures = record.pictures;
                    if(pictures != null){
                        var html = '';
                        for(i= 0 ;i <pictures.length;i++){
                            html+= '<div class="img"><img src="'+pictures[i]+'"></div>';
                        }
                        $('.picture').append(html);
                    }
                    $('.address').text(record.address);
                    var time = new Date(record.time*1000);
                    $('.date').text(time.getFullYear()+'-'+(time.getMonth()+1)+'-'+time.getDate());

                    var interact = data.data.interact;
                    if(interact != null){
                        $('.view').text(interact.view);
                        $('.like').text(interact.like);
                        $('.comment').text(interact.comment);
                    }
                }
            }
        },
        error: function () {
            loading.hide();
            $('.error').show();
            $('.all-content').empty();
        }
    });
}

function getComment(record_id){
    var loading = $('.loading');
    $('.error').hide();
    loading.show();
    $.ajax({
        url:'http://182.254.220.116/cgi-bin/interact.cgi',
        data:{action:'record',record_id:record_id},
        type:'post',
        success: function (data) {
            loading.hide();
            data = $.parseJSON(data);
            if(data.code == 0){
                //评论
                var comments = data.data.comments;
                $('.comments').empty();
                var html = '';
                for(i = 0;i<comments.length;i++){
                    var comment = comments[i];
                    var interact = comment.interact;
                    var user = comment.user;
                    if(interact.target_nickname != null){
                        html+= '<li><span class="green">'+user.nickname+'</span>回复<span class="green">'+interact.target_nickname+'</span>：<span>'+interact.text+'</span></li>';
                    }else{
                        html+= '<li><span class="green">'+user.nickname+'</span>：<span>'+interact.text+'</span></li>';
                    }
                }
                $('.comments').append(html);
                //点赞
                var likes = data.data.likes;
                if(likes.length > 0){
                    var html = '';
                    for(i = 0;i<likes.length;i++){
                        var user = likes[i].user;
                        html += '<span class="green">'+user.nickname+'</span>';
                        if(i>0 && i<likes.length-2){
                            html+=',';
                        }
                    }
                    $('.likes').append(html);
                }else{
                    $('.likes').hide();
                }
            }
        },
        error:function(error){
            loading.hide();
            $('.error').show();
            $('.all-content').empty();
        }
    });
}

$('.error').click(function(){
    getDetail();
    getComment();
});