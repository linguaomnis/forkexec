#!/usr/bin/perl
use strict;
use warnings;
use utf8;
use Encode;
use integer;
use Data::Dumper;
use HTTP::Request;
use URI::Split qw( uri_split uri_join );
use URI::Escape;
use HTML::Entities;
use POSIX;
use CGI;
use CGI::Cookie;
use Time::Local;
use Date::Parse;
use Hkr::Util;
use Izm::Util;

my $cgi = CGI->new;
my $base_url = $ENV{'HTTP_HOST'};
my $confpath = "$ENV{'FORKEXEC'}/etc/hkrflv.${base_url}.conf";
my $HEAD = "";
my $BODY = "";
my %get;
my %post;
my %conf;
my %cookies;
my $username_me;
my $page = 0;
my $posttext = "";
my $postid = 0;

( my $startup_sec, my $startup_min, my $startup_hour
, my $startup_mday, my $startup_month, my $startup_year
, my $startup_wday, my $startup_yday, my $startup_isdst )
    = localtime;
$startup_year += 1900;
$startup_month += 1;

sub _conf( $ )
{
    ( my $_confpath ) = @_;
    my %_conf;
    open( FH, "<" . $_confpath ) or return( %_conf );

    while( my $_line = <FH> ) {
        $_line =~ s/^\s*|\s*$|#.*//g;
        $_line !~ /^\s*$/ or next;

        my @_def = split( /\s+/, $_line );
        scalar( @_def ) == 2 or next;

        $_conf{ $_def[0] } = $_def[1];

        $_conf{ $_def[0] } =~ s/%space%/ /g;
    }

    return( %_conf );
}

sub _get()
{
    my %_get_raw = ();

    # for my $_pair_string( split( '&', uri_unescape( $ENV{'QUERY_STRING'} ) ) ) {
    #     my @_pair = split( '=', $_pair_string );
    #     $_get_raw{ $_pair[0] } = $_pair[1];
    # }

    for my $_pair_string( split( '&', $ENV{'QUERY_STRING'} ) ) {
        ( my $key, my $value ) = split( '=', $_pair_string );
        $key =~ s/\+/ /g;
        $key = uri_unescape( $key );
        $value =~ s/\+/ /g;
        $value = uri_unescape( $value );
    
        $_get_raw{ $key } = $value;
    }

    return( %_get_raw );
}

sub _post()
{
    my %_post_raw = ();

    if( defined( $ENV{'CONTENT_LENGTH'} ) && $ENV{'CONTENT_LENGTH'} =~ /[0-9]/ )
    {
        read( STDIN, my $_post_query, $ENV{'CONTENT_LENGTH'} );

        # for my $_pair_string( split( '&', uri_unescape( $_post_query ) ) ) {
        #     my @_pair = split( '=', $_pair_string );
        #     $_post_raw{ $_pair[0] } = $_pair[1];
        # }

        for my $_pair_string( split( '&', $_post_query ) ) {
            ( my $key, my $value ) = split( '=', $_pair_string );
            $key =~ s/\+/ /g;
            $key = uri_unescape( $key );
            $value =~ s/\+/ /g;
            $value = uri_unescape( $value );
            $_post_raw{ $key } = $value;
        }
    }

    return( %_post_raw );
}

sub _twitter_button( $ $ )
{
    ( my $text, my $url ) = @_;
    # <a href="https://twitter.com/share?ref_src=twsrc%5Etfw" class="twitter-share-button" data-text="また今度ね - chez Loco L&#39;omnis" data-url="https://blog.forkexec.net?article=202205292330" data-dnt="true" data-show-count="false">Tweet</a><script async src="https://platform.twitter.com/widgets.js" charset="utf-8"></script>
    return ( "<a href='https://twitter.com/share?ref_src=twsrc%5Etfw' class='twitter-share-button' data-text='" . encode_entities( Encode::decode_utf8( $text ) ) . "' data-url='" . encode_entities( Encode::decode_utf8( $url ) ) . "' data-dnt='true' data-show-count='false'>Tweet</a><script async src='https://platform.twitter.com/widgets.js' charset='utf-8'></script>" );
}

sub _get_send_post_media_sensitive ()
{
    my $send_post_media_sensitive = $cgi->param('send_post_media_sensitive');
    if( not defined( $send_post_media_sensitive ) ) {
        return( 0 );
    }
    Izm::Util::debugprint( __FILE__, __LINE__,
        "hkrflvindex.cgi : UPLOAD : send_post_media_sensitive : $send_post_media_sensitive\n"
    );

    return( 1 );
}

