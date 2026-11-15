#!/bin/perl
package Mik::Mik2;
use strict;
use warnings;
use Data::Dumper;
use JSON;
use Storable;
use utf8;
use Encode;

my $vow = "aeiouAEIOU";
my $acc = "áéíóúÁÉÍÓÚ";
my $cns = "bcdfghjklmnpqrstvwxyzçţčšťžBCDFGHJKLMNPQRSTVWXYZÇŢČŠŤŽ";
# my $json = JSON->new;
my $json = JSON->new->pretty();

my @gender         = ( "male", "female" );
my @genderDim      = ( "m", "f" );
my @number         = ( "singulare", "plurale" );
my @numberDim      = ( "sg", "pl" );
my @case           = ( "tem", "nom", "acc", "gen", "dat" );
my @mood           = ( 'infinito', 'gerundio',
                       'participio presente', 'participio passato',
                       'indicativo', 'subjunctivo', 'conditionale' );
my @tempo          = ( 'presente', 'imperfecto',
                       'passato semplice', 'futuro semplice',
                       'perfecto', 'passato', 'trapassato',
                       'passato anteriore', 'futuro anteriore' );
my @personalite    = ( 'primo', 'duemo', 'tremo' );
my @personaliteDim = ( '1', '2', '3' );

my $ruleRef = {
    decl => [
        # 1mo -a
        {
            classe => "a",
            gender => "female",
            singulare => {
                tem => "a", nom => "is", acc => "am",
                gen => "ae", dat => "á",
            },
            plurale => {
                tem => "e", nom => "és", acc => "ás",
                gen => "árum", dat => "íbus",
            },
        },
        # 2mo -o
        {
            classe => "o",
            gender => "male",
            singulare => {
                tem => "o", nom => "us", acc => "um",
                gen => "í", dat => "ó",
            },
            plurale => {
                tem => "a", nom => "és", acc => "ós",
                gen => "órum", dat => "íbus",
            },
        },
        # 2mo' -u
        {
            classe => "u",
            gender => "male",
            singulare => {
                tem => "u", nom => "us", acc => "um",
                gen => "í", dat => "ó",
            },
            plurale => {
                tem => "a", nom => "és", acc => "ós",
                gen => "órum", dat => "íbus",
            },
        },
        # 3mo -e
        {
            classe => "e",
            gender => "male",
            singulare => {
                tem => "e", nom => "is", acc => "em",
                gen => "í", dat => "é",
            },
            plurale => {
                tem => "i", nom => "és", acc => "ís",
                gen => "érum", dat => "ébus",
            },
        },
        # 3mo' -i
        {
            classe => "i",
            gender => "male",
            singulare => {
                tem => "i", nom => "is", acc => "em",
                gen => "í", dat => "é",
            },
            plurale => {
                tem => "i", nom => "és", acc => "ís",
                gen => "érum", dat => "ébus",
            },
        },
        # 3mo'' -∅
        {
            classe => "n",
            gender => "male",
            singulare => {
                tem => "", nom => "is", acc => "em",
                gen => "í", dat => "é",
            },
            plurale => {
                tem => "i", nom => "és", acc => "ís",
                gen => "érum", dat => "ébus",
            },
        },
    ],
    conj => [
        # 1mo -áre
        {
            classe => "áre",
            infinito => "áre",
            gerundio => "ándo",
            'participio presente' => "ánte",
            'participio passato' => "áto",
            indicativo => {
                presente => {
                    tipo => "semplice",
                    singulare => { primo => "o", duemo => "as", tremo => "a" },
                    plurale => { primo => "ámo", duemo => "áte", tremo => "ano" },
                },
                imperfecto => {
                    tipo => "semplice",
                    singulare => { primo => "ábo", duemo => "ábas", tremo => "ába" },
                    plurale => { primo => "abámo", duemo => "abáte", tremo => "ábano" },
                },
                'passato semplice' => {
                    tipo => "semplice",
                    singulare => { primo => "ái", duemo => "áis", tremo => "áit" },
                    plurale => { primo => "iémo", duemo => "iéte", tremo => "iéno" },
                },
                'futuro semplice' => {
                    tipo => "semplice",
                    singulare => { primo => "áro", duemo => "áras", tremo => "ára" },
                    plurale => { primo => "arámo", duemo => "aráte", tremo => "árano" },
                },
                perfecto => {
                    tipo => "complexo",
                    auxiliareTempo => "presente",
                },
                trapassato => {
                    tipo => "complexo",
                    auxiliareTempo => "imperfecto",
                },
                'passato anteriore' => {
                    tipo => "complexo",
                    auxiliareTempo => "passato semplice",
                },
                'futuro anteriore' => {
                    tipo => "complexo",
                    auxiliareTempo => "futuro semplice",
                },
            },
            subjunctivo => {
                presente => {
                    tipo => "semplice",
                    singulare => { primo => "e", duemo => "es", tremo => "i" },
                    plurale => { primo => "émo", duemo => "éte", tremo => "ano" },
                },
                imperfecto => {
                    tipo => "semplice",
                    singulare => { primo => "ásse", duemo => "ásses", tremo => "ássi" },
                    plurale => { primo => "assémo", duemo => "asséte", tremo => "ásseno" },
                },
                passato => {
                    tipo => "complexo",
                    auxiliareTempo => "presente",
                },
                trapassato => {
                    tipo => "complexo",
                    auxiliareTempo => "imperfecto",
                },
            },
            conditionale => {
                presente => {
                    tipo => "semplice",
                    singulare => { primo => "aréi", duemo => "aréis", tremo => "aréit" },
                    plurale => { primo => "ariémo", duemo => "ariéte", tremo => "ariéno" },
                },
                passato => {
                    tipo => "complexo",
                    auxiliareTempo => "presente",
                },
            },
        },
        # 2mo -ére
        {
            classe => "ére",
            infinito => "ére",
            gerundio => "éndo",
            'participio presente' => "énte",
            'participio passato' => "úto",
            indicativo => {
                presente => {
                    tipo => "semplice",
                    singulare => { primo => "eo", duemo => "es", tremo => "e" },
                    plurale => { primo => "émo", duemo => "éte", tremo => "eno" },
                },
                imperfecto => {
                    tipo => "semplice",
                    singulare => { primo => "ébo", duemo => "ébas", tremo => "éba" },
                    plurale => { primo => "ebámo", duemo => "ebáte", tremo => "ébano" },
                },
                'passato semplice' => {
                    tipo => "semplice",
                    singulare => { primo => "éi", duemo => "éis", tremo => "éit" },
                    plurale => { primo => "iémo", duemo => "iéte", tremo => "iéno" },
                },
                'futuro semplice' => {
                    tipo => "semplice",
                    singulare => { primo => "éro", duemo => "éras", tremo => "éra" },
                    plurale => { primo => "erámo", duemo => "eráte", tremo => "érano" },
                },
                perfecto => {
                    tipo => "complexo",
                    auxiliareTempo => "presente",
                },
                trapassato => {
                    tipo => "complexo",
                    auxiliareTempo => "imperfecto",
                },
                'passato anteriore' => {
                    tipo => "complexo",
                    auxiliareTempo => "passato semplice",
                },
                'futuro anteriore' => {
                    tipo => "complexo",
                    auxiliareTempo => "futuro semplice",
                },
            },
            subjunctivo => {
                presente => {
                    tipo => "semplice",
                    singulare => { primo => "ea", duemo => "eas", tremo => "i" },
                    plurale => { primo => "eámo", duemo => "eáte", tremo => "éano" },
                },
                imperfecto => {
                    tipo => "semplice",
                    singulare => { primo => "ésse", duemo => "ésses", tremo => "éssi" },
                    plurale => { primo => "essémo", duemo => "esséte", tremo => "ésseno" },
                },
                passato => {
                    tipo => "complexo",
                    auxiliareTempo => "presente",
                },
                trapassato => {
                    tipo => "complexo",
                    auxiliareTempo => "imperfecto",
                },
            },
            conditionale => {
                presente => {
                    tipo => "semplice",
                    singulare => { primo => "eréi", duemo => "eréis", tremo => "eréit" },
                    plurale => { primo => "eriémo", duemo => "eriéte", tremo => "eriéno" },
                },
                passato => {
                    tipo => "complexo",
                    auxiliareTempo => "presente",
                },
            },
        },
        # 2mo' -ere
        {
            classe => "ere",
            infinito => "ere",
            gerundio => "éndo",
            'participio presente' => "énte",
            'participio passato' => "úto",
            indicativo => {
                presente => {
                    tipo => "semplice",
                    singulare => { primo => "o", duemo => "is", tremo => "e" },
                    plurale => { primo => "émo", duemo => "éte", tremo => "eno" },
                },
                imperfecto => {
                    tipo => "semplice",
                    singulare => { primo => "ébo", duemo => "ébas", tremo => "éba" },
                    plurale => { primo => "ebámo", duemo => "ebáte", tremo => "ébano" },
                },
                'passato semplice' => {
                    tipo => "semplice",
                    singulare => { primo => "éi", duemo => "éis", tremo => "éit" },
                    plurale => { primo => "iémo", duemo => "iéte", tremo => "iéno" },
                },
                'futuro semplice' => {
                    tipo => "semplice",
                    singulare => { primo => "éro", duemo => "éras", tremo => "éra" },
                    plurale => { primo => "erámo", duemo => "eráte", tremo => "érano" },
                },
                perfecto => {
                    tipo => "complexo",
                    auxiliareTempo => "presente",
                },
                trapassato => {
                    tipo => "complexo",
                    auxiliareTempo => "imperfecto",
                },
                'passato anteriore' => {
                    tipo => "complexo",
                    auxiliareTempo => "passato semplice",
                },
                'futuro anteriore' => {
                    tipo => "complexo",
                    auxiliareTempo => "futuro semplice",
                },
            },
            subjunctivo => {
                presente => {
                    tipo => "semplice",
                    singulare => { primo => "a", duemo => "as", tremo => "i" },
                    plurale => { primo => "ámo", duemo => "áte", tremo => "ano" },
                },
                imperfecto => {
                    tipo => "semplice",
                    singulare => { primo => "ésse", duemo => "ésses", tremo => "éssi" },
                    plurale => { primo => "essémo", duemo => "esséte", tremo => "ésseno" },
                },
                passato => {
                    tipo => "complexo",
                    auxiliareTempo => "presente",
                },
                trapassato => {
                    tipo => "complexo",
                    auxiliareTempo => "imperfecto",
                },
            },
            conditionale => {
                presente => {
                    tipo => "semplice",
                    singulare => { primo => "eréi", duemo => "eréis", tremo => "eréit" },
                    plurale => { primo => "eriémo", duemo => "eriéte", tremo => "eriéno" },
                },
                passato => {
                    tipo => "complexo",
                    auxiliareTempo => "presente",
                },
            },
        },
        # 3mo -íre
        {
            classe => "íre",
            infinito => "íre",
            gerundio => "éndo",
            'participio presente' => "énte",
            'participio passato' => "íto",
            indicativo => {
                presente => {
                    tipo => "semplice",
                    singulare => { primo => "io", duemo => "es", tremo => "e" },
                    plurale => { primo => "émo", duemo => "éte", tremo => "eno" },
                },
                imperfecto => {
                    tipo => "semplice",
                    singulare => { primo => "ébo", duemo => "ébas", tremo => "éba" },
                    plurale => { primo => "ebámo", duemo => "ebáte", tremo => "ébano" },
                },
                'passato semplice' => {
                    tipo => "semplice",
                    singulare => { primo => "éi", duemo => "éis", tremo => "éit" },
                    plurale => { primo => "iémo", duemo => "iéte", tremo => "iéno" },
                },
                'futuro semplice' => {
                    tipo => "semplice",
                    singulare => { primo => "éro", duemo => "éras", tremo => "éra" },
                    plurale => { primo => "erámo", duemo => "eráte", tremo => "érano" },
                },
                perfecto => {
                    tipo => "complexo",
                    auxiliareTempo => "presente",
                },
                trapassato => {
                    tipo => "complexo",
                    auxiliareTempo => "imperfecto",
                },
                'passato nteriore' => {
                    tipo => "complexo",
                    auxiliareTempo => "passato semplice",
                },
                'futuro anteriore' => {
                    tipo => "complexo",
                    auxiliareTempo => "futuro semplice",
                },
            },
            subjunctivo => {
                presente => {
                    tipo => "semplice",
                    singulare => { primo => "ia", duemo => "as", tremo => "i" },
                    plurale => { primo => "iámo", duemo => "iáte", tremo => "íano" },
                },
                imperfecto => {
                    tipo => "semplice",
                    singulare => { primo => "ísse", duemo => "ísses", tremo => "íssi" },
                    plurale => { primo => "issémo", duemo => "isséte", tremo => "ísseno" },
                },
                passato => {
                    tipo => "complexo",
                    auxiliareTempo => "presente",
                },
                trapassato => {
                    tipo => "complexo",
                    auxiliareTempo => "imperfecto",
                },
            },
            conditionale => {
                presente => {
                    tipo => "semplice",
                    singulare => { primo => "eréi", duemo => "eréis", tremo => "eréit" },
                    plurale => { primo => "eriémo", duemo => "eriéte", tremo => "eriéno" },
                },
                passato => {
                    tipo => "complexo",
                    auxiliareTempo => "presente",
                },
            },
        },
    ],
};

