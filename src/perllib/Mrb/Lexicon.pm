#!/bin/perl
package Mrb::Lexicon;
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
use YAML;



sub new
{
    my $self = {};

    $self->{'lexicon_dpath'} = (exists($_->{'lexicon_dpath'}) ?
                    $_->{'lexicon_dpath'} : "$ENV{'FORKEXEC'}/var/lib/mrb/lexicon");

    bless( $self );
    return( $self );
}


sub load_word
{
    ( my $self, my $lemma ) = @_;

    my $ref_word = {
        'lemma' => $lemma,
        'info' => undef,
        'html' => undef
    };

    # printf STDERR ( "DEBUG : lexicon_dpath[" . $self->{'lexicon_dpath'} . "]\n" );
    # printf STDERR ( "DEBUG : lemma[" . $lemma . "]\n" );

    my $lexicon_fpath = $self->{'lexicon_dpath'} . "/" . ${lemma};

    if( not -r $lexicon_fpath )
    {
        return( undef );
    }

    $ref_word->{'lemma'} = $lemma;
    ( $ref_word->{'info'}, $ref_word->{'html'} ) = YAML::LoadFile( $lexicon_fpath );

    return( $ref_word );
}

sub load_allwords
{
    ( my $self ) = @_;

    my $ref_word = {};
    my @ref_word_list = ();

    for my $lemma (glob( $self->{'lexicon_dpath'} . "/*" ))
    {
        $lemma = Encode::decode_utf8( $lemma );
        $lemma =~ s/^.*\///g;
        # printf STDERR ( "DEBUG : lemma[" . $lemma . "]\n" );

        $ref_word = load_word( $self, $lemma );
        if( defined( $ref_word ) )
        {
            push( @ref_word_list, $ref_word );
        }
    }

    return( @ref_word_list );
}

sub get_root_declension
{
    ( my $self, my $lemma, my $subform_sg_obl ) = @_;

    my $root = "";
    if( $subform_sg_obl eq "-" )
    {
        $root = $lemma;
    }
    elsif( $subform_sg_obl =~ /^-\S+/ )
    {
        $root = $lemma;
        $root =~ s/(a|us|um|is|e)$//g;
    }
    else
    {
        $root = $subform_sg_obl;
        $root =~ s/(o|e)$//g;
    }

    return( $root );
}

sub get_form_by_root_and_subform
{
    ( my $self, my $root, my $subform ) = @_;
    my $form = "";

    if( $subform eq "-" )
    {
        $form = $root;
    }
    elsif( $subform =~ /^-\S+/ )
    {
        $form = $subform;
        $form =~ s/^-//g;
        $form = $root . $form;
    }
    else
    {
        $form = $subform;
    }

    return( $form );
}

sub get_declension_nom
{
    ( my $self, my $lemma, my $gender, my $ref_subform ) = @_;

    # $ref_category->{'subform'}->{'sg_obl'};
    # $ref_category->{'subform'}->{'pl_rct'};
    # $ref_category->{'subform'}->{'pl_obl'};

    my $ref_declension = {
        "sg" => { "rct" => undef, "obl" => undef },
        "pl" => { "rct" => undef, "obl" => undef }
    };

    $ref_declension->{'sg'}->{'rct'} = $lemma;

    my $root = get_root_declension( $self, $lemma, $ref_subform->{'sg_obl'} );

    $ref_declension->{'sg'}->{'obl'} = get_form_by_root_and_subform( $self, $root, $ref_subform->{'sg_obl'} );
    $ref_declension->{'pl'}->{'rct'} = get_form_by_root_and_subform( $self, $root, $ref_subform->{'pl_rct'} );
    $ref_declension->{'pl'}->{'obl'} = get_form_by_root_and_subform( $self, $root, $ref_subform->{'pl_obl'} );

    return( $ref_declension );
}

sub get_declension_adj
{
    ( my $self, my $lemma, my $ref_subform ) = @_;

    # $ref_category->{'subform'}->{'m_sg_obl'};
    # $ref_category->{'subform'}->{'m_pl_rct'};
    # $ref_category->{'subform'}->{'m_pl_obl'};

    my $ref_declension = {
        "m" => {
            "sg" => { "rct" => undef, "obl" => undef },
            "pl" => { "rct" => undef, "obl" => undef }
        },
        "f" => {
            "sg" => { "rct" => undef, "obl" => undef },
            "pl" => { "rct" => undef, "obl" => undef }
        }
    };

    $ref_declension->{'m'}->{'sg'}->{'rct'} = $lemma;

    my $root = get_root_declension( $self, $lemma, $ref_subform->{'m_sg_obl'} );

    $ref_declension->{'m'}->{'sg'}->{'obl'} = get_form_by_root_and_subform( $self, $root, $ref_subform->{'m_sg_obl'} );
    $ref_declension->{'m'}->{'pl'}->{'rct'} = get_form_by_root_and_subform( $self, $root, $ref_subform->{'m_pl_rct'} );
    $ref_declension->{'m'}->{'pl'}->{'obl'} = get_form_by_root_and_subform( $self, $root, $ref_subform->{'m_pl_obl'} );

    if( $ref_subform->{'m_sg_obl'} =~ /o$/ )
    {
        $ref_declension->{'f'}->{'sg'}->{'rct'} = get_form_by_root_and_subform( $self, $root, "-a"  );
        $ref_declension->{'f'}->{'sg'}->{'obl'} = get_form_by_root_and_subform( $self, $root, "-aé" );
        $ref_declension->{'f'}->{'pl'}->{'rct'} = get_form_by_root_and_subform( $self, $root, "-aé" );
        $ref_declension->{'f'}->{'pl'}->{'obl'} = get_form_by_root_and_subform( $self, $root, "-i"  );
    }
    else
    {
        $ref_declension->{'f'}->{'sg'}->{'rct'} = $ref_declension->{'m'}->{'sg'}->{'rct'};
        $ref_declension->{'f'}->{'sg'}->{'obl'} = $ref_declension->{'m'}->{'sg'}->{'obl'};
        $ref_declension->{'f'}->{'pl'}->{'rct'} = $ref_declension->{'m'}->{'pl'}->{'rct'};
        $ref_declension->{'f'}->{'pl'}->{'obl'} = $ref_declension->{'m'}->{'pl'}->{'obl'};
    }

    return( $ref_declension );
}