sub _get_posttext ()
{
    my $posttext = $cgi->param('send_post');
    if( not defined( $posttext ) ) {
        return( '' );
    }
    Izm::Util::debugprint( __FILE__, __LINE__,
        "hkrflvindex.cgi : UPLOAD : posttext : $posttext\n"
    );

    return( $posttext );
}

sub _get_deletepostid ()
{
    my $postid = $cgi->param('delete_post');
    if( not defined( $postid ) ) {
        return( '' );
    }
    # $postid =~ s/[^0-9]/ /g;
    if( not $postid =~ /^[0-9]{17}$/ ) {
        return( '' );    
    }
    Izm::Util::debugprint( __FILE__, __LINE__,
        "hkrflvindex.cgi : DELETE : postid : $postid\n"
    );

    return( $postid );
}

sub _save_media()
{
    my $mediaid = '';
    my $fh = $cgi->param('upload-file');
    if( not defined( $fh ) ) {
        return( '' );
    }
    my $info = $cgi->uploadInfo( $fh );
    if( not defined( $info ) ) {
        return( '' );
    }
    my $contenttype = $info->{'Content-Type'};
    my $ext = 'png';
    if( $contenttype eq 'image/png' ) {
        $ext = 'png';
    }elsif( $contenttype eq 'image/jpeg' ) {
        $ext = 'jpg';
    }else{
        Izm::Util::debugprint( __FILE__, __LINE__,
            "hkrflvindex.cgi : UPLOAD : invalid content-type : $contenttype\n"
        );
        return( '' );
    }
    
    # 20260113234002.png
    $mediaid = sprintf( "%04d%02d%02d%02d%02d%02d"
                                , $startup_year, $startup_month, $startup_mday
                                , $startup_hour, $startup_min, $startup_sec );

    my $savefilename = sprintf( "%s.%s", $mediaid, $ext );
    my $savefilepath_rel = $conf{'media_relpath'} . "/" . $savefilename;
    my $savefilepath_abs = $conf{'media_rootpath'} . "/" . $savefilepath_rel;

    Izm::Util::debugprint( __FILE__, __LINE__,
        "hkrflvindex.cgi : UPLOAD : Content-Type[$contenttype] : save to $savefilepath_abs\n"
    );
    open( SAVEFH, "> $savefilepath_abs" );
    print SAVEFH <$fh>;
    close( SAVEFH );
    close( $fh );

    my $command = "$conf{'occ_path'} files:scan --path=$savefilepath_rel >/dev/null 2>/dev/null";

    my $exitstatus = system( $command );
    Izm::Util::debugprint( __FILE__, __LINE__,
        "hkrflvindex.cgi : UPLOAD : command[$command] : exitstatus[$exitstatus]\n"
    );

    return( $mediaid );
}