my $irrRef = {
    decl => {
        'égo' => {
            nuclFormal => 'me',
            singulare => {
                tem => "égo", nom => "mis", acc => "me",
                gen => "meí", dat => "míhi",
            },
            plurale => {
                tem => "nos", nom => "nostés", acc => "nostís",
                gen => "ností", dat => "nobís",
            },
        },
        'nós' => {
            nuclFormal => 'nóste',
            singulare => {
                tem => "égo", nom => "mis", acc => "me",
                gen => "meí", dat => "míhi",
            },
            plurale => {
                tem => "nos", nom => "nostés", acc => "nostís",
                gen => "ností", dat => "nobís",
            },
        },
        'tu' => {
            nuclFormal => 'te',
            singulare => {
                tem => "tu", nom => "tis", acc => "te",
                gen => "tuí", dat => "tíbi",
            },
            plurale => {
                tem => "vos", nom => "vostés", acc => "vostís",
                gen => "vostí", dat => "vobís",
            },
        },
        'vós' => {
            nuclFormal => 'vóste',
            singulare => {
                tem => "tu", nom => "tis", acc => "te",
                gen => "tuí", dat => "tíbi",
            },
            plurale => {
                tem => "vos", nom => "vostés", acc => "vostís",
                gen => "vostí", dat => "vobís",
            },
        },
    },
    conj => {
        éstere => {
            classe => "ere",
            'participio passato' => "státo",
            indicativo => {
                presente => {
                    singulare => { primo => "sum", duemo => "es", tremo => "est" },
                    plurale => { primo => "súmus", duemo => "éstis", tremo => "sunt" },
                },
                imperfecto => {
                    singulare => { primo => "ébo", duemo => "ébas", tremo => "éba" },
                    plurale => { primo => "ebámo", duemo => "ebáte", tremo => "ébano" },
                },
                'passato semplice' => {
                    singulare => { primo => "fui", duemo => "fuis", tremo => "fuit" },
                    plurale => { primo => "fuémo", duemo => "fuéte", tremo => "fuéno" },
                },
                'futuro semplice' => {
                    singulare => { primo => "éro", duemo => "éras", tremo => "éra" },
                    plurale => { primo => "erámo", duemo => "eráte", tremo => "érano" },
                },
            },
            subjunctivo => {
                presente => {
                    singulare => { primo => "sim", duemo => "sis", tremo => "sit" },
                    plurale => { primo => "símus", duemo => "sítis", tremo => "sint" },
                },
                imperfecto => {
                    singulare => { primo => "ésse", duemo => "ésses", tremo => "éssi" },
                    plurale => { primo => "essémo", duemo => "esséte", tremo => "ésseno" },
                },
            },
            conditionale => {
                presente => {
                    singulare => { primo => "eréi", duemo => "eréis", tremo => "eréit" },
                    plurale => { primo => "eriémo", duemo => "eriéte", tremo => "eriéno" },
                },
            },
        },
    },
};

