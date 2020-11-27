(function ($) {

    /*      加载动画    */
    $(window).on('load', function () {
        $(".loader").fadeOut();
        $("#preloader").delay(200).fadeOut();

        /*      data-setbg    */
        $('.set-bg').each(function () {
            let bg = $(this).data('setbg');
            $(this).css('background-image', 'url(' + bg + ')');
        });
    });

})(jQuery);