sub _get_datetimestr( $ $ )
{
    ( my $date_in, my $time_in ) = @_;
    # 2025-08-14 11:31:55.129
    my $str = "";

    ( my $date_in_year, my $date_in_month, my $date_in_mday )
        = split( /[ .:-]+/, $date_in );
    ( my $time_in_hour, my $time_in_min, my $time_in_sec, my $time_in_msec )
        = split( /[ .:-]+/, $time_in );

    my $epoch_target = timelocal( $time_in_sec, $time_in_min, $time_in_hour, $date_in_mday, $date_in_month - 1, $date_in_year );
    # my $epoch_now = time;
    my $epoch_now = timelocal( $startup_sec, $startup_min, $startup_hour, $startup_mday, $startup_month - 1, $startup_year );

    my $epoch_diff = $epoch_now - $epoch_target;
    my $str_relative = "";

    if( abs($epoch_diff) < (60) )
    {
        # epoch_diff ... 1分未満
        # XX秒
        $str_relative = $epoch_diff;
        $str_relative .= "s";
    }
    elsif( abs($epoch_diff) < (60 * 60) )
    {
        # $epoch_diff ... 1時間未満
        # XX分
        $str_relative = ( $epoch_diff / (60) );
        $str_relative .= "m";
    }
    elsif( abs($epoch_diff) < (60 * 60 * 24) )
    {
        # $epoch_diff ... 1日未満
        # XX時間
        $str_relative = ( $epoch_diff / (60 * 60) );
        $str_relative .= "h";
    }
    elsif( abs($epoch_diff) < (60 * 60 * 24 * 7) )
    {
        # $epoch_diff ... 1週間未満
        # XX日
        $str_relative = ( $epoch_diff / (60 * 60 * 24) );
        $str_relative .= "d";
        # $str_relative .= " ($date_in_year-$date_in_month-$date_in_mday $time_in_hour:$time_in_min)";
    }
    elsif( abs($epoch_diff) < (60 * 60 * 24 * 7 * 5) )
    {
        # $epoch_diff ... 5週間未満
        # XX週間
        $str_relative = ( $epoch_diff / (60 * 60 * 24 * 7) );
        $str_relative .= "spt";
        # $str_relative .= " ($date_in_year-$date_in_month-$date_in_mday $time_in_hour:$time_in_min)";
    }
    elsif( abs($epoch_diff) < (60 * 60 * 24 * 366) )
    {
        # epoch_diff ... 366日未満
        # XX月
        $str_relative = ( $epoch_diff / (60 * 60 * 24 * 7 * 4) );
        $str_relative .= "mes";
        # $str_relative .= " ($date_in_year-$date_in_month-$date_in_mday $time_in_hour:$time_in_min)";
    }
    else
    {
        # XX年
        $str_relative = ( $epoch_diff / (60 * 60 * 24 * 365) );
        $str_relative .= "ann";
        # $str_relative .= " ($date_in_year-$date_in_month-$date_in_mday $time_in_hour:$time_in_min)";
    }

    if( $epoch_diff < 0 )
    {
        $str_relative = "+" . $str_relative . " futur";
    }
    elsif( 0 < $epoch_diff )
    {
        # nop
    }
    else
    {
        $str_relative = "nunc";
    }

    $str = $str_relative;

    # $str = $time_in;
    # $str =~ s/\.[0-9]+$//g; # 表示上、秒未満は除去する
    # $str = $date_in . " " . $str;

    # if( $str_relative ne "" )
    # {
    #     $str = $str . " ($str_relative)";
    # }


    return( $str );
}

sub _parse_mediaidlist( $ )
{
    ( my $mediaidlist_in ) = @_;
    my @mediaidlist = split( /,/, $mediaidlist_in );
    return( @mediaidlist );
}

sub _parse_text( $ )
{
    ( my $text_in ) = @_;
    my $text = $text_in;
    $text =~ s/\\n/ <br>/g;
    $text =~ s/(https?:\/\/\S+)/<a href='$1'>$1<\/a>/g;
    $text =~ s/>(https?:\/\/[^<]{32})[^<]*</>$1...</g;
    $text =~ s/\@([0-9A-Za-z]+)/<a href='?user=$1'>\@$1<\/a>/g;
    return( $text );
}