sub printdebug
{
    (my $filename, my $linenum, my $multi, my $name, my $dumpstr) = @_;

    my $str = "";

    if( $multi )
    {
        $str = sprintf (
                  ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
                . "DEBUG : %s(%d) : %-12s\n"
                . "%s\n"
                . "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"
                , $filename, $linenum, $name, $dumpstr );
    }
    else
    {
        $str = sprintf (
                  "DEBUG : %s(%d) : %-12s : %s\n",
                , $filename, $linenum, $name, $dumpstr );
    }

    print STDERR ( Encode::encode( 'UTF-8', $str ) );
}


# $mode : 変換モードを指定する。
#         "gr"  : 文字列全体を ASCII > GREEK に変換する。
#         "asc" : 文字列全体を GREEK > ASCII に変換する。
#         undef : タグ "!gr{...}" 内を ASCII > GREEK に、
#                 タグ "!asc{...}" 内を GREEK > ASCII に変換する。
sub translit
{
    my ( $str, $mode ) = @_;

    my $loopmax = 128;

    if( defined( $mode ) )
    {
        if( $mode eq "gr" )
        {
            $str = "!gr{" . $str . "}";
        }
        elsif( $mode eq "asc" )
        {
            $str = "!asc{" . $str . "}";
        }
        else
        {
            # 引数不正
            return( $str );
        }
    }

    ################################################################
    # !gr{ASCII} > GREEK
    ################################################################

    for( my $loop = 0; $loop < $loopmax && $str =~ /(!gr\{[^}]*)[bgdzuklmnjprswtfxcBGDZUKLMNJPRSWTFXC]([^}]*})/; $loop++ )
    {
        $str =~ s/(!gr\{[^}]*)b([^}]*})/$1β$2/g;   $str =~ s/(!gr\{[^}]*)B([^}]*})/$1Β$2/g;
        $str =~ s/(!gr\{[^}]*)g([^}]*})/$1γ$2/g;   $str =~ s/(!gr\{[^}]*)G([^}]*})/$1Γ$2/g;
        $str =~ s/(!gr\{[^}]*)d([^}]*})/$1δ$2/g;   $str =~ s/(!gr\{[^}]*)D([^}]*})/$1Δ$2/g;
        $str =~ s/(!gr\{[^}]*)z([^}]*})/$1ζ$2/g;   $str =~ s/(!gr\{[^}]*)Z([^}]*})/$1Ζ$2/g;
        $str =~ s/(!gr\{[^}]*)u([^}]*})/$1θ$2/g;   $str =~ s/(!gr\{[^}]*)U([^}]*})/$1Θ$2/g;
        $str =~ s/(!gr\{[^}]*)k([^}]*})/$1κ$2/g;   $str =~ s/(!gr\{[^}]*)K([^}]*})/$1Κ$2/g;
        $str =~ s/(!gr\{[^}]*)l([^}]*})/$1λ$2/g;   $str =~ s/(!gr\{[^}]*)L([^}]*})/$1Λ$2/g;
        $str =~ s/(!gr\{[^}]*)m([^}]*})/$1μ$2/g;   $str =~ s/(!gr\{[^}]*)M([^}]*})/$1Μ$2/g;
        $str =~ s/(!gr\{[^}]*)n([^}]*})/$1ν$2/g;   $str =~ s/(!gr\{[^}]*)N([^}]*})/$1Ν$2/g;
        $str =~ s/(!gr\{[^}]*)j([^}]*})/$1ξ$2/g;   $str =~ s/(!gr\{[^}]*)J([^}]*})/$1Ξ$2/g;
        $str =~ s/(!gr\{[^}]*)p([^}]*})/$1π$2/g;   $str =~ s/(!gr\{[^}]*)P([^}]*})/$1Π$2/g;
        $str =~ s/(!gr\{[^}]*)r([^}]*})/$1ρ$2/g;   $str =~ s/(!gr\{[^}]*)R([^}]*})/$1Ρ$2/g;
        $str =~ s/(!gr\{[^}]*)s([^}]*})/$1σ$2/g;   $str =~ s/(!gr\{[^}]*)S([^}]*})/$1Σ$2/g;
        $str =~ s/(!gr\{[^}]*)w([^}]*})/$1ς$2/g;   $str =~ s/(!gr\{[^}]*)W([^}]*})/$1Σ$2/g;
        $str =~ s/(!gr\{[^}]*)t([^}]*})/$1τ$2/g;   $str =~ s/(!gr\{[^}]*)T([^}]*})/$1Τ$2/g;
        $str =~ s/(!gr\{[^}]*)f([^}]*})/$1φ$2/g;   $str =~ s/(!gr\{[^}]*)F([^}]*})/$1Φ$2/g;
        $str =~ s/(!gr\{[^}]*)x([^}]*})/$1χ$2/g;   $str =~ s/(!gr\{[^}]*)X([^}]*})/$1Χ$2/g;
        $str =~ s/(!gr\{[^}]*)c([^}]*})/$1ψ$2/g;   $str =~ s/(!gr\{[^}]*)C([^}]*})/$1Ψ$2/g;
    }

    for( my $loop = 0; $loop < $loopmax && $str =~ /(!gr\{[^}]*)[iyIY]:([^}]*})/; $loop++ )
    {
        $str =~ s/(!gr\{[^}]*)i:([^}]*})/$1ϊ$2/g;  $str =~ s/(!gr\{[^}]*)I:([^}]*})/$1Ϊ$2/g;
        $str =~ s/(!gr\{[^}]*)y:([^}]*})/$1ϋ$2/g;  $str =~ s/(!gr\{[^}]*)Y:([^}]*})/$1Ϋ$2/g;
    }

    for( my $loop = 0; $loop < $loopmax && $str =~ /(!gr\{[^}]*)[aeioyhvAEIOYHV]~([^}]*})/; $loop++ )
    {
        $str =~ s/(!gr\{[^}]*)a~([^}]*})/$1ὰ$2/g;  $str =~ s/(!gr\{[^}]*)A~([^}]*})/$1Ὰ$2/g;
        $str =~ s/(!gr\{[^}]*)h~([^}]*})/$1ὴ$2/g;  $str =~ s/(!gr\{[^}]*)H~([^}]*})/$1Ὴ$2/g; $str =~ s/(!gr\{[^}]*)e~([^}]*})/$1ὴ$2/g; $str =~ s/(!gr\{[^}]*)E~([^}]*})/$1Ὴ$2/g;
        $str =~ s/(!gr\{[^}]*)i~([^}]*})/$1ὶ$2/g;  $str =~ s/(!gr\{[^}]*)I~([^}]*})/$1Ὶ$2/g;
        $str =~ s/(!gr\{[^}]*)v~([^}]*})/$1ὼ$2/g;  $str =~ s/(!gr\{[^}]*)V~([^}]*})/$1Ὼ$2/g; $str =~ s/(!gr\{[^}]*)o~([^}]*})/$1ὼ$2/g; $str =~ s/(!gr\{[^}]*)O~([^}]*})/$1Ὼ$2/g;
        $str =~ s/(!gr\{[^}]*)y~([^}]*})/$1ὺ$2/g;  $str =~ s/(!gr\{[^}]*)Y~([^}]*})/$1Ὺ$2/g;
    }

    for( my $loop = 0; $loop < $loopmax && $str =~ /(!gr\{[^}]*)[aeioyhvAEIOYHV]_([^}]*})/; $loop++ )
    {
        $str =~ s/(!gr\{[^}]*)a_([^}]*})/$1ᾱ$2/g;  $str =~ s/(!gr\{[^}]*)A_([^}]*})/$1Ᾱ$2/g;
        $str =~ s/(!gr\{[^}]*)h([^}]*})/$1η$2/g;   $str =~ s/(!gr\{[^}]*)H([^}]*})/$1Η$2/g;  $str =~ s/(!gr\{[^}]*)e_([^}]*})/$1η$2/g; $str =~ s/(!gr\{[^}]*)E_([^}]*})/$1Η$2/g;
        $str =~ s/(!gr\{[^}]*)i_([^}]*})/$1ῑ$2/g;  $str =~ s/(!gr\{[^}]*)I_([^}]*})/$1Ῑ$2/g;
        $str =~ s/(!gr\{[^}]*)v([^}]*})/$1ω$2/g;   $str =~ s/(!gr\{[^}]*)V([^}]*})/$1Ω$2/g;  $str =~ s/(!gr\{[^}]*)o_([^}]*})/$1ω$2/g; $str =~ s/(!gr\{[^}]*)O_([^}]*})/$1Ω$2/g;
        $str =~ s/(!gr\{[^}]*)y_([^}]*})/$1ῡ$2/g;  $str =~ s/(!gr\{[^}]*)Y_([^}]*})/$1Ῡ$2/g;
    }

    for( my $loop = 0; $loop < $loopmax && $str =~ /(!gr\{[^}]*)[aeioyAEIOY]\^([^}]*})/; $loop++ )
    {
        $str =~ s/(!gr\{[^}]*)a\^([^}]*})/$1ά$2/g; $str =~ s/(!gr\{[^}]*)A\^([^}]*})/$1Ά$2/g;
        $str =~ s/(!gr\{[^}]*)e\^([^}]*})/$1έ$2/g; $str =~ s/(!gr\{[^}]*)E\^([^}]*})/$1Έ$2/g;
        $str =~ s/(!gr\{[^}]*)i\^([^}]*})/$1ί$2/g; $str =~ s/(!gr\{[^}]*)I\^([^}]*})/$1Ί$2/g;
        $str =~ s/(!gr\{[^}]*)o\^([^}]*})/$1ό$2/g; $str =~ s/(!gr\{[^}]*)O\^([^}]*})/$1Ό$2/g;
        $str =~ s/(!gr\{[^}]*)y\^([^}]*})/$1ύ$2/g; $str =~ s/(!gr\{[^}]*)Y\^([^}]*})/$1Ύ$2/g;
    }

    for( my $loop = 0; $loop < $loopmax && $str =~ /(!gr\{[^}]*)[aeioyAEIOY]([^}]*})/; $loop++ )
    {
        $str =~ s/(!gr\{[^}]*)a([^}]*})/$1α$2/g;   $str =~ s/(!gr\{[^}]*)A([^}]*})/$1Α$2/g;
        $str =~ s/(!gr\{[^}]*)e([^}]*})/$1ε$2/g;   $str =~ s/(!gr\{[^}]*)E([^}]*})/$1Ε$2/g;
        $str =~ s/(!gr\{[^}]*)i([^}]*})/$1ι$2/g;   $str =~ s/(!gr\{[^}]*)I([^}]*})/$1Ι$2/g;
        $str =~ s/(!gr\{[^}]*)o([^}]*})/$1ο$2/g;   $str =~ s/(!gr\{[^}]*)O([^}]*})/$1Ο$2/g;
        $str =~ s/(!gr\{[^}]*)y([^}]*})/$1υ$2/g;   $str =~ s/(!gr\{[^}]*)Y([^}]*})/$1Υ$2/g;
    }

    $str =~ s/!gr\{([^}]*)}/$1/g;
    

    ################################################################
    # !asc{GREEK} > ASCII
    ################################################################

    for( my $loop = 0; $loop < $loopmax && $str =~ /(!asc\{[^}]*)[βγδζθκλμνξπρσςτφχψαειουάέίόύᾱηῑωῡὰὴὶὼὺΒΓΔΖΘΚΛΜΝΞΠΡΣΣΤΦΧΨΑΕΙΟΥΆΈΊΌΎᾹΗῙΩῩᾺῊῚῺῪ]([^}]*})/; $loop++ )
    {
        $str =~ s/(!asc\{[^}]*)β([^}]*})/$1b$2/g;   $str =~ s/(!asc\{[^}]*)Β([^}]*})/$1B$2/g;
        $str =~ s/(!asc\{[^}]*)γ([^}]*})/$1g$2/g;   $str =~ s/(!asc\{[^}]*)Γ([^}]*})/$1G$2/g;
        $str =~ s/(!asc\{[^}]*)δ([^}]*})/$1d$2/g;   $str =~ s/(!asc\{[^}]*)Δ([^}]*})/$1D$2/g;
        $str =~ s/(!asc\{[^}]*)ζ([^}]*})/$1z$2/g;   $str =~ s/(!asc\{[^}]*)Ζ([^}]*})/$1Z$2/g;
        $str =~ s/(!asc\{[^}]*)θ([^}]*})/$1u$2/g;   $str =~ s/(!asc\{[^}]*)Θ([^}]*})/$1U$2/g;
        $str =~ s/(!asc\{[^}]*)κ([^}]*})/$1k$2/g;   $str =~ s/(!asc\{[^}]*)Κ([^}]*})/$1K$2/g;
        $str =~ s/(!asc\{[^}]*)λ([^}]*})/$1l$2/g;   $str =~ s/(!asc\{[^}]*)Λ([^}]*})/$1L$2/g;
        $str =~ s/(!asc\{[^}]*)μ([^}]*})/$1m$2/g;   $str =~ s/(!asc\{[^}]*)Μ([^}]*})/$1M$2/g;
        $str =~ s/(!asc\{[^}]*)ν([^}]*})/$1n$2/g;   $str =~ s/(!asc\{[^}]*)Ν([^}]*})/$1N$2/g;
        $str =~ s/(!asc\{[^}]*)ξ([^}]*})/$1j$2/g;   $str =~ s/(!asc\{[^}]*)Ξ([^}]*})/$1J$2/g;
        $str =~ s/(!asc\{[^}]*)π([^}]*})/$1p$2/g;   $str =~ s/(!asc\{[^}]*)Π([^}]*})/$1P$2/g;
        $str =~ s/(!asc\{[^}]*)ρ([^}]*})/$1r$2/g;   $str =~ s/(!asc\{[^}]*)Ρ([^}]*})/$1R$2/g;
        $str =~ s/(!asc\{[^}]*)σ([^}]*})/$1s$2/g;   $str =~ s/(!asc\{[^}]*)Σ([^}]*})/$1S$2/g;
        $str =~ s/(!asc\{[^}]*)ς([^}]*})/$1w$2/g;
        $str =~ s/(!asc\{[^}]*)τ([^}]*})/$1t$2/g;   $str =~ s/(!asc\{[^}]*)Τ([^}]*})/$1T$2/g;
        $str =~ s/(!asc\{[^}]*)φ([^}]*})/$1f$2/g;   $str =~ s/(!asc\{[^}]*)Φ([^}]*})/$1F$2/g;
        $str =~ s/(!asc\{[^}]*)χ([^}]*})/$1x$2/g;   $str =~ s/(!asc\{[^}]*)Χ([^}]*})/$1X$2/g;
        $str =~ s/(!asc\{[^}]*)ψ([^}]*})/$1c$2/g;   $str =~ s/(!asc\{[^}]*)Ψ([^}]*})/$1C$2/g;

        $str =~ s/(!asc\{[^}]*)ϊ([^}]*})/$1i:$2/g;  $str =~ s/(!asc\{[^}]*)Ϊ([^}]*})/$1I:$2/g;
        $str =~ s/(!asc\{[^}]*)ϋ([^}]*})/$1y:$2/g;  $str =~ s/(!asc\{[^}]*)Ϋ([^}]*})/$1Y:$2/g;

        $str =~ s/(!asc\{[^}]*)ὰ([^}]*})/$1a~$2/g;  $str =~ s/(!asc\{[^}]*)Ὰ([^}]*})/$1A~$2/g;
        $str =~ s/(!asc\{[^}]*)ὴ([^}]*})/$1e~$2/g;  $str =~ s/(!asc\{[^}]*)Ὴ([^}]*})/$1E~$2/g;
        $str =~ s/(!asc\{[^}]*)ὶ([^}]*})/$1i~$2/g;  $str =~ s/(!asc\{[^}]*)Ὶ([^}]*})/$1I~$2/g;
        $str =~ s/(!asc\{[^}]*)ὼ([^}]*})/$1o~$2/g;  $str =~ s/(!asc\{[^}]*)Ὼ([^}]*})/$1O~$2/g;
        $str =~ s/(!asc\{[^}]*)ὺ([^}]*})/$1y~$2/g;  $str =~ s/(!asc\{[^}]*)Ὺ([^}]*})/$1Y~$2/g;

        $str =~ s/(!asc\{[^}]*)ᾱ([^}]*})/$1a_$2/g;  $str =~ s/(!asc\{[^}]*)Ᾱ([^}]*})/$1A_$2/g;
        $str =~ s/(!asc\{[^}]*)η([^}]*})/$1e_$2/g;  $str =~ s/(!asc\{[^}]*)Η([^}]*})/$1E_$2/g;
        $str =~ s/(!asc\{[^}]*)ῑ([^}]*})/$1i_$2/g;  $str =~ s/(!asc\{[^}]*)Ῑ([^}]*})/$1I_$2/g;
        $str =~ s/(!asc\{[^}]*)ω([^}]*})/$1o_$2/g;  $str =~ s/(!asc\{[^}]*)Ω([^}]*})/$1O_$2/g;
        $str =~ s/(!asc\{[^}]*)ῡ([^}]*})/$1y_$2/g;  $str =~ s/(!asc\{[^}]*)Ῡ([^}]*})/$1Y_$2/g;

        $str =~ s/(!asc\{[^}]*)ά([^}]*})/$1a\^$2/g; $str =~ s/(!asc\{[^}]*)Ά([^}]*})/$1A\^$2/g;
        $str =~ s/(!asc\{[^}]*)έ([^}]*})/$1e\^$2/g; $str =~ s/(!asc\{[^}]*)Έ([^}]*})/$1E\^$2/g;
        $str =~ s/(!asc\{[^}]*)ί([^}]*})/$1i\^$2/g; $str =~ s/(!asc\{[^}]*)Ί([^}]*})/$1I\^$2/g;
        $str =~ s/(!asc\{[^}]*)ό([^}]*})/$1o\^$2/g; $str =~ s/(!asc\{[^}]*)Ό([^}]*})/$1O\^$2/g;
        $str =~ s/(!asc\{[^}]*)ύ([^}]*})/$1y\^$2/g; $str =~ s/(!asc\{[^}]*)Ύ([^}]*})/$1Y\^$2/g;

        $str =~ s/(!asc\{[^}]*)α([^}]*})/$1a$2/g;   $str =~ s/(!asc\{[^}]*)Α([^}]*})/$1A$2/g;
        $str =~ s/(!asc\{[^}]*)ε([^}]*})/$1e$2/g;   $str =~ s/(!asc\{[^}]*)Ε([^}]*})/$1E$2/g;
        $str =~ s/(!asc\{[^}]*)ι([^}]*})/$1i$2/g;   $str =~ s/(!asc\{[^}]*)Ι([^}]*})/$1I$2/g;
        $str =~ s/(!asc\{[^}]*)ο([^}]*})/$1o$2/g;   $str =~ s/(!asc\{[^}]*)Ο([^}]*})/$1O$2/g;
        $str =~ s/(!asc\{[^}]*)υ([^}]*})/$1y$2/g;   $str =~ s/(!asc\{[^}]*)Υ([^}]*})/$1Y$2/g;
    }

    $str =~ s/!asc\{([^}]*)}/$1/g;


    return( $str );
}

