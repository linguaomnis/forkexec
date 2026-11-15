#!/bin/perl
package Hkr::Util;
use strict;
use warnings;
use utf8;
use Encode;
use Data::Dumper;
use JSON;
use Storable;
use HTTP::Request;
use URI::Split qw( uri_split uri_join );
use URI::Escape;
use HTML::Entities;
use CGI::Cookie;
use Izm::Util;
use Digest::MD5;

sub get_userinfo( $ )
{
    ( my $username ) = @_;

    my %userinfo = (
          "username"        => undef
        , "dispname"        => ""
        , "location"        => ""
        , "url"             => ""
        , "about"           => ""
        , "icon"    => ""
    );

    if( ! defined( $username ) )
    {
        return( %userinfo );
    }
    if( $username !~ m/^[0-9A-Za-z]{1,32}$/ )
    {
        return( %userinfo );
    }

    my $dispname = "";
    if( ! open( PIPE, "-|", "izmusr show-userinfo-dispname-by-username $username" ) )
    {
        return( %userinfo );
    }
    while( <PIPE> )
    {
        $dispname = $_;
        last;
    }
    close( PIPE );

    my $location = "";
    if( ! open( PIPE, "-|", "izmusr show-userinfo-location-by-username $username" ) )
    {
        return( %userinfo );
    }
    while( <PIPE> )
    {
        $location = $_;
        last;
    }
    close( PIPE );

    my $url = "";
    if( ! open( PIPE, "-|", "izmusr show-userinfo-url-by-username $username" ) )
    {
        return( %userinfo );
    }
    while( <PIPE> )
    {
        $url = $_;
        last;
    }
    close( PIPE );

    my $about = "";
    if( ! open( PIPE, "-|", "izmusr show-userinfo-about-by-username $username" ) )
    {
        return( %userinfo );
    }
    while( <PIPE> )
    {
        $about = $_;
        last;
    }
    close( PIPE );

    my $icon = "";
    if( ! open( PIPE, "-|", "izmusr show-userinfo-icon-by-username $username" ) )
    {
        return( %userinfo );
    }
    while( <PIPE> )
    {
        $icon = $_;
        last;
    }
    close( PIPE );

    $userinfo{"username"} = $username;
    $userinfo{"dispname"} = $dispname;
    $userinfo{"location"} = $location;
    $userinfo{"url"} = $url;
    $userinfo{"about"} = $about;
    $userinfo{"icon"} = $icon;

    return( %userinfo );
}

sub userauth_getpasswordhash( $ )
{
    ( my $password ) = @_;

    if( ! defined( $password ) )
    {
        return( undef );
    }
    if( $password !~ m/^[0-9A-Za-z]{1,32}$/ )
    {
        return( undef );
    }

    my $passwordhash = "";
    if( ! open( PIPE, "-|", "izmusr get-passwordhash $password" ) )
    {
        return( undef );
    }
    while( <PIPE> )
    {
        $passwordhash = $_;
        last;
    }
    close( PIPE );

    if( $passwordhash !~ m/^[0-9a-f]{32}$/ )
    {
        return( undef );
    }

    return( $passwordhash );
}

sub userauth_check_password( $ $ )
{
    ( my $username, my $password ) = @_;

    if( ! defined( $username ) )
    {
        return( 0 );
    }
    if( $username !~ m/^[0-9A-Za-z]{1,32}$/ )
    {
        return( 0 );
    }

    if( ! defined( $password ) )
    {
        return( 0 );
    }
    if( $password !~ m/^[0-9A-Za-z]{1,32}$/ )
    {
        return( 0 );
    }

    my $ret = system( "izmusr auth-by-username-password $username $password >/dev/null 2>/dev/null" );

    return( $ret == 0 ? 1 : 0 );
}

sub userauth_check_passwordhash( $ $ )
{
    ( my $username, my $passwordhash ) = @_;

    if( ! defined( $username ) )
    {
        return( 0 );
    }
    if( $username !~ m/^[0-9A-Za-z]{1,32}$/ )
    {
        return( 0 );
    }

    if( ! defined( $passwordhash ) )
    {
        return( 0 );
    }
    if( $passwordhash !~ m/^[0-9a-f]{32}$/ )
    {
        return( 0 );
    }

    my $ret = system( "izmusr auth-by-username-passwordhash $username $passwordhash >/dev/null 2>/dev/null" );

    return( $ret == 0 ? 1 : 0 );
}