sub _make_postblock(
    $ $ $ $ $ $ $
    $ $ $ $ $
) {
    ( my $date_in, my $time_in, my $username_in, my $mediaidlist_in, my $pin_in, my $sensitive_in, my $text_in
        , my $is_media_detail, my $is_show_ctrl, my $is_datetime_abs, my $is_post_link, my $is_show_pin
    ) = @_;

    my $block = "";

    my $text = _parse_text( $text_in );
    my @mediaidlist = _parse_mediaidlist( $mediaidlist_in );


    # Izm::Util::debugprint( __FILE__, __LINE__,
    #     "hkrflvindex.cgi :"
    #     . " date_in[$date_in] time_in[$time_in] username_in[$username_in] mediaidlist_in[$mediaidlist_in] pin_in[$pin_in] text[$text]"
    #     . " is_media_detail[$is_media_detail]"
    #     . " is_show_ctrl[$is_show_ctrl]"
    #     . " is_datetime_abs[$is_datetime_abs]"
    #     . " is_post_link[$is_post_link]"
    #     . " is_show_pin[$is_show_pin]"
    #     . "\n"
    # );

    # 2025-08-14 11:31:55.129
    my $datetime_str = $is_datetime_abs ? ($date_in . " " . $time_in) : _get_datetimestr( $date_in, $time_in );

    my $postid = $date_in . $time_in;
    $postid =~ s/[^0-9]//g;

    my %userinfo = Hkr::Util::get_userinfo( $username_in );

    my $icon_imgsrc = sprintf( $conf{'media_url'}, $userinfo{'icon'}, "w100" );

    $block .= "
        <div class='post'>
            <div class='post-icon'>
                <a href='?user=" . $userinfo{'username'} . "'>
                    <img src='https://" . $icon_imgsrc . "'>
                </a>
            </div>
            <div class='post-main'>
                <div class='post-head'>
                    <span class='post-username'>" . $userinfo{'username'} . "</span>
                    <span class='post-username'>\@" . $username_in . "</span>
                    " . Encode::encode_utf8("·") . "
                    <span class='post-time'>
                        " . (($is_post_link) ? ("<a href='?post=" . $postid . "'>" . $datetime_str . "</a>") : ($datetime_str)) . "
                    </span>
                    " . (($is_show_pin and $pin_in eq "pin") ? ("<span class='post-pin'>" . Encode::encode_utf8("⚲<i>POSTE-FIXE") . "</i></span>") : ("")) . "
                </div>
                <div class='post-text' translate='no'>
                    " . $text . "
                </div>
                <div class='". (($is_media_detail) ? ("post-media-magna") : ("post-media")) . (($sensitive_in eq "sensitive") ? (" sensitive") : ("")) . "'>
    ";
    for my $mediaid ( @mediaidlist ) {
        if( not $mediaid =~ /^[0-9]{14}/ ) {
            next
        }
        my $url_link = sprintf( $conf{'media_url'}, $mediaid, "original" );
        my $url_disp = sprintf( $conf{'media_url'}, $mediaid, ($is_media_detail) ? ("w500") : ("w100") );

        $block .= "
                    <a href='https://$url_link'><img src='https://$url_disp'></a>
        ";
    }
    $block .= "
                </div>
    ";

    if( $is_show_ctrl and ($username_me ne '') and ($username_me eq $username_in) )
    {
        $block .= "
                <hr>
                <div class='post-ctrl'>
                    <span class='deleteform'>

                        <form action='' method='post' enctype='multipart/form-data'>
                            <textarea name='delete_post' readonly style='display:none'>" . $postid . "</textarea>
                            <input type='submit' name='DELETE' value='delete'/>
                        </form>

                        <!--
                        <form action='' method='post' enctype='multipart/form-data'>
                            <textarea name='send_post' placeholder='What is new?' autofocus rows='5' cols='30'></textarea>
                            <input type='file' name='upload-file'/>
                            <input type='submit' value='post'/>
                        </form>
                        -->

                    </span>
                </div>
        ";
    }

    $block .= "
            </div>
        </div>
    ";

    return( $block );
}

sub _make_streamtabblock ( $ $ $ )
{
    ( my $username, my $page, my $nowselect ) = @_;
    my $body = "";
    my $style_nowselect = "style='background-color: #353a2c'";

    $body .= "
        <div class='stream-tab'>
    ";

    if( $nowselect eq "post" )
    {
        $body .= "
            <div class='stream-tab-post' " . $style_nowselect . ">
                Post
            </div>
        ";
    }
    else
    {
        $body .= "
            <a href='?page=${page}&user=${username}'>
                <div class='stream-tab-post'>
                    Post
                </div>
            </a>
        ";
    }

    if( $nowselect eq "media" )
    {
        $body .= "
            <div class='stream-tab-media' " . $style_nowselect . ">
                Media
            </div>
        ";
    }
    else
    {
        $body .= "
            <a href='?page=${page}&user=${username}&media'>
                <div class='stream-tab-media'>
                    Media
                </div>
            </a>
        ";
    }

    $body .= "
        </div>
    ";

    return( $body );
}

sub _make_navigatorblock ( $ $ $ )
{
    ( my $page, my $username, my $is_mediaonly ) = @_;
    my $body = "";
    my $str_prev = $conf{'prev_str'};
    my $str_next = $conf{'next_str'};

    my $url_prae = "?page=" . ($page - 1) . (($username =~ /^[0-1a-zA-Z_-]+$/) ? "&user=$username" : "") . ($is_mediaonly ? "&media" : "");
    my $url_post = "?page=" . ($page + 1) . (($username =~ /^[0-1a-zA-Z_-]+$/) ? "&user=$username" : "") . ($is_mediaonly ? "&media" : "");

    $body .= "
            <div class='navigator'>
    ";

    $body .= "
                " . ((0 <= $page - 1) ? "<a href='". $url_prae . "'>" . $str_prev . "</a>" : "" . $str_prev . "") . "
                " . Encode::encode_utf8("·") . "
                <a href='". $url_post . "'>". $str_next . "</a>
    ";

    $body .= "
            </div>
    ";

    return( $body );
}