sub deacc
{
    (my $str) = @_;

    # printf( "DEBUG : [%s]\n", encode_utf8( $str ) );

    $str = translit( $str, "asc" );

    # printf( "DEBUG : [%s]\n", encode_utf8( $str ) );

    $str =~ s/~/_/g;
    $str =~ s/\^//g;

    # printf( "DEBUG : [%s]\n", encode_utf8( $str ) );

    $str = translit( $str, "gr" );

    # printf( "DEBUG : [%s]\n", encode_utf8( $str ) );

    return( $str );
}

sub acc
{
    (my $str, my $force) = @_;

    $force = (not defined $force) ? 0 : $force;

    $str = deacc( $str );

    if( $force == 0 )
    {
        if( not $str =~ /((^|[^${vow}])+[${vow}]+){2}/ )
        {
            # 1音節の語は対象外
            return( $str );
        }
    }

    $str =~ s/^(.*[^${vow}${acc}])?[a]([${vow}${acc}]*[^${vow}${acc}]*)$/$1á$2/g;
    $str =~ s/^(.*[^${vow}${acc}])?[e]([${vow}${acc}]*[^${vow}${acc}]*)$/$1é$2/g;
    $str =~ s/^(.*[^${vow}${acc}])?[i]([${vow}${acc}]*[^${vow}${acc}]*)$/$1í$2/g;
    $str =~ s/^(.*[^${vow}${acc}])?[o]([${vow}${acc}]*[^${vow}${acc}]*)$/$1ó$2/g;
    $str =~ s/^(.*[^${vow}${acc}])?[u]([${vow}${acc}]*[^${vow}${acc}]*)$/$1ú$2/g;
    $str =~ s/^(.*[^${vow}${acc}])?[A]([${vow}${acc}]*[^${vow}${acc}]*)$/$1Á$2/g;
    $str =~ s/^(.*[^${vow}${acc}])?[E]([${vow}${acc}]*[^${vow}${acc}]*)$/$1É$2/g;
    $str =~ s/^(.*[^${vow}${acc}])?[I]([${vow}${acc}]*[^${vow}${acc}]*)$/$1Í$2/g;
    $str =~ s/^(.*[^${vow}${acc}])?[O]([${vow}${acc}]*[^${vow}${acc}]*)$/$1Ó$2/g;
    $str =~ s/^(.*[^${vow}${acc}])?[U]([${vow}${acc}]*[^${vow}${acc}]*)$/$1Ú$2/g;

    return( $str );
}