sub userauth( $ )
{
    ( my $cgi_ref ) = @_;

    my $username = '';
    my $msg = '';

    if( defined( $$cgi_ref->param('userauth_username') ) && defined( $$cgi_ref->param('userauth_password') ) )
    {
        my $username_in = $$cgi_ref->param('userauth_username');
        my $password_in = $$cgi_ref->param('userauth_password');

        # username, password が送られてきた場合
        Izm::Util::debugprint( __FILE__, __LINE__,
            "Hkr::Util::userauth : received username, password via POST method : userauth_username[$username_in] userauth_password[$password_in]\n"
        );

        if( userauth_check_password( $username_in, $password_in ) )
        {
            # username に対して password が正しい場合
            # 「username」と「認証印（passwordのハッシュ）」をセットする
            Izm::Util::debugprint( __FILE__, __LINE__,
                "Hkr::Util::userauth : password is OK for username\n"
            );

            my $cookie1 = CGI::Cookie->new( -name => 'userauth_username', -value => $username_in );
            # $cookie1->bake;

            my $cookie2 = CGI::Cookie->new( -name => 'userauth_passwordhash', -value => userauth_getpasswordhash( $password_in ) );
            # $cookie2->bake;

            my $HTML = "";
            $HTML .= "Set-Cookie: $cookie1\n";
            $HTML .= "Set-Cookie: $cookie2\n";
            $HTML .= "Content-type: text/html\n\n";

            $HTML .= "
            <html>
                <head>
                    <meta http-equiv='Refresh' charset='utf-8' content='0; URL=" . $ENV{'REQUEST_URI'} . "' />
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

            Izm::Util::debugprint( __FILE__, __LINE__,
                "Hkr::Util::userauth : HTML[" . $HTML . "]\n"
            );

            print( $HTML );

            # 強制終了
            exit( 0 );
        }
        else
        {
            # username に対して password が正しくない場合
            # 不許可

            Izm::Util::debugprint( __FILE__, __LINE__,
                "Hkr::Util::userauth : password is NG for username\n"
            );

            $msg = 'username or password is incorrect';
            $username = '';
        }
    }
    elsif( defined( $$cgi_ref->cookie('userauth_username') ) && defined( $$cgi_ref->cookie('userauth_passwordhash') ) )
    {
        my $username_in = $$cgi_ref->cookie('userauth_username');
        my $passwordhash_in = $$cgi_ref->cookie('userauth_passwordhash');

        # username, passwordhash が設定済みの場合
        Izm::Util::debugprint( __FILE__, __LINE__,
            "Hkr::Util::userauth : received username, passwordhash from cookie\n"
        );

        if( userauth_check_passwordhash( $username_in, $passwordhash_in ) )
        {
            # username に対して passworhash が正しい場合
            # 許可
            Izm::Util::debugprint( __FILE__, __LINE__,
                "Hkr::Util::userauth : passworhash is OK for username\n"
            );

            $username = $username_in;
        }
        else
        {
            # username に対して passwordhash が正しくない場合
            # 不許可
            Izm::Util::debugprint( __FILE__, __LINE__,
                "Hkr::Util::userauth : passwordhash is NG for username\n"
            );

            $username = '';
        }
    }

    Izm::Util::debugprint( __FILE__, __LINE__,
        "Hkr::Util::userauth : username[$username]\n"
    );

    return( $username );
}

sub consension_r18 ( $ $ )
{
    ( my $cgi_ref ) = @_;

    if( defined( $cgi_ref->param('consension_r18_yes') ) )
    {
        # 「同意する」意志が送られてきた場合
        # 状態を「同意した」にして再アクセスさせる

        my $cookie = CGI::Cookie->new( -name => 'consension_r18', -value => 'yes' );
        $cookie->bake;
    
        print( "Content-type: text/html

        <html>
            <head>
                <meta http-equiv='Refresh' charset='utf-8' content='0; URL=" . $ENV{'REQUEST_URI'} . "' />
                <style>
                    body {
                        text-align: center;
                    }
                </style>
            </head>
            <body>
            </body>
        </html>
        " );

        # 強制終了
        exit( 0 );
    }
    elsif( defined( $cgi_ref->param('consension_r18_no') ) )
    {
        # 「同意しない」意志が送られてきた場合
        # google.com に飛ばす

        print( "Content-type: text/html

        <html>
            <head>
                <meta http-equiv='Refresh' charset='utf-8' content='3; URL=https://google.com' />
                <style>
                    body {
                        text-align: center;
                        margin: 2rem;
                    }
                </style>
            </head>
            <body>
                <h2>
                    Adieu.
                </h2>
            </body>
        <html>
        " );

        # 強制終了
        exit( 0 );
    }
    elsif( defined( $cgi_ref->cookie('consension_r18') ) && $cgi_ref->cookie('consension_r18') eq 'yes' )
    {
        # 「同意済み」状態である場合
        # 通過
    }
    else
    {
        # 未同意の場合
        # 同意／非同意回答フォームを表示する

        print( "Content-type: text/html

        <html>
            <head>
                <meta http-equiv='content-type' charset='utf-8'>
                <style>
                    input[type=submit] {
                        width: 12rem;
                        height: 6rem;
                        font-size: 3rem;
                    }
                    body {
                        text-align: center;
                        margin: 2rem;
                    }
                </style>
            </head>
            <body>
                <h2>
                    This site has some sensible contents like sexual, violence, etc.
                </h2>
        
                <h2>
                    Are you enter on your own responsibility?
                </h2>
        
                <form action='' method='post'>
                    <input type='submit' name='consension_r18_yes' value='yes' width='48' height='48' />
                </form>
        
                <form action='' method='post'>
                    <input type='submit' name='consension_r18_no' value='no' />
                </form>
            </head>
        </html>
        " );

        # 強制終了
        exit( 0 );
    }

    return( 1 );
}


1;