sub _make_body_post( $ )
{
    ( my $postid ) = @_;
    my $body = "";

    # if( not $postid =~ /^[0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\.[0-9]{3}$/ )
    if( not $postid =~ /^[0-9]{17}$/ )
    {
        $body = "
            <div class='whole'>
                NUL MSG TROVATUM
            </div>
        ";

        return( $body );
    }

    my $date_search_year    = substr( $postid,  0, 4 );
    my $date_search_month   = substr( $postid,  4, 2 );
    my $date_search_mday    = substr( $postid,  6, 2 );
    my $time_search_hour    = substr( $postid,  8, 2 );
    my $time_search_min     = substr( $postid, 10, 2 );
    my $time_search_sec     = substr( $postid, 12, 2 );
    my $time_search_msec    = substr( $postid, 14, 3 );

    my $date_search = "$date_search_year-$date_search_month-$date_search_mday";
    my $time_search = "$time_search_hour:$time_search_min:$time_search_sec.$time_search_msec";

    $body .= "
    <div class='whole'>
        <div class='left'>
    ";
    
    $body .= "
        </div>
        
        <div class='middle'>
            <div class='middle-header'>
                <div class='credit'>
                    <a href='/'>" . $base_url . "</a>
                </div>
            </div>
            <hr>

            <div class='stream'>
    ";

    open( FILE, "awk '\$1 == \"$date_search\" && \$2 == \"$time_search\" {print}' $conf{'stream_path'} | head -n1 |" ) or die( "ERROR" );
    if( 1 )
    {
        Izm::Util::debugprint( __FILE__, __LINE__,
            "hkrflvindex.cgi : stream 読み込み成功！\n"
        );
    
        while( my $line = <FILE> )
        {
            # 2026-02-07 11:35:01 linguaomnis Re-lancerò la mie societas minimalis. Solum facta via mei ipsae posso credergli.
            # 2026-02-07 11:20:02 linguaomnis non jam posso equi stare...
            # 2026-02-06 08:34:03 linguaomnis Spartacus / Jan Van der Roost
            # 2026-02-06 08:33:04 linguaomnis invocatiòn et tuccata
            # 2026-02-02 10:50:05 linguaomnis Hoc camera vero tranquillis est. Nulli stulti sonaci.
    
            # Izm::Util::debugprint( __FILE__, __LINE__,
            #     "hkrflvindex.cgi : line[$line]\n"
            # );
    
            ( my $date_in, my $time_in, my $username_in
                , my $mediaidlist_in, my $pin_in, my $sensitive_in, my $reserved4
                , my $reserved5, my $reserved6, my $reserved7, my $reserved8
                , my $text_in ) = split( /\s/, $line, 12 );
    
            $body .= _make_postblock( $date_in, $time_in, $username_in, $mediaidlist_in, $pin_in, $sensitive_in, $text_in
                        , 1, 1, 1, 0, 0 );
    
            last;
        }
        close( FILE );
    }
    
    $body .= "
            </div>
    
            <div class='middle-footer'>
            </div>
        </div>
    
        <div class='right'>
        </div>
    </div>
    ";

    return( $body );
}