sub redice
{
    (my $str, my $cnt) = @_;

    $str =~ s/(\S){${cnt}}$//g;

    return( $str );
}

sub coda
{
    (my $str, my $cnt) = @_;

    $str =~ s/^.*(\S{${cnt}})$/$1/g;

    return( $str );
}

sub nomFondo
{
    (my $redice, my $classe) = @_;

    foreach my $decl ( @{$ruleRef->{'decl'}} )
    {
        if( $classe eq $decl->{'classe'} )
        {
            my %nomFondo = %{Storable::dclone( $decl )};

            foreach my $key1 (sort keys %nomFondo)
            {
                if( $key1 ne 'singulare' && $key1 ne 'plurale' )
                {
                    next;
                }

                foreach my $key2 (sort keys %{$nomFondo{$key1}})
                {
                    my $coda = $nomFondo{$key1}->{$key2};
                    $nomFondo{$key1}->{$key2} =
                        ( ($coda =~ /[${acc}]/) ? deacc($redice) : $redice ) . $coda;
                }
            }

            return( %nomFondo );
        }
    }

    return( undef );
}

sub nom
{
    (my $nucl) = @_;

    my $redice = $nucl;
    my $classe = 'n';

    # printf STDERR ( "nucl[$nucl]\n" );

    if( defined $irrRef->{'decl'}->{$nucl} )
    {
        # 不規則活用
        $redice = redice( $irrRef->{'decl'}->{$nucl}->{'nuclFormal'}, 1 );
        $classe = deacc( coda( $irrRef->{'decl'}->{$nucl}->{'nuclFormal'}, 1 ) );
        # printf STDERR ( "Irregular\n" );
    }
    else
    {
        # 規則活用
        if( deacc($nucl) =~ /[${vow}]$/ )
        {
            # 開音節
            $redice = redice( $nucl, 1 );
            $classe = deacc( coda( $nucl, 1 ) );
            # printf STDERR ( "Regular Open\n" );
        }
        else
        {
            # 閉音節
            $redice = $nucl;
            $classe = 'n';
            # printf STDERR ( "Regular Close\n" );
        }
    }

    # printf STDERR ( "redice[$redice] classe[$classe]\n" );

    my %nomFondo = nomFondo( $redice, $classe );

    if( defined $irrRef->{'decl'}->{$nucl} )
    {
        # 不規則活用

        foreach my $keyNumber (sort keys %{$irrRef->{'decl'}->{$nucl}})
        {
            if( $keyNumber ne 'singulare'
             && $keyNumber ne 'plurale' )
            {
                next;
            }

            foreach my $keyCase (sort keys %{$irrRef->{'decl'}->{$nucl}->{$keyNumber}})
            {
                # printf STDERR (
                #     "dst[%s] < src[%s]\n",
                #     $nomFondo{$keyNumber}->{$keyCase},
                #     $irrRef->{'decl'}->{$nucl}->{$keyNumber}->{$keyCase}
                # );
                $nomFondo{$keyNumber}->{$keyCase}
                    = $irrRef->{'decl'}->{$nucl}->{$keyNumber}->{$keyCase};
            }
        }
    }

    return( %nomFondo );
}

