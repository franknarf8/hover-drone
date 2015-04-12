gamepadSupport.init();

$( document ).ready(function() {
    $('#menu-opener').mouseenter(function() {
        $('#menu').show();
    });
    $('#menu-wrapper').mouseleave(function() {
        $('#menu').hide();
    });

    $('#menu li').click(function() {
        var cssClass = $(this).attr('class'),
            $check = $(this).find('.check');

        if( $check.hasClass('checked') ) {
            $check.html('<i class="fa fa-square-o"></i>');
            $check.removeClass('checked');
            $(".location-pin." + cssClass).hide();
        } else {
            $check.html('<i class="fa fa-check-square-o"></i>');
            $check.addClass('checked');
            $(".location-pin." + cssClass).show();
        }

    });

    $("#drone").click(function() {
        var offset = $(this).offset();

        $('#stats').offset({
            'left': offset.left + 120,
            'top': offset.top + 30
        });
        $('#stats').show(200, function() {
            $('#drone').animate({
                'marginTop': "-=200px"
            }, 4000);
            $('#stats').animate({
                'marginTop': "-=200px"
            }, 4000, function() {
                $('#stats').find('.first').hide(200, function() {
                    $(this).remove();
                    $('#stats').find('.second').find('.task-icon').html('<i class="fa fa-spinner"></i>');
                    setTimeout(function() {
                        $('#drone').animate({
                            'marginTop': "-=100px"
                        }, 3000);
                        $('#stats').animate({
                            'marginTop': "-=100px"
                        }, 3000);
                    }, 400);
                });
            });
        });
    });
});