sub get_conjugation_verb
{
    ( my $self, my $lemma, my $ref_subform ) = @_;

    # $ref_category->{'subform'}->{'fin_dir_pres_sg_1st'};
    # $ref_category->{'subform'}->{'fin_dir_perf_sg_1st'};
    # $ref_category->{'subform'}->{'part_perf_m_sg_rct'};

    my $ref_conjugation = {
        "inf" => {
            "inf" => undef,
            "gerund" => {
                "sg" => { "rct" => undef, "obl" => undef },
                "pl" => { "rct" => undef, "obl" => undef }
            },
            "part" => {
                "pres" => {
                    "m" => { "sg" => { "rct" => undef, "obl" => undef },
                             "pl" => { "rct" => undef, "obl" => undef } },
                    "f" => { "sg" => { "rct" => undef, "obl" => undef },
                             "pl" => { "rct" => undef, "obl" => undef } }
                },
                "perf" => {
                    "m" => { "sg" => { "rct" => undef, "obl" => undef },
                             "pl" => { "rct" => undef, "obl" => undef } },
                    "f" => { "sg" => { "rct" => undef, "obl" => undef },
                             "pl" => { "rct" => undef, "obl" => undef } }
                },
                "prtr" => {
                    "m" => { "sg" => { "rct" => undef, "obl" => undef },
                             "pl" => { "rct" => undef, "obl" => undef } },
                    "f" => { "sg" => { "rct" => undef, "obl" => undef },
                             "pl" => { "rct" => undef, "obl" => undef } }
                }
            }
        },
        "fin" => {
            "dir" => {
                "pres" => {
                    "sg" => { "1st" => undef, "2nd" => undef, "3rd" => undef },
                    "pl" => { "1st" => undef, "2nd" => undef, "3rd" => undef }
                },
                "pass" => {
                    "sg" => { "1st" => undef, "2nd" => undef, "3rd" => undef },
                    "pl" => { "1st" => undef, "2nd" => undef, "3rd" => undef }
                },
                "prtr" => {
                    "sg" => { "1st" => undef, "2nd" => undef, "3rd" => undef },
                    "pl" => { "1st" => undef, "2nd" => undef, "3rd" => undef }
                },
                "futr" => {
                    "sg" => { "1st" => undef, "2nd" => undef, "3rd" => undef },
                    "pl" => { "1st" => undef, "2nd" => undef, "3rd" => undef }
                }
            },
            "sub" => {
                "pres" => {
                    "sg" => { "1st" => undef, "2nd" => undef, "3rd" => undef },
                    "pl" => { "1st" => undef, "2nd" => undef, "3rd" => undef }
                },
                "futr" => {
                    "sg" => { "1st" => undef, "2nd" => undef, "3rd" => undef },
                    "pl" => { "1st" => undef, "2nd" => undef, "3rd" => undef }
                }
            },
            "con" => {
                "pres" => {
                    "sg" => { "1st" => undef, "2nd" => undef, "3rd" => undef },
                    "pl" => { "1st" => undef, "2nd" => undef, "3rd" => undef }
                }
            },
            "imp" => {
                "pres" => {
                    "sg" => { "2nd" => undef },
                    "pl" => { "2nd" => undef }
                }
            }
        }
    };

    ################################################################
    # inf => inf
    ################################################################
    $ref_conjugation->{'inf'}->{'inf'} = $lemma;

    my $inf_root = $ref_conjugation->{'inf'}->{'inf'};
    $inf_root =~ s/[aei]r$//g;
    my $inf_root2 = $inf_root . (($ref_conjugation->{'inf'}->{'inf'} =~ /ar$/) ? "a" : "e");

    ################################################################
    # inf => gerund
    ################################################################
    my $gerund_lemma = $inf_root2 . "nde";

    $ref_conjugation->{'inf'}->{'gerund'}
        = get_declension_nom(
            $self,
            $gerund_lemma,
            'n',
            { "sg_obl" => "-o", "pl_rct" => "-i", "pl_obl" => "-i" }
        );

    ################################################################
    # inf => part => pres
    ################################################################
    my $part_pres_lemma   = $inf_root2 . "ns";
    my $part_pres_subform = $inf_root2 . "nte";

    $ref_conjugation->{'inf'}->{'part'}->{'pres'}
        = get_declension_adj(
            $self,
            $part_pres_lemma,
            { "m_sg_obl" => $part_pres_subform, "m_pl_rct" => "-i", "m_pl_obl" => "-i" }
        );

    ################################################################
    # inf => part => perf
    ################################################################
    # $ref_category->{'subform'}->{'part_perf_m_sg_rct'};

    # my $part_perf_lemma = $ref_subform->{'part_perf_m_sg_rct'};

    # if( $part_perf_lemma =~ /^-\S+/ )
    # {
    #     $part_perf_lemma =~ s/^-//g;
    #     $part_perf_lemma = $inf_root2 . $part_perf_lemma;
    # }

    # $ref_conjugation->{'inf'}->{'part'}->{'perf'}
    #     = get_declension_adj(
    #         $self,
    #         $part_perf_lemma,
    #         { "m_sg_obl" => "-o", "m_pl_rct" => "-i", "m_pl_obl" => "-i" }
    #     );

    my $part_perf_lemma = $ref_subform->{'part_perf_root'};
    $part_perf_lemma =~ s/-$//g;
    $part_perf_lemma = $part_perf_lemma . "e";

    $ref_conjugation->{'inf'}->{'part'}->{'perf'}
        = get_declension_adj(
            $self,
            $part_perf_lemma,
            { "m_sg_obl" => "-o", "m_pl_rct" => "-i", "m_pl_obl" => "-i" }
        );

    ################################################################
    # inf => part => prtr
    ################################################################
    my $part_prtr_lemma = $inf_root2 . "nde";

    $ref_conjugation->{'inf'}->{'part'}->{'prtr'}
        = get_declension_adj(
            $self,
            $part_prtr_lemma,
            { "m_sg_obl" => "-o", "m_pl_rct" => "-i", "m_pl_obl" => "-i" }
        );

    ################################################################
    # fin => dir => pres
    ################################################################
    $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'1st'} = $ref_subform->{'fin_dir_pres_sg_1st'};
    $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'1st'} = $ref_subform->{'fin_dir_pres_sg_1st'};
    $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'1st'} =~ s/^-//g;
    $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'1st'} = $inf_root . $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'1st'};

    if( $lemma eq 'esser' )
    {
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'1st'} = "soi";
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'2nd'} = "es";
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'3rd'} = "est, e";
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'pl'}->{'1st'} = "somos";
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'pl'}->{'2nd'} = "estes";
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'pl'}->{'3rd'} = "sont";
    }elsif( $lemma eq 'haber' ){
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'1st'} = "ho";
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'2nd'} = "has";
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'3rd'} = "hat";
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'pl'}->{'1st'} = $inf_root2 . "mos";
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'pl'}->{'2nd'} = $inf_root2 . "tes";
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'pl'}->{'3rd'} = $inf_root2 . "nt";
    }else{
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'2nd'} = $inf_root2 . "s";
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'3rd'} = $inf_root2 . "t";
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'pl'}->{'1st'} = $inf_root2 . "mos";
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'pl'}->{'2nd'} = $inf_root2 . "tes";
        $ref_conjugation->{'fin'}->{'dir'}->{'pres'}->{'pl'}->{'3rd'} = $inf_root2 . "nt";
    }

    ################################################################
    # fin => dir => pass
    ################################################################
    if( $ref_subform->{'fin_dir_perf_sg_1st'} =~ /^-/ )
    {
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'sg'}->{'1st'} = $ref_subform->{'fin_dir_perf_sg_1st'};
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'sg'}->{'1st'} =~ s/^-//g;
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'sg'}->{'1st'} = $inf_root2 . $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'sg'}->{'1st'};
    }
    else
    {
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'sg'}->{'1st'} = $ref_subform->{'fin_dir_perf_sg_1st'};
    }

    my $fin_dir_pass_root = $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'sg'}->{'1st'};
    $fin_dir_pass_root =~ s/i$//g;

    if( $lemma eq 'esser' )
    {
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'sg'}->{'1st'} = "fui";
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'sg'}->{'2nd'} = "fuis";
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'sg'}->{'3rd'} = "fuò";
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'pl'}->{'1st'} = "fuimos";
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'pl'}->{'2nd'} = "fuites";
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'pl'}->{'3rd'} = "fueront";
    }else{
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'sg'}->{'2nd'} = $fin_dir_pass_root . "is";
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'sg'}->{'3rd'} = $fin_dir_pass_root . "ò";
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'pl'}->{'1st'} = $fin_dir_pass_root . "imos";
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'pl'}->{'2nd'} = $fin_dir_pass_root . "ites";
        $ref_conjugation->{'fin'}->{'dir'}->{'pass'}->{'pl'}->{'3rd'} = $fin_dir_pass_root . "eront";
    }

    ################################################################
    # fin => dir => prtr
    ################################################################
    my $fin_dir_prtr_root = $inf_root2 . "b";

    if( $lemma eq 'esser' )
    {
        $ref_conjugation->{'fin'}->{'dir'}->{'prtr'}->{'sg'}->{'1st'} = "ebo";
        $ref_conjugation->{'fin'}->{'dir'}->{'prtr'}->{'sg'}->{'2nd'} = "ebas";
        $ref_conjugation->{'fin'}->{'dir'}->{'prtr'}->{'sg'}->{'3rd'} = "ebat";
        $ref_conjugation->{'fin'}->{'dir'}->{'prtr'}->{'pl'}->{'1st'} = "ebamos";
        $ref_conjugation->{'fin'}->{'dir'}->{'prtr'}->{'pl'}->{'2nd'} = "ebates";
        $ref_conjugation->{'fin'}->{'dir'}->{'prtr'}->{'pl'}->{'3rd'} = "ebant";
    }else{
        $ref_conjugation->{'fin'}->{'dir'}->{'prtr'}->{'sg'}->{'1st'} = $fin_dir_prtr_root . "o";
        $ref_conjugation->{'fin'}->{'dir'}->{'prtr'}->{'sg'}->{'2nd'} = $fin_dir_prtr_root . "as";
        $ref_conjugation->{'fin'}->{'dir'}->{'prtr'}->{'sg'}->{'3rd'} = $fin_dir_prtr_root . "at";
        $ref_conjugation->{'fin'}->{'dir'}->{'prtr'}->{'pl'}->{'1st'} = $fin_dir_prtr_root . "amos";
        $ref_conjugation->{'fin'}->{'dir'}->{'prtr'}->{'pl'}->{'2nd'} = $fin_dir_prtr_root . "ates";
        $ref_conjugation->{'fin'}->{'dir'}->{'prtr'}->{'pl'}->{'3rd'} = $fin_dir_prtr_root . "ant";
    }

    ################################################################
    # fin => dir => futr
    ################################################################
    my $fin_dir_futr_root = $inf_root . "er";

    if( $lemma eq 'esser' )
    {
        $ref_conjugation->{'fin'}->{'dir'}->{'futr'}->{'sg'}->{'1st'} = "serò";
        $ref_conjugation->{'fin'}->{'dir'}->{'futr'}->{'sg'}->{'2nd'} = "seràs";
        $ref_conjugation->{'fin'}->{'dir'}->{'futr'}->{'sg'}->{'3rd'} = "seràt";
        $ref_conjugation->{'fin'}->{'dir'}->{'futr'}->{'pl'}->{'1st'} = "seramos";
        $ref_conjugation->{'fin'}->{'dir'}->{'futr'}->{'pl'}->{'2nd'} = "serates";
        $ref_conjugation->{'fin'}->{'dir'}->{'futr'}->{'pl'}->{'3rd'} = "serànt";
    }else{
        $ref_conjugation->{'fin'}->{'dir'}->{'futr'}->{'sg'}->{'1st'} = $fin_dir_futr_root . "ò";
        $ref_conjugation->{'fin'}->{'dir'}->{'futr'}->{'sg'}->{'2nd'} = $fin_dir_futr_root . "às";
        $ref_conjugation->{'fin'}->{'dir'}->{'futr'}->{'sg'}->{'3rd'} = $fin_dir_futr_root . "àt";
        $ref_conjugation->{'fin'}->{'dir'}->{'futr'}->{'pl'}->{'1st'} = $fin_dir_futr_root . "amos";
        $ref_conjugation->{'fin'}->{'dir'}->{'futr'}->{'pl'}->{'2nd'} = $fin_dir_futr_root . "ates";
        $ref_conjugation->{'fin'}->{'dir'}->{'futr'}->{'pl'}->{'3rd'} = $fin_dir_futr_root . "ànt";
    }

    ################################################################
    # fin => sub => pres
    ################################################################
    if( $lemma eq 'esser' )
    {
        $ref_conjugation->{'fin'}->{'sub'}->{'pres'}->{'sg'}->{'1st'} = "sim";
        $ref_conjugation->{'fin'}->{'sub'}->{'pres'}->{'sg'}->{'2nd'} = "sis";
        $ref_conjugation->{'fin'}->{'sub'}->{'pres'}->{'sg'}->{'3rd'} = "sit";
        $ref_conjugation->{'fin'}->{'sub'}->{'pres'}->{'pl'}->{'1st'} = "simos";
        $ref_conjugation->{'fin'}->{'sub'}->{'pres'}->{'pl'}->{'2nd'} = "sites";
        $ref_conjugation->{'fin'}->{'sub'}->{'pres'}->{'pl'}->{'3rd'} = "sint";
    }else{
        $ref_conjugation->{'fin'}->{'sub'}->{'pres'}->{'sg'}->{'1st'} = $inf_root . "ea";
        $ref_conjugation->{'fin'}->{'sub'}->{'pres'}->{'sg'}->{'2nd'} = $inf_root . "eas";
        $ref_conjugation->{'fin'}->{'sub'}->{'pres'}->{'sg'}->{'3rd'} = $inf_root . "eat";
        $ref_conjugation->{'fin'}->{'sub'}->{'pres'}->{'pl'}->{'1st'} = $inf_root . "eamos";
        $ref_conjugation->{'fin'}->{'sub'}->{'pres'}->{'pl'}->{'2nd'} = $inf_root . "eates";
        $ref_conjugation->{'fin'}->{'sub'}->{'pres'}->{'pl'}->{'3rd'} = $inf_root . "eant";
    }

    ################################################################
    # fin => sub => prtr
    ################################################################
    my $fin_sub_prtr_root = $inf_root2;
    if( defined( $ref_subform->{'fin_sub_prtr_sg_1st'} ) )
    {
        if( not $ref_subform->{'fin_sub_prtr_sg_1st'} =~ /^-/ )
        {
            $fin_sub_prtr_root = $ref_subform->{'fin_sub_prtr_sg_1st'};
            $fin_sub_prtr_root =~ s/esse$//g;
        }
    }

    if( $lemma eq 'esser' )
    {
        $ref_conjugation->{'fin'}->{'sub'}->{'prtr'}->{'sg'}->{'1st'} = "essim";
        $ref_conjugation->{'fin'}->{'sub'}->{'prtr'}->{'sg'}->{'2nd'} = "essis";
        $ref_conjugation->{'fin'}->{'sub'}->{'prtr'}->{'sg'}->{'3rd'} = "essit";
        $ref_conjugation->{'fin'}->{'sub'}->{'prtr'}->{'pl'}->{'1st'} = "essemos";
        $ref_conjugation->{'fin'}->{'sub'}->{'prtr'}->{'pl'}->{'2nd'} = "essetes";
        $ref_conjugation->{'fin'}->{'sub'}->{'prtr'}->{'pl'}->{'3rd'} = "essent";
    }else{
        $ref_conjugation->{'fin'}->{'sub'}->{'prtr'}->{'sg'}->{'1st'} = $fin_sub_prtr_root . "esse";
        $ref_conjugation->{'fin'}->{'sub'}->{'prtr'}->{'sg'}->{'2nd'} = $fin_sub_prtr_root . "esses";
        $ref_conjugation->{'fin'}->{'sub'}->{'prtr'}->{'sg'}->{'3rd'} = $fin_sub_prtr_root . "essit";
        $ref_conjugation->{'fin'}->{'sub'}->{'prtr'}->{'pl'}->{'1st'} = $fin_sub_prtr_root . "essemos";
        $ref_conjugation->{'fin'}->{'sub'}->{'prtr'}->{'pl'}->{'2nd'} = $fin_sub_prtr_root . "essetes";
        $ref_conjugation->{'fin'}->{'sub'}->{'prtr'}->{'pl'}->{'3rd'} = $fin_sub_prtr_root . "essent";
    }

    ################################################################
    # fin => con => pres
    ################################################################
    if( $lemma eq 'esser' )
    {
        $ref_conjugation->{'fin'}->{'con'}->{'pres'}->{'sg'}->{'1st'} = "erei";
        $ref_conjugation->{'fin'}->{'con'}->{'pres'}->{'sg'}->{'2nd'} = "ereis";
        $ref_conjugation->{'fin'}->{'con'}->{'pres'}->{'sg'}->{'3rd'} = "eriesti";
        $ref_conjugation->{'fin'}->{'con'}->{'pres'}->{'pl'}->{'1st'} = "eriemos";
        $ref_conjugation->{'fin'}->{'con'}->{'pres'}->{'pl'}->{'2nd'} = "erietes";
        $ref_conjugation->{'fin'}->{'con'}->{'pres'}->{'pl'}->{'3rd'} = "erieront";
    }else{
        $ref_conjugation->{'fin'}->{'con'}->{'pres'}->{'sg'}->{'1st'} = $inf_root2 . "rei";
        $ref_conjugation->{'fin'}->{'con'}->{'pres'}->{'sg'}->{'2nd'} = $inf_root2 . "reis";
        $ref_conjugation->{'fin'}->{'con'}->{'pres'}->{'sg'}->{'3rd'} = $inf_root2 . "resti";
        $ref_conjugation->{'fin'}->{'con'}->{'pres'}->{'pl'}->{'1st'} = $inf_root2 . "riemos";
        $ref_conjugation->{'fin'}->{'con'}->{'pres'}->{'pl'}->{'2nd'} = $inf_root2 . "rietes";
        $ref_conjugation->{'fin'}->{'con'}->{'pres'}->{'pl'}->{'3rd'} = $inf_root2 . "rieront";
    }

    ################################################################
    # fin => imp => pres
    ################################################################
    if( $lemma eq 'esser' )
    {
        $ref_conjugation->{'fin'}->{'imp'}->{'pres'}->{'sg'}->{'2nd'} = "es";
        $ref_conjugation->{'fin'}->{'imp'}->{'pres'}->{'pl'}->{'2nd'} = "este";
    }else{
        $ref_conjugation->{'fin'}->{'imp'}->{'pres'}->{'sg'}->{'2nd'} = $inf_root2 . "";
        $ref_conjugation->{'fin'}->{'imp'}->{'pres'}->{'pl'}->{'2nd'} = $inf_root2 . "te";
    }



    return( $ref_conjugation );
}