sub nomTable
{
    (my $nomRef, my $header) = @_;

    my $nomTableStr = "";

    $nomTableStr .=
        sprintf( "%-16s %s\n",
            "#", $nomRef->{'gender'} );

    foreach my $keyNumber ( @number )
    {
        foreach my $keyCase ( @case )
        {
            $nomTableStr .=
                sprintf( "%-16s %s\n",
                    sprintf( "%s,%s", $keyNumber, $keyCase ),
                    $nomRef->{$keyNumber}->{$keyCase} );
        }
    }

    return( $nomTableStr );
}

sub adj
{
    (my $nucl) = @_;

    my $coda = coda( deacc( $nucl ), 1 );
    my $redice = redice( $nucl, 1 );
    my %adj = ();

    # printdebug( __FILE__, __LINE__, 0, "\$nucl", $nucl );
    # printdebug( __FILE__, __LINE__, 0, "\$coda", $coda );
    # printdebug( __FILE__, __LINE__, 0, "\$redice", $redice );

    if( $coda eq 'o' )
    {
        my %nomA = nomFondo( $redice, 'a' );
        my %nomO = nomFondo( $redice, 'o' );
        my %nomE = nomFondo( $redice, 'e' );

        # printdebug( __FILE__, __LINE__, 1, "\%nomA", Dumper(\%nomA) );
        # printdebug( __FILE__, __LINE__, 1, "\%nomO", Dumper(\%nomO) );
        # printdebug( __FILE__, __LINE__, 1, "\%nomE", Dumper(\%nomE) );

        $adj{'male'}->{'singulare'}   = $nomO{'singulare'};
        $adj{'male'}->{'plurale'}     = $nomE{'plurale'};
        $adj{'female'}->{'singulare'} = $nomA{'singulare'};
        $adj{'female'}->{'plurale'}   = $nomA{'plurale'};
    }
    elsif( $coda eq 'e' )
    {
        my %nomE = nomFondo( $redice, 'e' );

        $adj{'male'}->{'singulare'}   = $nomE{'singulare'};
        $adj{'male'}->{'plurale'}     = $nomE{'plurale'};
        $adj{'female'}->{'singulare'} = $nomE{'singulare'};
        $adj{'female'}->{'plurale'}   = $nomE{'plurale'};
    }
    else
    {
        return( undef );
    }

    # printdebug( __FILE__, __LINE__, 0, "\$redice", $redice );
    # printdebug( __FILE__, __LINE__, 0, "\$classe", $classe );

    return( %adj );
}

sub adjTable
{
    (my $adjRef, my $header) = @_;

    my $adjTableStr = "";

    $adjTableStr .=
        sprintf( "%-16s %-16s%-16s\n",
            "#", 'male', 'female' );

    foreach my $keyNumber ( @number )
    {
        foreach my $keyCase ( @case )
        {
            $adjTableStr .=
                sprintf( "%-16s", sprintf( "%s,%s", $keyNumber, $keyCase ) );
            foreach my $keyGender ( @gender )
            {
                # printf STDERR ( "keyGender[%s] keyNumber[%s] keyCsae[%s]\n",
                #     $keyGender, $keyNumber, $keyCase );
                # printdebug( __FILE__, __LINE__, 1, "\$adjRef->{\$keyGender}",
                #     Dumper( $adjRef->{$keyGender} ) );

                $adjTableStr .=
                    sprintf( " %-16s",
                        $adjRef->{$keyGender}->{$keyNumber}->{$keyCase} );
            }
            $adjTableStr .= "\n";
        }
    }

    return( $adjTableStr );
}