sub _make_body_fluvius( $ $ )
{
    ( my $username, my $is_mediaonly ) = @_;

    my %userinfo_me = Hkr::Util::get_userinfo( $username_me );
    my %userinfo = Hkr::Util::get_userinfo( $username );
    my $body = "";

    $body .= "
    <div class='whole'>
        <div class='left'>
    ";
    
    
    $body .= "
        </div>
        
        <div class='middle'>
            <div class='middle-header'>
                <div class='credit'>
                    <a href='/'>" . $base_url . "</a>
                </div>
            </div>
            <hr>
    ";

    if( defined( $username ) && $username =~ /^[a-zA-Z0-9_-]+$/ )
    {
        my $about = $userinfo{'about'};
        $about =~ s/(https?:\/\/\S+)/<a href='$1'>$1<\/a>/g;
        $about =~ s/\@([0-9A-Za-z]+)/<a href='?user=$1'>\@$1<\/a>/g;

        my $icon_imgsrc = sprintf( $conf{'media_url'}, $userinfo{'icon'}, "w100" );

        $body .= "
            <div class='user'>
                <div class='user-icon'>
                    <img src='https://" . $icon_imgsrc . "'>
                </div>
                <div class='user-info'>
                    <div class='user-username'>
                        " . $userinfo{'username'} . "
                    </div>
                    <div class='user-username'>
                        @" . $userinfo{'username'} . "
                    </div>
                    <div class='user-about'>
                        " . $about . "
                    </div>
                    <div class='user-info-other'>
                        <span class='user-location'>
                            " . Encode::encode_utf8("📍") . $userinfo{'location'} . "
                        </span>
                        <span class='user-link'>
                            " . Encode::encode_utf8("🔗") . "<a href='" . $userinfo{'url'} . "'>" . $userinfo{'url'} . "</a>
                        </span>
                    </div>
                </div>
            </div>
        ";
    }
    
    if( $username_me ne '' )
    {
        my $icon_me_imgsrc = sprintf( $conf{'media_url'}, $userinfo_me{'icon'}, "w100" );

        $body .= "
            <div class='postmaking'>
                <div class='postmaking-icon'>
                    <a href='?user=${username_me}'>
                        <img src='https://" . $icon_me_imgsrc . "'>
                    </a>
                </div>
                <div class='postmaking-form'>
                    <form action='' method='post' enctype='multipart/form-data'>
                        <textarea name='send_post' placeholder='$conf{'postmakingform_placeholder'}' autofocus rows='5' cols='30'>"
                            . ((defined($username) and $username =~ /^[a-zA-Z0-9_-]+$/ and $username_me ne $username) ? ("@" . $username) : "") .
                        "</textarea>
                        <input type='file' name='upload-file'/>
                        <input type='checkbox' id='sensitive' name='send_post_media_sensitive' /><label for='sensitive'>media sensitivi</label>
                        <input type='submit' value='post'/>
                    </form>
                </div>
            </div>
        ";
    }
    
    if( defined( $username ) && $username =~ /^[a-zA-Z0-9_-]+$/ )
    {
        $body .= _make_streamtabblock( $username, 0, (exists($get{'media'}) ? "media" : "post") );
    }
    else
    {
        $body .= _make_navigatorblock( $page, $username, $is_mediaonly );
    }

    $body .= "
            <div class='stream'>
    ";
    
    Izm::Util::debugprint( __FILE__, __LINE__,
        "hkrflvindex.cgi : stream を読み込みます\n"
    );
    
    my $range = 50;
    my $begin = $range * $page + 1;
    my $end   = $begin + $range - 1;

    my $cmd = "";

    if( defined( $username ) and $username =~ /^[a-zA-Z0-9_-]+$/ )
    {
        # ユーザページ
        # 指定ユーザのみ、固定ポストから優先表示
        $cmd .= "(tac $conf{'stream_path'} | awk '\$3 == \"$username\" && \$5 == \"pin\" {print}'; tac $conf{'stream_path'} | awk '\$3 == \"$username\" && \$5 != \"pin\" {print}';)";
    }
    else
    {
        # 共有TL
        # 全ユーザ
        $cmd .= "tac $conf{'stream_path'} | awk '{print}'";
    }

    if( defined( $is_mediaonly ) && $is_mediaonly )
    {
        $cmd .= " | awk '\$4 != \"-\" {print}'";
    }

    $cmd .= " | sed -n '" . $begin . "," . $end . "p' |";

    Izm::Util::debugprint( __FILE__, __LINE__,
        "hkrflvindex.cgi : cmd[$cmd]\n"
    );
    
    open( FILE, $cmd ) or die( "ERROR" );
    if( 1 )
    {
        Izm::Util::debugprint( __FILE__, __LINE__,
            "hkrflvindex.cgi : stream 読み込み成功！\n"
        );
    
        while( my $line = <FILE> )
        {
            # 2026-02-07 11:35:01 linguaomnis Re-lancerò la mie societas minimalis. Solum facta via mei ipsae posso credergli.
            # 2026-02-07 11:20:02 linguaomnis non jam posso equi stare...
            # 2026-02-06 08:34:03 linguaomnis Spartacus / Jan Van der Roost
            # 2026-02-06 08:33:04 linguaomnis invocatiòn et tuccata
            # 2026-02-02 10:50:05 linguaomnis Hoc camera vero tranquillis est. Nulli stulti sonaci.
    
            # Izm::Util::debugprint( __FILE__, __LINE__,
            #     "hkrflvindex.cgi : line[$line]\n"
            # );
    
            ( my $date_in, my $time_in, my $username_in
                , my $mediaidlist_in, my $pin_in, my $sensitive_in, my $reserved4
                , my $reserved5, my $reserved6, my $reserved7, my $reserved8
                , my $text_in ) = split( /\s/, $line, 12 );
    
            $body .= _make_postblock($date_in, $time_in, $username_in, $mediaidlist_in, $pin_in, $sensitive_in, $text_in
                            , (defined( $username ) and $username =~ /^[a-zA-Z0-9_-]+$/ and exists($get{'media'})) # $is_media_detail
                            , 0 # $is_show_ctrl
                            , 0 # $is_datetime_abs
                            , 1 # $is_post_link
                            , (defined( $username ) and $username =~ /^[a-zA-Z0-9_-]+$/) # $is_show_pin
            );
    
        }
        close( FILE );
    }
        
    
    $body .= "
            </div>
    ";

    $body .= _make_navigatorblock( $page, $username, $is_mediaonly );
    

    $body .= "
            <hr>
    
            <div class='middle-footer'>
    ";
    
    if( $username_me eq '' )
    {
        $body .= "
                <div class='loginform'>
                    <form action='' method='post'>
                        <input type='text' name='userauth_username' placeholder='uesrid'/>
                        <input type='password' name='userauth_password' placeholder='password'/>
                        <input type='submit' value='login'/>
                    </form>
                </div>
        ";
    }
    
    $body .= "
    
                <div class='credit'>
                    <a href='/'>" . $base_url . "</a>
                </div>
            </div>
        </div>
    
        <div class='right'>
        </div>
    </div>
    ";

    return( $body );
}


