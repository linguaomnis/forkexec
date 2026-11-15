#!/usr/bin/perl
use strict;
use warnings;
use utf8;
use Encode;
use Data::Dumper;
use HTTP::Request;
use URI::Split qw( uri_split uri_join );
use URI::Escape;
use HTML::Entities;
use POSIX;

my $base_url = $ENV{'HTTP_HOST'};
my $toolpath = "$ENV{'FORKEXEC'}/bin";
my $confpath = "$ENV{'FORKEXEC'}/etc/hkrblg.${base_url}.conf";
my $HTML = "Content-type: text/html\n\n";
my $HEAD = "";
my $BODY = "";
my $debug = 0;
my $DEBUG = "";

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

    for my $_pair_string( split( '&', uri_unescape( $ENV{'QUERY_STRING'} ) ) ) {
        my @_pair = split( '=', $_pair_string );
        $_get_raw{ $_pair[0] } = $_pair[1];
    }
    return( %_get_raw );
}

sub _post()
{
    my %_post_raw = ();

    if( defined( $ENV{'CONTENT_LENGTH'} ) && $ENV{'CONTENT_LENGTH'} =~ /[0-9]/ )
    {
        read( STDIN, my $_post_query, $ENV{'CONTENT_LENGTH'} );
        for my $_pair_string( split( '&', uri_unescape( $_post_query ) ) ) {
            my @_pair = split( '=', $_pair_string );
            $_post_raw{ $_pair[0] } = $_pair[1];
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

$ENV{'PATH'} = "$ENV{'PATH'}:$toolpath";

my %get = _get();
# $DEBUG .= Dumper( %get );

my %post = _post();
# $DEBUG .= Dumper( %post );

my %conf = _conf( $confpath );
# $DEBUG .= "<pre>" . Dumper( \%conf ) . "</pre>";

my %ogp;
$ogp{'title'}       = $conf{'title'};
$ogp{'description'} = $conf{'description'};
$ogp{'url'}         = $base_url;
$ogp{'type'}        = "website";
$ogp{'image'}       = sprintf( $conf{'image_url'}, $conf{'ogp_image_default_mediaid'}, $conf{'ogp_image_size'} );
$ogp{'twitter'}     = $conf{'ogp_twitter'};

my $title       = exists( $conf{'title'}       ) ? encode_entities( decode_utf8( $conf{'title'}       ) ) : "";
my $description = exists( $conf{'description'} ) ? encode_entities( decode_utf8( $conf{'description'} ) ) : "";
my $notice      = exists( $conf{'notice'}      ) ? encode_entities( decode_utf8( $conf{'notice'}      ) ) : "";

$BODY .= "
    <div class='kanban_head'>
        <div class='title'><a href='/'>${title}</a></div>
        <div class='description'>${description}</div>
        <div class='notice'>${notice}</div>
    </div>
";

my $page_min = 0;
my $page_max = 0;
my $page = 0;
exists( $get{'pagina'} )
    and $get{'pagina'} =~ /^([0-9]|[1-9][0-9]+)$/
    and $page = $get{'pagina'};

my $cmd = "find $conf{'cache_dir'}"
        . ' -maxdepth 1'
        . ' -type f'
        . ' -regextype posix-extended'
        . ' -regex ^.*\/[0-9]{12}.html$'
        . ' | sort -r'
        . ' |';

my $count = 0;

open( FH, "$cmd" ) or die;
while( <FH> ) {

    my $line = $_;
    $line =~ s/\n//g;

    if( ! ( $line =~ /\/[0-9]{12}\.html$/ ) )
    {
        next;
    }

    my $article_path = $line;
    my $article_page = POSIX::floor( $count / $conf{'article_per_page'} );
    my $article_index_per_page = $count % $conf{'article_per_page'};
    my $article_basename = $article_path;
        $article_basename =~ s/^.*\/([0-9]{12})\.html$/$1/g;

    if( ! exists( $get{'articolo'} ) ) {
        # page
        ! ($article_basename lt "197001010000") or next;
    }

    $count++;

    my $year   = substr( $article_basename,  0,  4 );
    my $month  = substr( $article_basename,  4,  2 );
    my $day    = substr( $article_basename,  6,  2 );
    my $hour   = substr( $article_basename,  8,  2 );
    my $minute = substr( $article_basename, 10,  2 );

    $page_max = $article_page;

    if( ! exists( $get{'articolo'} ) ) {
        # page
        $article_page == $page or next;
    }else{
        # article
        $article_basename eq $get{'articolo'} or next;
    }

    my $article_title   = `awk '\$2 == "!title" { printf( gensub( "<!-- !title[[:space:]]*|[[:space:]]*-->", "", "g" ) ); }' ${article_path}`;
    $article_title = $article_title eq "" ? "no title" : $article_title;

    my $article_thumb   = `awk '\$2 == "!thumb" { printf( gensub( "<!-- !thumb[[:space:]]*|[[:space:]]*-->", "", "g" ) ); }' ${article_path}`;
    $article_thumb = $article_thumb eq "" ? $conf{'ogp_image_default_mediaid'} : $article_thumb;

    my $article_body    = `cat $article_path`;

    $BODY .= "
        <div class='article' id='" . $article_basename . "'>
            <div class='article_head'>
                <div class='article_time'>
                    " . $year . "-" . $month . "-" . $day . "
                </div>
                <div class='article_title'>
                    <a href='/?articolo=" . $article_basename . "'>" . $article_title . "</a>
                </div>
            </div>
            <div class='article_body'>
    ";
    $BODY .= $article_body;
    $BODY .= "
            </div>
            <div class='article_foot'>
    "; 
    if( $conf{'twitter_button'} eq "true" ) {
        $BODY .= _twitter_button( $article_title . " - " . $conf{'title'}, $base_url . "/?article=" . $article_basename );
    }
    $BODY .= "
            </div>
        </div>
    ";

    if( exists( $get{'articolo'} ) ) {
        $ogp{'title'}       = $article_title . " - " . $conf{'title'};
        $ogp{'url'}         = $base_url . "/?articolo=" . $article_basename;
        $ogp{'description'} = $article_title;
        $ogp{'type'}        = "article";
        $ogp{'image'}       = sprintf( $conf{'image_url'}, $article_thumb, $conf{'ogp_image_size'} );
    }
}
close( FH );

if( ! exists( $get{'articolo'} ) ) {
    # page

    $BODY .= "
        <div class='nav'>
    ";
    # if( $page_min < $page ){
    #     my $page_prev = $page + 1;
    #     $BODY .= "<a href='/?page=" . $page_prev . "'>Prev page</a>";
    # }
    # }# for( my $nav_page = $page_min; $nav_page <= $page_max; $nav_page++ ) {
    #     if( $nav_page == $page ) {
    #         $BODY .= " " . $nav_page;
    #     }else{
    #         $BODY .= " <a href='/?page=" . $nav_page . "'>" . $nav_page . "</a>";
    #     }
    # }
    if( $page < $page_max ){
        my $page_next = $page + 1;
        # $BODY .= "<a href='/?pagina=" . $page_next . "'>Secunde Pagina</a>";
        $BODY .= "<a href='/?pagina=" . $page_next . "'>" . $conf{'str_nextpage'} . "</a>";
    }$BODY .= "
        </div>
    ";
}

$BODY .= "
    <div class='kanban_foot'>
        <a href='/'>" . $conf{'title'} . "</a>
    </div>
";

$HEAD .= "<meta http-equiv='content-type' charset='utf-8'>";
$HEAD .= "<title>" . encode_entities( Encode::decode_utf8( $conf{'title'} )) . "</title>";
$HEAD .= "<meta name='robots' content='" . encode_entities( Encode::decode_utf8( $conf{'robots'} )) . "'>";
$HEAD .= "<meta name='keywords' content='" . encode_entities( Encode::decode_utf8( $conf{'keywords'} )) . "'>";
$HEAD .= "<meta name='description' content='" . encode_entities( Encode::decode_utf8( $conf{'description'} )) . "'>";
$HEAD .= "<link href='https://fonts.googleapis.com/css2?family=Noto+Sans&amp;display=swap' rel='stylesheet'>";
# $HEAD .= "<link href='https://fonts.googleapis.com/css2?family=Noto+Sans+JP&amp;display=swap' rel='stylesheet'>";
# $HEAD .= "<link href='https://fonts.googleapis.com/css2?family=Noto+Sans+SC&amp;display=swap' rel='stylesheet'>";
# $HEAD .= "<link href='https://fonts.googleapis.com/css2?family=Noto+Sans+KR&amp;display=swap' rel='stylesheet'>";
# $HEAD .= "<link href='https://fonts.googleapis.com/css2?family=Noto+Serif&amp;display=swap' rel='stylesheet'>";
# $HEAD .= "<link href='https://fonts.googleapis.com/css2?family=Noto+Serif+JP&amp;display=swap' rel='stylesheet'>";
# $HEAD .= "<link href='https://fonts.googleapis.com/css2?family=Noto+Serif+SC&amp;display=swap' rel='stylesheet'>";
# $HEAD .= "<link href='https://fonts.googleapis.com/css2?family=Noto+Serif+KR&amp;display=swap' rel='stylesheet'>";
$HEAD .= "<link rel='stylesheet' href='style.css'>";
$HEAD .= "<meta name='viewport' content='width=device-width,initial-scale=1'>";
$HEAD .= "<meta property='og:url' content='" . encode_entities( Encode::decode_utf8( $ogp{'url'} )) . "'>";
$HEAD .= "<meta property='og:title' content='" . encode_entities( Encode::decode_utf8( $ogp{'title'} )) . "'>";
$HEAD .= "<meta property='og:description' content='" . encode_entities( Encode::decode_utf8( $ogp{'description'} )) . "'>";
$HEAD .= "<meta property='og:type' content='" . encode_entities( Encode::decode_utf8( $ogp{'type'} )) . "'>";
$HEAD .= "<meta property='og:image' content='" . encode_entities( Encode::decode_utf8( $ogp{'image'} )) . "'>";
$HEAD .= "<meta name='twitter:card' content='summary_large_image'>";
$HEAD .= "<meta name='twitter:site' content='" . encode_entities( Encode::decode_utf8( $ogp{'twitter'} )) . "'>";
$HEAD .= "<meta name='twitter:creator' content='" . encode_entities( Encode::decode_utf8( $ogp{'twitter'} )) . "'>";
$HEAD .= "<meta name='twitter:title' content='" . encode_entities( Encode::decode_utf8( $ogp{'title'} )) . "'>";
$HEAD .= "<meta name='twitter:description' content='" . encode_entities( Encode::decode_utf8( $ogp{'description'} )) . "'>";
$HEAD .= "<meta name='twitter:image' content='" . encode_entities( Encode::decode_utf8( $ogp{'image'} )) . "'>";

if( $debug == 1 )
{
    $BODY = "<div class='debug'>$DEBUG</div>" . $BODY;
}

$HTML .= "
<html>
    <head>
        " . $HEAD . "
    </head>
    <body>
        " . $BODY . "
    </body>
</html>
";

print( $HTML );

exit( 0 );