sub verbFondo
{
    (my $redice, my $classe) = @_;

    # printf STDERR ( "\n=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>\n\n" );

    my $rediceDeacc = deacc( $redice );
    my $rediceAcc = ( ($redice =~ /[${acc}]/) ? $redice : acc($rediceDeacc,1) );

    printdebug( __FILE__, __LINE__, 0, "\$redice", $redice );
    printdebug( __FILE__, __LINE__, 0, "\$classe", $classe );
    printdebug( __FILE__, __LINE__, 0, "\$rediceDeacc", $rediceDeacc );
    printdebug( __FILE__, __LINE__, 0, "\$rediceAcc", $rediceAcc );

    foreach my $conj ( @{$ruleRef->{'conj'}} )
    {
        if( $classe eq $conj->{'classe'} )
        {
            # my %verbFondo = %{$conj};
            my %verbFondo = %{Storable::dclone( $conj )};

            # printdebug( __FILE__, __LINE__, 0, "\%verbFondo", Dumper(\%verbFondo) );

            foreach my $keyMood (sort keys %verbFondo)
            {
                if(
                    $keyMood eq 'infinito'
                 || $keyMood eq 'gerundio'
                 || $keyMood eq 'participio presente'
                 || $keyMood eq 'participio passato'
                )
                {
                    my $coda = $verbFondo{$keyMood};
                    $verbFondo{$keyMood} =
                        ( ($coda =~ /[${acc}]/) ? $rediceDeacc : $rediceAcc ) . $coda;

                    # printf STDERR ( "keyMood[$keyMood] redice[$redice] deacc(redice)[%s] coda[$coda]\n", deacc($redice) );
                    # printf STDERR ( "\$verbFondo{\$keyMood}[$verbFondo{$keyMood}]\n" );
                }
                elsif(
                    $keyMood eq 'indicativo'
                 || $keyMood eq 'subjunctivo'
                 || $keyMood eq 'conditionale'
                )
                {
                    foreach my $keyTempo (sort keys %{$verbFondo{$keyMood}})
                    {
                        my $finitivo = $verbFondo{$keyMood}->{$keyTempo};

                        if( $finitivo->{'tipo'} eq 'semplice' )
                        {
                            foreach my $keyNumber (sort keys %{$verbFondo{$keyMood}{$keyTempo}})
                            {
                                if( not ( $keyNumber eq 'singulare' ) && not ( $keyNumber eq 'plurale' ) )
                                {
                                    next;
                                }

                                foreach my $keyPersonalite (sort keys %{$verbFondo{$keyMood}{$keyTempo}{$keyNumber}})
                                {
                                    # printf STDERR ( "keyMood[$keyMood] keyTempo[$keyTempo] keyNumber[$keyNumber] keyPersonalite[$keyPersonalite]\n" );

                                    my $coda = $verbFondo{$keyMood}->{$keyTempo}->{$keyNumber}->{$keyPersonalite};
                                    $verbFondo{$keyMood}->{$keyTempo}->{$keyNumber}->{$keyPersonalite} =
                                        ( ($coda =~ /[${acc}]/) ? $rediceDeacc : $rediceAcc ) . $coda;
                                }
                            }
                        }
                    }
                }
            }

            # printdebug( __FILE__, __LINE__, 0, "\$redice", $redice );
            # printdebug( __FILE__, __LINE__, 0, "\$classe", $classe );
            # printdebug( __FILE__, __LINE__, 1, "\%verbFondo", Dumper(\%verbFondo) );
            # printf STDERR ( "\n=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<\n\n" );

            return( %verbFondo );
        }
    }

    # printf STDERR ( "\n=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<\n\n" );

    return( undef );
}

sub verb
{
    (my $nucl) = @_;

    my $redice = redice( $nucl, 3 );
    my $classe = coda( $nucl, 3 );

    # printdebug( __FILE__, __LINE__, 0, "\$nucl", $nucl );
    # printdebug( __FILE__, __LINE__, 0, "\$redice", $redice );
    # printdebug( __FILE__, __LINE__, 0, "\$classe", $classe );

    if( not $classe =~ /^[áéeí]re$/ )
    {
        return( undef );
    }

    my %verb = verbFondo( $redice, $classe );
    my %auxl = verbFondo( 'hab', 'ére' );

    if( defined( $irrRef->{'conj'}->{$nucl} ) )
    {
        foreach my $keyMood (sort keys %{$irrRef->{'conj'}->{$nucl}})
        {
            if(
                $keyMood eq 'infinito'
             || $keyMood eq 'gerundio'
             || $keyMood eq 'participio presente'
             || $keyMood eq 'participio passato'
            )
            {
                $verb{$keyMood} = $irrRef->{'conj'}->{$nucl}->{$keyMood};
            }
        }
    }

    foreach my $keyMood (sort keys %verb)
    {
        if(
            $keyMood eq 'indicativo'
         || $keyMood eq 'subjunctivo'
         || $keyMood eq 'conditionale'
        )
        {
            foreach my $keyTempo (sort keys %{$verb{$keyMood}})
            {
                my $finitivo = $verb{$keyMood}->{$keyTempo};

                if( $finitivo->{'tipo'} eq 'complexo' )
                {
                    # printf STDERR ( "keyMood[$keyMood] keyTempo[$keyTempo] auxiliareTempo[$finitivo->{'auxiliareTempo'}]\n" );

                    %{$verb{$keyMood}->{$keyTempo}} = (
                        %{$verb{$keyMood}->{$keyTempo}},
                        ( 'singulare' => Storable::dclone( $auxl{$keyMood}->{$finitivo->{'auxiliareTempo'}}->{'singulare'} ) ),
                        ( 'plurale' => Storable::dclone( $auxl{$keyMood}->{$finitivo->{'auxiliareTempo'}}->{'plurale'} ) ),
                    );

                    # printdebug( __FILE__, __LINE__, 1, "\$verb{\$keyMood}->{\$keyTempo}", Dumper( $verb{$keyMood}->{$keyTempo} ) );

                    foreach my $keyNumber (sort keys %{$verb{$keyMood}{$keyTempo}})
                    {
                        if( not ( $keyNumber eq 'singulare' ) && not ( $keyNumber eq 'plurale' ) )
                        {
                            next;
                        }

                        foreach my $keyPersonalite (sort keys %{$verb{$keyMood}{$keyTempo}{$keyNumber}})
                        {
                            # printf STDERR ( "keyMood[$keyMood] keyTempo[$keyTempo] keyNumber[$keyNumber] keyPersonalite[$keyPersonalite]\n" );

                            $verb{$keyMood}->{$keyTempo}->{$keyNumber}->{$keyPersonalite} .=
                                " " . $verb{'participio passato'};

                            # printdebug( __FILE__, __LINE__, 1, "aaa", $verb{$keyMood}->{$keyTempo}->{$keyNumber}->{$keyPersonalite} );
                        }
                    }
                }
            }
        }
    }

    if( defined( $irrRef->{'conj'}->{$nucl} ) )
    {
        foreach my $keyMood (sort keys %{$irrRef->{'conj'}->{$nucl}})
        {
            if(
                $keyMood eq 'indicativo'
             || $keyMood eq 'subjunctivo'
             || $keyMood eq 'conditionale'
            )
            {
                foreach my $keyTempo (sort keys %{$irrRef->{'conj'}->{$nucl}->{$keyMood}})
                {
                    foreach my $keyNumber (sort keys %{$irrRef->{'conj'}->{$nucl}->{$keyMood}->{$keyTempo}})
                    {
                        if( not ( $keyNumber eq 'singulare' ) && not ( $keyNumber eq 'plurale' ) )
                        {
                            next;
                        }

                        foreach my $keyPersonalite (sort keys %{$irrRef->{'conj'}->{$nucl}->{$keyMood}->{$keyTempo}->{$keyNumber}})
                        {
                            $verb{$keyMood}->{$keyTempo}->{$keyNumber}->{$keyPersonalite}
                                = $irrRef->{'conj'}->{$nucl}->{$keyMood}->{$keyTempo}->{$keyNumber}->{$keyPersonalite};
                        }
                    }
                }
            }
        }
    }

    return( %verb );
}