sub genhtml_categoryinfo_short
{
    ( my $self, my $ref_category ) = @_;
    my $str = "";

    if( not defined( $ref_category ) or not exists( $ref_category->{'category'} ) )
    {
        return( "" );
    }

    $str .= $ref_category->{'category'};

    if( $ref_category->{'category'} eq "nom" )
    {
        $str .= " " . $ref_category->{'gender'};
        if(  $ref_category->{'subform'}->{'sg_obl'} eq '-'
         and $ref_category->{'subform'}->{'pl_rct'} eq '-'
         and $ref_category->{'subform'}->{'pl_obl'} eq '-'
        ) {
            $str .= " " . "INVAR";
        }else{
            $str .= " " . $ref_category->{'subform'}->{'sg_obl'};
            $str .= " " . $ref_category->{'subform'}->{'pl_rct'};
            $str .= " " . $ref_category->{'subform'}->{'pl_obl'};
        }
    }
    elsif( $ref_category->{'category'} eq "adj" )
    {
        # $str .= " " . $ref_category->{'gender'};
        if(  $ref_category->{'subform'}->{'m_sg_obl'} eq '-'
         and $ref_category->{'subform'}->{'m_pl_rct'} eq '-'
         and $ref_category->{'subform'}->{'m_pl_obl'} eq '-'
        ) {
            $str .= " " . "INVAR";
        }else{
            $str .= " " . $ref_category->{'subform'}->{'m_sg_obl'};
            $str .= " " . $ref_category->{'subform'}->{'m_pl_rct'};
            $str .= " " . $ref_category->{'subform'}->{'m_pl_obl'};
        }
    }
    elsif( $ref_category->{'category'} eq "verb" )
    {
        $str .= " " . $ref_category->{'subform'}->{'fin_dir_pres_sg_1st'};
        $str .= " " . $ref_category->{'subform'}->{'fin_dir_perf_sg_1st'};

        my $part_perf = $ref_category->{'subform'}->{'part_perf_root'};
        $part_perf =~ s/-$/e/g;
        $str .= " " . $part_perf;
    }

    return( $str );
}