################################################################
# MAIN
################################################################

if( not -w $confpath )
{
    if( open( FILE, "> /tmp/hkrflv_${base_url}_log" ) )
    {
        printf FILE ( "error : not found : \$confpath[${confpath}]\n" );
    }
    close( FILE );
    exit( 0 ); 
}

%conf = _conf( $confpath );

if(
       not exists( $conf{'stylesheet_path'} )
    or not exists( $conf{'stream_path'} )
    or not exists( $conf{'log_flag'} )
    or not exists( $conf{'log_maxsize'} )
    or not exists( $conf{'log_path'} )
    or not exists( $conf{'occ_path'} )
    or not exists( $conf{'media_rootpath'} )
    or not exists( $conf{'media_relpath'} )
    or not exists( $conf{'prev_str'} )
    or not exists( $conf{'next_str'} )
    or not exists( $conf{'postmakingform_placeholder'} )
) {
    if( open( FILE, "> /tmp/hkrflv_${base_url}_log" ) )
    {
        printf FILE ( "error : conf read failed : \$confpath[${confpath}]\n" );
    }
    close( FILE );
    exit( 0 ); 
}

if( $conf{'log_flag'} == 1 )
{
    system( "touch $conf{'log_path'}" );
    if( not -w $conf{'log_path'} )
    {
        if( open( FILE, "> /tmp/hkrflv_${base_url}_log" ) )
        {
            printf FILE ( "error : log unavailable : \$log_path[$conf{'log_path'}]\n" );
        }
        close( FILE );
        exit( 0 ); 
    }
    
    Izm::Util::setdebuglogfilepath( $conf{'log_path'} );
    Izm::Util::setdebuglogfilemaxsize( $conf{'log_maxsize'} );
    Izm::Util::debugflagon();
}
else
{
    Izm::Util::debugflagoff();
}

system( "touch $conf{'stream_path'}" );
if( not -w $conf{'stream_path'} )
{
    Izm::Util::debugprint( __FILE__, __LINE__,
        "hkrflvindex.cgi : error : stream unavailable :\$stream_path[$conf{'stream_path'}\n"
    );
    exit( 0 ); 
}



Izm::Util::debugprint( __FILE__, __LINE__,
    "hkrflvindex.cgi : BEGIN\n"
);

%get = _get();
Izm::Util::debugprint( __FILE__, __LINE__,
    "hkrflvindex.cgi : %get :\n" . Dumper( %get )
);

%post = _post();
Izm::Util::debugprint( __FILE__, __LINE__,
    "hkrflvindex.cgi : %post :\n" . Dumper( %post )
);

%cookies = CGI::Cookie->fetch;
Izm::Util::debugprint( __FILE__, __LINE__,
    "hkrflvindex.cgi : %cookies :\n" . Dumper( %cookies )
);

$username_me = Hkr::Util::userauth( \$cgi );
Izm::Util::debugprint( __FILE__, __LINE__,
    "hkrflvindex.cgi : \$username_me :\n" . Dumper( $username_me )
);


$page = 0;
if( exists( $get{'page'} ) && $get{'page'} =~ /^[0-9]+$/ )
{
    $page = $get{'page'};
    $page =~ s/^0+//g;
    if( $page eq '' )
    {
        $page = 0;
    }
}


