# profevts.pl
# Thierry Fevrier 25-Feb-2000
#
# Perl script to generate IA64 profiling events.
#
# Caveats: 
#   sorry but it is my first perl script... 
#   and it was done very quickly to satisfy the needs
#   of a simple program.
#

require 5.001;

# Forward declarations
sub Usage;
sub PrintProlog;
sub PrintBody;
sub HalpProfileMappingToSource;
sub PrintEpilog;

# Contants

my $SCRIPT_VERSION     = "1.00";
my $SCRIPT_CMD         = "profevts.pl";
my $SCRIPT_LONGVERSION = "PROFEVTS.PL Version $SCRIPT_VERSION";
my $typedstr           = "HalpProfileMapping";

if ($#ARGV != 0)
{
    $error = "requires 1 arguments...";
    Usage($error);
}
my $filename = $ARGV[0];
if (-e $filename && -T $filename)
{
    open(FH, "<$filename") || die "$SCRIPT_CMD: could not open $filename...\n";
}
else 
{
    $error = "$filename does not exist or is not a text file...";
    Usage($error);
}

while ( <FH> )
{
    # look for the specified string
    if ( ($_ =~ /$typedstr/) && ($_ =~ /\[.*\]/) && ($_ =~ /\{/) )   {
       chop $_;
       PrintProlog( $typedstr );
       if ( PrintBody( ) )
       {
          PrintEpilog( $typedstr );
          last;
       }
       else  
       {
          print "Parsing failed...\n";
       }
    }
}
close(FH);
exit 0;

sub PrintBody
{
# Note: in_comment is really for the section defining the structure.
#       I do not handle the case if the structure is inside a comment block.
    my $in_comment = 0;
    my $index = 0;

LINE:
while (<FH>)
{
    local($line) = $_;
#print $line;

    if ( $line =~ /^\s*#.*$/ )   {
        chop $line;
#print "Found pre-processor macro \"$line\" in $typedstr...\n";
        print "$line\n";
        next LINE;
    }

    local($line) = $_;
    if ( $in_comment )   {
       # Does this line have the end of the C comment?
       #
       if ($line =~ /\*\//)
       {
          # Yes. Keep everything after the end of the
          # comment and keep going with normal processing

          $line = $';
          $in_comment = 0;
       }
       else
       {
          next LINE;
       }
    }
    # Remove single line C "/* */" comments
    $line =~ s/\/\*.*?\*\///g;

    # Remove any "//" comments
    # Make sure the start of the comment is NOT
    # inside a string
    if (($line =~ /\/\//) && ($line !~ /\".*?\/\/.*?\"/))
    {
       $line =~ s/\/\/.*$/ /;
    }

    # Multi-line C comment?
    # Make sure the start of the comment is NOT
    # inside a string
    if (($line =~ /\/\*/) && ($line !~ /\".*?\/\*.*?\"/))
    {
       # Grab anything before the comment
       # Need to make it look like there's still a EOL marker
       $line = $` . "\n";

       # Remember that we're in "comment" mode
       $in_comment = 1;

       next LINE;
    }

    local($line_pack) = $line;

    # Replace spaces between word characters with '#'
    $line_pack =~ s/(\w)\s+(\w)/$1#$2/g;

    # remove whitespace
    $line_pack =~ tr/ \t//d;

    # Remove quoted double-quote characters
    $line_pack =~ s/'\\?"'/'_'/g;

    # Remove any strings
    # Note: Doesn't handle quoted quote characters correctly
    $line_pack =~ s/"[^"]*"/_/g;

    # Remove any "//" comments
    $line_pack =~ s/\/\/.*$//;

    # For empty lines,
    if ( $line_pack eq "\n" )
    {
        next LINE;
    }

    if ( $line_pack =~ /^\}/)
    {
        return $index;
    }

    # Remove beginning "{"
    $line_pack =~ s/^\{(.*)\}.*$/$1/;

# print "line_pack: $line_pack\n";

    @words = split(/,/, $line_pack);
    HalpProfileMappingToSource( $index, @words );

    $index++;
    next LINE;
}

return 0;

}

sub HalpProfileMappingToSource
{
    local($idx, $supported, $event, $profileSource, $profileSourceMask, $interval, $defInterval, $maxInterval, $minInterval) = @_;
    if ( $idx eq 0 )
    {
       print "   \{ \"ProfileTime\", ProfileTime, \"ProfileTime\", $defInterval, $defInterval \}";
    }
    elsif ( $supported eq "FALSE" )  {
       print ",\n   \{ \"INVALID_$idx\", ProfileIA64Maximum, \"INVALID_$idx\", 0, 0 \}";
    }
    else {
       print ",\n   \{ \"$event\", Profile$event, \"$event\", $defInterval, 0 \}";
    }
}

sub PrintProlog
{
    local($str) = @_;
    local($basename) = $filename;
    local($header)   = $filename;
    local($arrayname);
    $basename  =~ s/.*\\(.*).[ch]/$1/;
    $arrayname = $basename . "StaticSources";
    if ($header !~ /\.h/)
    {
        $header =~ s/\.c$/\.h/;
    }
    print "\/\/\n";
    print "\/\/ This file is automatically generated by $SCRIPT_CMD, parsing\n";
    print "\/\/ $filename.\n";
    print "\/\/\n\n";
    print "\#include \"$header\"\n\n";
    print "SOURCE\n";
    print "$arrayname\[\] = \{\n";
}

sub PrintEpilog
{
    local($str)  = @_;
    print "\n};\n";
}

sub Usage
{
    local($error) = @_;

    die "$error\n",
        "$SCRIPT_LONGVERSION\n",
        "Usage  : $SCRIPT_CMD filename\n",
        "Options:\n",
        "    filename      file containing the IA64 $typedstr definition\n";
}  