sub genhtml_categoryinfo_long
{
    ( my $self, my $ref_category ) = @_;
    my $str = "";

    if( not defined( $ref_category ) or not exists( $ref_category->{'category'} ) )
    {
        return( "" );
    }

    if( $ref_category->{'category'} eq "nom" )
    {
        if( $ref_category->{'gender'} eq "m" )
        {
            $str .= "男性名詞";
        }
        elsif( $ref_category->{'gender'} eq "f" )
        {
            $str .= "女性名詞";
        }
        else
        {
            $str .= "中性名詞";
        }
    }
    elsif( $ref_category->{'category'} eq "adj" )
    {
        $str .= "形容詞";
    }
    elsif( $ref_category->{'category'} eq "verb" )
    {
        $str .= "動詞";
    }
    elsif( $ref_category->{'category'} eq "adv" )
    {
        $str .= "副詞";
    }
    elsif( $ref_category->{'category'} eq "art" )
    {
        $str .= "冠詞";
    }
    elsif( $ref_category->{'category'} eq "conj" )
    {
        $str .= "接続詞";
    }
    elsif( $ref_category->{'category'} eq "int" )
    {
        $str .= "間投詞";
    }
    elsif( $ref_category->{'category'} eq "prep" )
    {
        $str .= "前置詞";
    }
    elsif( $ref_category->{'category'} eq "pro" )
    {
        $str .= "代名詞";
    }

    return( $str );
}