sub verbTable
{
    (my $verbRef, my $header) = @_;

    my $verbTableStr = "";

    $verbTableStr .=
        sprintf( "%-32s%-24s%-24s%-24s%-24s%-24s%-24s\n",
            "#", 'sg.1', 'sg.2', 'sg.3', 'pl.1', 'pl.2', 'pl.3' );

    foreach my $keyMood ( @mood )
    {
        if( $keyMood eq 'infinito'
         || $keyMood eq 'gerundio'
         || $keyMood eq 'participio presente'
         || $keyMood eq 'participio passato' )
        {
            $verbTableStr .= sprintf( "%-32s%-24s\n", $keyMood, $verbRef->{${keyMood}} );
        }
        elsif( $keyMood eq 'indicativo'
            || $keyMood eq 'subjunctivo'
            || $keyMood eq 'conditionale' )
        {
            foreach my $keyTempo ( @tempo )
            {
                if( not defined $verbRef->{$keyMood}->{$keyTempo} )
                {
                    next;
                }

                $verbTableStr .=
                    sprintf( "%-32s", sprintf( "%s,%s", $keyMood, $keyTempo ) );

                foreach my $keyNumber ( @number )
                {

                    foreach my $keyPersonalite ( @personalite )
                    {
                        $verbTableStr .=
                            sprintf( "%-24s",
                                $verbRef->{$keyMood}->{$keyTempo}->{$keyNumber}->{$keyPersonalite} );
                    }
                }

                $verbTableStr .= "\n";
            }
        }
    }

    return( $verbTableStr );
}

sub nomHtml
{
    (my $nomRef, my $htmlClass, my $htmlId) = @_;

    my $nomHtmlStr = "";

    $htmlClass = (not defined $htmlClass) ? "nom" : $htmlClass;
    $htmlId    = (not defined $htmlId   ) ? deacc( $nomRef->{'singulare'}->{'tem'} ) : $htmlId   ;

    $nomHtmlStr .= "<table class='" . $htmlClass . "' id='" . $htmlId . "'>\n";

    $nomHtmlStr .= "  <tr class='tableHead'>\n";
    $nomHtmlStr .= "    <td>number</td>\n";
    $nomHtmlStr .= "    <td>case</td>\n";
    $nomHtmlStr .= "    <td>" . $nomRef->{'gender'} . "</td>\n";
    $nomHtmlStr .= "  </tr>\n";

    foreach my $keyNumber ( @number )
    {
        my $caseNum = @case;
        my $keyCaseIndex = 0;

        foreach my $keyCase ( @case )
        {
            $nomHtmlStr .= "  <tr>\n";
            if( $keyCaseIndex == 0 )
            {
                $nomHtmlStr .= "    <td class='tableHead' rowspan='" . $caseNum . "'>" . $keyNumber . "</td>\n";
            }
            $nomHtmlStr .= "    <td class='tableHead'>" . $keyCase . "</td>\n";
            $nomHtmlStr .= "    <td>" . $nomRef->{$keyNumber}->{$keyCase} . "</td>\n";
            $nomHtmlStr .= "  </tr>\n";

            $keyCaseIndex++;
        }
    }

    $nomHtmlStr .= "</table>\n";

    return( $nomHtmlStr );
}

sub adjHtml
{
    (my $adjRef, my $htmlClass, my $htmlId) = @_;

    my $adjHtmlStr = "";

    $htmlClass = (not defined $htmlClass) ? "adj" : $htmlClass;
    $htmlId    = (not defined $htmlId   ) ? deacc( $adjRef->{'male'}->{'singulare'}->{'tem'} ) : $htmlId   ;

    $adjHtmlStr .= "<table class='" . $htmlClass . "' id='" . $htmlId . "'>\n";

    $adjHtmlStr .= "  <tr class='tableHead'>\n";
    $adjHtmlStr .= "    <td>number</td>\n";
    $adjHtmlStr .= "    <td>case</td>\n";
    foreach my $keyGender ( @gender )
    {
        $adjHtmlStr .= "    <td>" . $keyGender . "</td>\n";
    }
    $adjHtmlStr .= "  </tr>\n";

    foreach my $keyNumber ( @number )
    {
        my $caseNum = @case;
        my $keyCaseIndex = 0;

        foreach my $keyCase ( @case )
        {
            $adjHtmlStr .= "  <tr>\n";
            if( $keyCaseIndex == 0 )
            {
                $adjHtmlStr .= "    <td class='tableHead' rowspan='" . $caseNum . "'>" . $keyNumber . "</td>\n";
            }
            $adjHtmlStr .= "    <td class='tableHead'>" . $keyCase . "</td>\n";

            foreach my $keyGender ( @gender )
            {
                $adjHtmlStr .= "    <td>" . $adjRef->{$keyGender}->{$keyNumber}->{$keyCase} . "</td>\n";
            }

            $adjHtmlStr .= "  </tr>\n";
            $keyCaseIndex++;
        }
    }

    $adjHtmlStr .= "</table>\n";

    return( $adjHtmlStr );
}

sub verbHtml
{
    (my $verbRef, my $htmlClass, my $htmlId) = @_;

    my $verbHtmlStr = "";

    $htmlClass = (not defined $htmlClass) ? "verb" : $htmlClass;
    $htmlId    = (not defined $htmlId   ) ? deacc( $verbRef->{'infinito'} ) : $htmlId   ;

    my $numberNum = @number;
    my $personaliteNum = @personalite;

    $verbHtmlStr .= "<table class='" . $htmlClass . "' id='" . $htmlId . "'>\n";


    foreach my $keyMood ( @mood )
    {
        if( $keyMood eq 'infinito'
         || $keyMood eq 'gerundio'
         || $keyMood eq 'participio presente'
         || $keyMood eq 'participio passato' )
        {
            $verbHtmlStr .= "  <tr>\n";
            $verbHtmlStr .= "    <td class='tableHead'>" . $keyMood . "</td>\n";
            $verbHtmlStr .= "    <td colspan='" . ($numberNum * $personaliteNum) . "'>" . $verbRef->{$keyMood} . "</td>\n";
            $verbHtmlStr .= "  </tr>\n";
        }
        elsif( $keyMood eq 'indicativo'
            || $keyMood eq 'subjunctivo'
            || $keyMood eq 'conditionale' )
        {
            $verbHtmlStr .= "  <tr class='tableHead'>\n";
            $verbHtmlStr .= "    <td>" . $keyMood . "</td>\n";
            foreach my $keyNumber ( @numberDim )
            {
                foreach my $keyPersonalite ( @personaliteDim )
                {
                    $verbHtmlStr .= "    <td>" . $keyNumber . "." . $keyPersonalite . "</td>\n";
                }
            }
            $verbHtmlStr .= "  </tr>\n";
            foreach my $keyTempo ( @tempo )
            {
                if( not defined $verbRef->{$keyMood}->{$keyTempo} )
                {
                    next;
                }

                $verbHtmlStr .= "  <tr>\n";
                $verbHtmlStr .= "    <td class='tableHead'>" . $keyTempo . "</td>\n";
                foreach my $keyNumber ( @number )
                {
                    foreach my $keyPersonalite ( @personalite )
                    {
                        $verbHtmlStr .= "    <td>" . $verbRef->{$keyMood}->{$keyTempo}->{$keyNumber}->{$keyPersonalite} . "</td>\n";
                    }
                }
                $verbHtmlStr .= "  </tr>\n";
            }
        }
    }

    $verbHtmlStr .= "</table>\n";

    return( $verbHtmlStr );
}


1;
