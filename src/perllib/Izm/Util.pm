#!/bin/perl
package Izm::Util;
use strict;
use warnings;
use Data::Dumper;
use JSON;
use Storable;
use utf8;
use Encode;

our $debuglogfilepath = "/tmp/perl.log";
our $debuglogfilemaxsize = 10000;
our $debugflag = 0;

sub setdebuglogfilepath( $ )
{
    ( my $debuglogfilepath_in ) = @_;
    $Izm::Util::debuglogfilepath = $debuglogfilepath_in;
    return;
}

sub setdebuglogfilemaxsize( $ )
{
    ( my $debuglogfilemaxsize_in ) = @_;
    $Izm::Util::debuglogfilemaxsize = $debuglogfilemaxsize_in;
    return;
}

sub debugflagon()
{
    $Izm::Util::debugflag = 1;
    return;
}

sub debugflagoff()
{
    $Izm::Util::debugflag = 0;
    return;
}

sub debugprint( $ $ $ )
{
    $Izm::Util::debugflag or return(0);

    ( my $file, my $line, my $str ) = @_;

    ( my $sec, my $min, my $hour, my $day, my $month, my $year )
        = localtime;

    if(
        ( -f $Izm::Util::debuglogfilepath )
        and
        ( ( -s $Izm::Util::debuglogfilepath ) > $debuglogfilemaxsize )
    ) {
        rename( $Izm::Util::debuglogfilepath, $Izm::Util::debuglogfilepath . ".old" );
    }

    open( FILE, ">> $Izm::Util::debuglogfilepath" ) or return(0);
    printf FILE ( "%04d-%02d-%02d %02d:%02d:%02d %s:%s %s"
            , $year, $month, $day, $hour, $min, $sec
            , $file, $line
            , $str
    );
    close( FILE );

    return(1);
}


1;