sub genhtml_inflectiontable
{
    ( my $self, my $lemma, my $ref_category ) = @_;
    my $str = "";

    if( $ref_category->{'category'} eq "nom" )
    {
        my $ref_declension_nom = get_declension_nom( $self, $lemma, $ref_category->{'gender'}, $ref_category->{'subform'} );
        # ( my $self, my $lemma, my $gender, my $subform ) = @_;

        $str .= "
            <table class='declension_nom'>
              <tbody>
                <tr>
                  <th></th>
                  <td>単数</td>
                  <td>複数</td>
                </tr>
                <tr>
                  <th>直格</th>
                  <td>$ref_declension_nom->{'sg'}->{'rct'}</td>
                  <td>$ref_declension_nom->{'pl'}->{'rct'}</td>
                </tr>
                <tr>
                  <th>斜格</th>
                  <td>$ref_declension_nom->{'sg'}->{'obl'}</td>
                  <td>$ref_declension_nom->{'pl'}->{'obl'}</td>
                </tr>
              </tbody>
            </table>
        ";
    }
    elsif( $ref_category->{'category'} eq "adj" )
    {
        my $ref_declension_adj = get_declension_adj( $self, $lemma, $ref_category->{'subform'} );
        # ( my $self, my $lemma, my $subform ) = @_;

        $str .= "
            <table class='declension_adj'>
              <tbody>
                <tr>
                  <th rowspan='2'></th>
                  <td colspan='2'>男性</td>
                  <td colspan='2'>女性</td>
                </tr>
                <tr>
                  <td>単数</td>
                  <td>複数</td>
                  <td>単数</td>
                  <td>複数</td>
                </tr>
                <tr>
                  <th>直格</th>
                  <td>$ref_declension_adj->{'m'}->{'sg'}->{'rct'}</td>
                  <td>$ref_declension_adj->{'m'}->{'pl'}->{'rct'}</td>
                  <td>$ref_declension_adj->{'f'}->{'sg'}->{'rct'}</td>
                  <td>$ref_declension_adj->{'f'}->{'pl'}->{'rct'}</td>
                </tr>
                <tr>
                  <th>斜格</th>
                  <td>$ref_declension_adj->{'m'}->{'sg'}->{'obl'}</td>
                  <td>$ref_declension_adj->{'m'}->{'pl'}->{'obl'}</td>
                  <td>$ref_declension_adj->{'f'}->{'sg'}->{'obl'}</td>
                  <td>$ref_declension_adj->{'f'}->{'pl'}->{'obl'}</td>
                </tr>
              </tbody>
            </table>
        ";

    }
    elsif( $ref_category->{'category'} eq "verb" )
    {
        my $ref_conjugation_verb = get_conjugation_verb( $self, $lemma, $ref_category->{'subform'} );

        $str .= "
            <table class='conjugation_verb'>
              <tbody>
                <tr>
                  <th rowspan='5' style='writing-mode: vertical-rl'>不定法</th>
                  <th            >不定詞</th>
                  <td colspan='6'>$ref_conjugation_verb->{'inf'}->{'inf'}</td>
                </tr>
                <tr>
                  <th            >動名詞</th>
                  <td colspan='6'>$ref_conjugation_verb->{'inf'}->{'gerund'}->{'sg'}->{'rct'}, -o, -i, -i</td>
                </tr>
                <tr>
                  <th            >能動分詞</th>
                  <td colspan='6'>$ref_conjugation_verb->{'inf'}->{'part'}->{'pres'}->{'m'}->{'sg'}->{'rct'}, $ref_conjugation_verb->{'inf'}->{'part'}->{'pres'}->{'m'}->{'sg'}->{'obl'}, -i, -i</td>
                </tr>
                <tr>
                  <th            >完了<br>受動分詞</th>
                  <td colspan='6'>$ref_conjugation_verb->{'inf'}->{'part'}->{'perf'}->{'m'}->{'sg'}->{'rct'}, -o, -i, -i</td>
                </tr>
                <tr>
                  <th            >未完了<br>受動分詞</th>
                  <td colspan='6'>$ref_conjugation_verb->{'inf'}->{'part'}->{'prtr'}->{'m'}->{'sg'}->{'rct'}, -o, -i, -i</td>
                </tr>
                <tr>
                  <th rowspan='2' colspan='2'></th>
                  <th colspan='3'>単数</th>
                  <th colspan='3'>複数</th>
                </tr>
                <tr>
                  <th            >一人称</th>
                  <th            >二人称</th>
                  <th            >三人称</th>
                  <th            >一人称</th>
                  <th            >二人称</th>
                  <th            >三人称</th>
                </tr>
                <tr>
                  <th rowspan='8' style='writing-mode: vertical-rl'>直説法</th>
                  <th            >現在</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'1st'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'2nd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'pres'}->{'sg'}->{'3rd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'pres'}->{'pl'}->{'1st'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'pres'}->{'pl'}->{'2nd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'pres'}->{'pl'}->{'3rd'}</td>
                </tr>
                <tr>
                  <th            >単純過去</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'pass'}->{'sg'}->{'1st'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'pass'}->{'sg'}->{'2nd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'pass'}->{'sg'}->{'3rd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'pass'}->{'pl'}->{'1st'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'pass'}->{'pl'}->{'2nd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'pass'}->{'pl'}->{'3rd'}</td>
                </tr>
                <tr>
                  <th            >継続過去</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'prtr'}->{'sg'}->{'1st'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'prtr'}->{'sg'}->{'2nd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'prtr'}->{'sg'}->{'3rd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'prtr'}->{'pl'}->{'1st'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'prtr'}->{'pl'}->{'2nd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'prtr'}->{'pl'}->{'3rd'}</td>
                </tr>
                <tr>
                  <th            >単純未来</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'futr'}->{'sg'}->{'1st'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'futr'}->{'sg'}->{'2nd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'futr'}->{'sg'}->{'3rd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'futr'}->{'pl'}->{'1st'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'futr'}->{'pl'}->{'2nd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'dir'}->{'futr'}->{'pl'}->{'3rd'}</td>
                </tr>
                <tr>
                  <th            >現在完了</td>
                  <td colspan='6'>完了受動分詞＋助動詞の直説法現在</td>
                </tr>
                <tr>
                  <th            >前過去</td>
                  <td colspan='6'>完了受動分詞＋助動詞の直説法単純過去</td>
                </tr>
                <tr>
                  <th            >大過去</td>
                  <td colspan='6'>完了受動分詞＋助動詞の直説法継続過去</td>
                </tr>
                <tr>
                  <th            >前未来</td>
                  <td colspan='6'>完了受動分詞＋助動詞の直説法単純未来</td>
                </tr>
                <tr>
                  <th rowspan='4' style='writing-mode: vertical-rl'>接続法</th>
                  <th            >現在</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'sub'}->{'pres'}->{'sg'}->{'1st'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'sub'}->{'pres'}->{'sg'}->{'2nd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'sub'}->{'pres'}->{'sg'}->{'3rd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'sub'}->{'pres'}->{'pl'}->{'1st'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'sub'}->{'pres'}->{'pl'}->{'2nd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'sub'}->{'pres'}->{'pl'}->{'3rd'}</td>
                </tr>
                <tr>
                  <th            >継続過去</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'sub'}->{'prtr'}->{'sg'}->{'1st'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'sub'}->{'prtr'}->{'sg'}->{'2nd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'sub'}->{'prtr'}->{'sg'}->{'3rd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'sub'}->{'prtr'}->{'pl'}->{'1st'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'sub'}->{'prtr'}->{'pl'}->{'2nd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'sub'}->{'prtr'}->{'pl'}->{'3rd'}</td>
                </tr>
                <tr>
                  <th            >過去</td>
                  <td colspan='6'>完了受動分詞＋助動詞の接続法現在</td>
                </tr>
                <tr>
                  <th            >大過去</td>
                  <td colspan='6'>完了受動分詞＋助動詞の接続法継続過去</td>
                </tr>
                <tr>
                  <th rowspan='2' style='writing-mode: vertical-rl'>条件法</th>
                  <th            >現在</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'con'}->{'pres'}->{'sg'}->{'1st'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'con'}->{'pres'}->{'sg'}->{'2nd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'con'}->{'pres'}->{'sg'}->{'3rd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'con'}->{'pres'}->{'pl'}->{'1st'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'con'}->{'pres'}->{'pl'}->{'2nd'}</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'con'}->{'pres'}->{'pl'}->{'3rd'}</td>
                </tr>
                <tr>
                  <th            >過去</td>
                  <td colspan='6'>完了受動分詞＋助動詞の条件法現在</td>
                </tr>
                <tr>
                  <th colspan='2'>命令法</th>
                  <td            >-</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'imp'}->{'pres'}->{'sg'}->{'2nd'}</td>
                  <td            >-</td>
                  <td            >-</td>
                  <td            >$ref_conjugation_verb->{'fin'}->{'imp'}->{'pres'}->{'pl'}->{'2nd'}</td>
                  <td            >-</td>
                </tr>
              </tbody>
            </table>
        ";
    }
    else
    {
        return( undef );
    }

    return( $str );
}


