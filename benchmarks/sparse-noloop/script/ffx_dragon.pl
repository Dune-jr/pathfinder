use strict;
use warnings;

@ARGV == 2 or die "Usage: $0 number file\n";

my ($max,$file) = @ARGV;
($max eq int($max)) or die "Error: expecting a number as first argument. Found $max.\n";

print STDERR "Building FFX file using $file (at most ".$max." \"then\" taken)\n";

# Parsing dot
open(my $fh, "<", $file) or die "Cannot open file ".$file."\n";
my %addrs=();
my %weight=();
my %decision=();
my @edges=(); # Not "taken"
my ($loop_file,$loop_line,$loop_count);
while(my $line = <$fh>) {
    if(my ($bb,$addr,$txt) = $line =~ /{BB (\d+) \(0*([0-9a-f]+)\) \| (.*)}/) {
	$addrs{$bb}=$addr;
	$weight{$bb}=length($txt);
	if(my ($jump) = $txt =~ /0*([0-9a-f]+)\W+bge/) {
	    $decision{$bb} = $jump;
	} elsif (my ($filename,$linenum,$bound) = $txt =~ /(sparse[0-9]+\.c):([0-9]+).*?#([0-9]+)\\l0*[0-9a-f]+\W+ble/) {
	    $loop_file = $filename;
	    $loop_line = $linenum;
	    $loop_count = $bound;
	}
    } elsif(my ($src,$dst) = $line =~ /"(\d+)" -> "(\d+)" \[weight=4\];/) {
	my $ed = [$src,$dst];
	push(@edges,$ed);
    }
}

# Pre-analysis
defined($loop_count) or warn "No loop was found. Skipping additional checks.\n";
my $tcount = 0;
foreach my $ed (@edges) {
    my ($src,$dst) = @$ed;
    if(exists $decision{$src}) {
	$tcount++;
    }
}
print STDERR "Found ".keys(%addrs)." basic blocks, ".@edges." direct edges and $tcount \"then\" branches\n";
my $exp_dir = 3 * $tcount + 2;
$exp_dir == @edges or not defined($loop_count) or die "Expecting $exp_dir direct edges. Something must have gone wrong.\n";
my $exp_bb = $exp_dir + 2;
$exp_bb == keys(%addrs) or not defined($loop_count) or die "Expecting $exp_bb basic blocks. Something must have gone wrong.\n";

# Outputting xml
print("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
print("<flowfacts>\n");
print("  <function name=\"main\">\n");
print("    <loop loopId=\"1\" line=\"".$loop_line."\" source=\"".$loop_file."\" exact=\"true\" maxcount=\"".$loop_count."\" totalcount=\"".$loop_count."\">\n") if defined($loop_count);
print("      <iteration number=\"*\">\n") if defined($loop_count);
print("        <control-constraint>\n");
print("          <le>\n");
print("            <add>\n");
foreach my $ed (@edges) {
    my ($src,$dst) = @$ed;
    if(exists $decision{$src}) {
	print("              <count src=\"0x".$addrs{$src}."\" dst=\"0x".$addrs{$dst}."\"/>\n");
    }
}
print("            </add>\n");
print("            <const int=\"".$max."\"/>\n");
print("          </le>\n");
print("        </control-constraint>\n");
print("      </iteration>\n") if defined($loop_count);
print("    </loop>\n") if defined($loop_count);
print("  </function>\n");
print("</flowfacts>\n");