$posttext = _get_posttext();
if( $posttext ne '' && $username_me ne '' )
{
    Izm::Util::debugprint( __FILE__, __LINE__,
        "hkrflvindex.cgi : send_post が送られてきた場合 posttext[$posttext]\n"
    );

    $posttext =~ s/\r\n/\\n/g;
    $posttext =~ s/[\r\n]/\\n/g;
    $posttext =~ s/\t/ /g;
    $posttext =~ s/^\s+|\s+$//g;

    my $mediaid = _save_media();

    my $media_sensitive = _get_send_post_media_sensitive();
    
    Izm::Util::debugprint( __FILE__, __LINE__,
        "hkrflvindex.cgi : mediaid[$mediaid] media_sensitive[$media_sensitive] posttext[$posttext]\n"
    );

    my $msec = 0;
    ( my $sec, my $min, my $hour, my $mday, my $month, my $year, my $wday, my $yday, my $isdst )
        = localtime;
    $year += 1900;
    $month += 1;

    if( open( FILE, ">> " . $conf{'stream_path'} ) )
    {
        printf FILE ( "%04d-%02d-%02d %02d:%02d:%02d.%03d %s %s %s %s %s %s %s %s %s %s\n"
                , $year, $month, $mday, $hour, $min, $sec, $msec
                , $username_me
                , ($mediaid =~ /^[0-9]{14}$/ ? $mediaid : "-")
                , "-" #pin
                , ($media_sensitive ? "sensitive" : "-") #sensitive
                , "-"
                , "-"
                , "-"
                , "-"
                , "-"
                , $posttext
        );
        close( FILE );

        Izm::Util::debugprint( __FILE__, __LINE__,
            "hkrflvindex.cgi : posttext write ok\n"
        );
    }
    else
    {
        Izm::Util::debugprint( __FILE__, __LINE__,
            "hkrflvindex.cgi : stream file open failed\n"
        );
    }

    my $HTML = "Content-type: text/html; charset=utf-8

    <!doctype html>
    <html>
        <head>
            <meta http-equiv='Refresh' charset='utf-8' content='0; URL=\"/\"' />
            <style>
                body {
                    text-align: center;
                }
            </style>
        </head>
        <body>
        </body>
    </html>
    ";

    print ( $HTML );

    Izm::Util::debugprint( __FILE__, __LINE__,
        "hkrflvindex.cgi : HTML[" . $HTML . "]\n"
    );

    # 強制終了
    exit( 0 );
}

$postid = _get_deletepostid();
if( $postid ne '' && $username_me ne '' )
{
    # 2025-08-14 07:03:00.979

    Izm::Util::debugprint( __FILE__, __LINE__,
        "hkrflvindex.cgi : delete_post が送られてきた場合 postid[$postid]\n"
    );

    my $year    = substr( $postid,  0, 4 );
    my $month   = substr( $postid,  4, 2 );
    my $mday    = substr( $postid,  6, 2 );
    my $hour    = substr( $postid,  8, 2 );
    my $min     = substr( $postid, 10, 2 );
    my $sec     = substr( $postid, 12, 2 );
    my $msec    = substr( $postid, 14, 3 );

    my $command = sprintf(
        ">/dev/null 2>/dev/null sed -E '/^%s-%s-%s %s:%s:%s\\.%s %s/d' -i $conf{'stream_path'}"
        , $year, $month, $mday, $hour, $min, $sec, $msec, $username_me
    );

    Izm::Util::debugprint( __FILE__, __LINE__,
        "hkrflvindex.cgi : command[$command]\n"
    );

    system( $command );

    my $HTML = "Content-type: text/html; charset=utf-8

    <!doctype html>
    <html>
        <head>
            <meta http-equiv='Refresh' charset='utf-8' content='0; URL=\"/\"' />
            <style>
                body {
                    text-align: center;
                }
            </style>
        </head>
        <body>
        </body>
    </html>
    ";

    print ( $HTML );

    Izm::Util::debugprint( __FILE__, __LINE__,
        "hkrflvindex.cgi : HTML[" . $HTML . "]\n"
    );

    # 強制終了
    exit( 0 );
}



$HEAD .= "
<meta http-equiv='content-type' charset='utf-8'>
<style>
";

if( open( FILE, "<" . $conf{'stylesheet_path'} ) )
{
    while( my $line = <FILE> ) {
        $HEAD .= $line;
    }
    close( FILE );
}

$HEAD .= "
</style>
";



if( exists( $get{'post'} ) )
{
    $BODY .= _make_body_post( $get{'post'} );
}
else
{
    $BODY .= _make_body_fluvius(
                exists($get{'user'}) ? $get{'user'} : ""
            , exists($get{'media'}) ? 1 : 0 );
}


my $HTML = "Content-type: text/html; charset=utf-8

<!doctype html>
<html translate='no'>
    <head>
        " . $HEAD . "
    </head>
    <body>
        " . $BODY . "
    </body>
</html>
";

print( $HTML );

exit( 404 );