sub genhtml_index
{
    ( my $self ) = @_;
    my @ref_word_list = $self->load_allwords();
    my $str = "";

    $str .= "<div class='lexicon'>\n";
    $str .= "  <ul class='lxc-list'>\n";

    my $lxc_count = 0;
    for my $ref_word (@ref_word_list)
    {
        # print STDERR ( "DEBUG : ref_word :" . Dumper( $ref_word ) . "\n" );

        my $lxc = "";

        $lxc .= "<span class='lemma' id='" . $ref_word->{'lemma'} . "'><a href='/?page=lexicon&word=" . $ref_word->{'lemma'} . "'>" . $ref_word->{'lemma'} . "</a></span>";
        # $lxc .= " <span class='phonetic'>/" . $ref_word->{'info'}->{'phonetic'} . "/</span>";

        my $category_count = 0;
        for my $ref_category (@{$ref_word->{'info'}->{'category_list'}})
        {
            $lxc .= " <span class='subinfo'>⟦" . genhtml_categoryinfo_short( $self, $ref_category ) . "⟧</span>";

            my $def_count = 0;
            for my $ref_def (@{$ref_category->{'def_list'}})
            {
                $lxc .= (0 < $def_count) ? "▶" : "";
                $lxc .= " <span class='def" . (($ref_def->{'sensitive'} eq 1) ? " sensitive" : "") . "'>" . $ref_def->{'def'} . "</span>";

                # my $exp_count = 0;
                # for my $ref_exp (@{$ref_def->{'exp_list'}})
                # {
                #     $lxc .= " :";
                #     $lxc .= " <span class='exp original'>" . $ref_exp->{'original'} . "</span>";
                #     $lxc .= " <span class='exp translate'>" . $ref_exp->{'translate'} . "</span>";

                #     $exp_count++;
                # }

                $def_count++;
            }

            # my $syn_count = 0;
            # for my $ref_syn (@{$ref_category->{'syn_list'}})
            # {
            #     $lxc .= ($syn_count == 0) ? " ⟪syn⟫" : ",";
            #     $lxc .= " <span class='syn'>" . $ref_syn . "</span>";

            #     $syn_count++;
            # }

            # my $ant_count = 0;
            # for my $ref_ant (@{$ref_category->{'ant_list'}})
            # {
            #     $lxc .= ($ant_count == 0) ? " ⟪ant⟫" : ",";
            #     $lxc .= " <span class='ant'>" . $ref_ant . "</span>";

            #     $ant_count++;
            # }

            # my $ref_count = 0;
            # for my $ref_ref (@{$ref_category->{'ref_list'}})
            # {
            #     $lxc .= ($ref_count == 0) ? " ⟪ref⟫" : ",";
            #     $lxc .= " <span class='ref'>" . $ref_ref . "</span>";

            #     $ref_count++;
            # }

            # my $org_count = 0;
            # for my $ref_org (@{$ref_category->{'org_list'}})
            # {
            #     $lxc .= ($org_count == 0) ? " ⟪org⟫" : ",";
            #     $lxc .= " <span class='org'>" . $ref_org . "</span>";

            #     $org_count++;
            # }

            $category_count++;
        }

        $str .= "    <li class='lxc'>" . $lxc . "</li>\n";

        $lxc_count += 1;
    }
    $str .= "  </ul>\n";
    $str .= "</div>\n";

    return( $str );
}

sub genhtml_word
{
    ( my $self, my $lemma ) = @_;
    my $ref_word = $self->load_word( $lemma );
    my $str = "";

    if( not defined( $ref_word ) )
    {
        printf STDERR ( "error : not found word : $lemma" );
        return( "<p>${lemma} : NON TROVATE</p>" );
    }

    $str .= "<div class='word'>\n";
    $str .= "  <div class='lemma'>\n";
    $str .= "    <span class='spell'>" . $ref_word->{'lemma'} . "</span>\n";
    $str .= "    <span class='phonetic'>/" . $ref_word->{'info'}->{'phonetic'} . "/</span>\n";
    $str .= "  </div>\n";

    my $category_count = 0;
    for my $ref_category (@{$ref_word->{'info'}->{'category_list'}})
    {
        if( 0 < $category_count ) {
            $str .= " <hr>\n";
        }

        $str .= "  <div class='category_item'>\n";
        $str .= "    <div class='category'>" . genhtml_categoryinfo_long( $self, $ref_category ) . "</div>\n";

        my $def_count = 0;
        for my $ref_def (@{$ref_category->{'def_list'}})
        {
            $str .= "    <div class='def_item'>\n";
            $str .= "      <div class='def'>" . $ref_def->{'def'} . "</div>\n";

            my $exp_count = 0;
            for my $ref_exp (@{$ref_def->{'exp_list'}})
            {
                $str .= "      <div class='exp'>\n";
                $str .= "        <span class='original'>" . $ref_exp->{'original'} . "</span>\n";
                $str .= "        <span class='translate'>" . $ref_exp->{'translate'} . "</span>\n";
                $str .= "      </div>\n";

                $exp_count++;
            }

            $str .= "    </div>\n";

            $def_count++;
        }

        if( my $inflectiontable = genhtml_inflectiontable( $self, $lemma, $ref_category ) )
        {
            $str .= "    <div class='inflection'>\n";
            $str .= $inflectiontable;
            $str .= "    </div>\n";
        }

        $str .= "  </div>\n";

        $category_count++;
    }

    $str .= "</div>\n";

    return( $str );
}

sub genhtml_conj
{
    ( my $self, my $lemma ) = @_;

    # TODO

    return( 0 );
}













1;
