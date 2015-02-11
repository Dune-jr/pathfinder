use strict;
use warnings;

@ARGV == 2 or die "Usage: $0 number file\n";

my ($exp,$file) = @ARGV;
eval { $exp = $exp+0; 1; } or die "Error: expecting a number\n";

my $max = int($exp/2);

print STDERR "Building FFX file using $file (at most ".$max." over ".$exp.")\n";

open(my $fh, "<", $file) or die "Cannot open file ".$file."\n";
print("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
print("<flowfacts>\n");
print("  <function name=\"main\">\n");
print("    <control-constraint>\n");
print("      <le>\n");
print("        <add>\n");
my %addrs=();
my %weight=();
my @edges=();
while(my $line = <$fh>) {
    if(my ($bb,$addr,$txt) = $line =~ /{BB (\d+) \(0*([0-9a-f]+)\) \| (.*)}/) {
	$addrs{$bb}=$addr;
	$weight{$bb}=length($txt);
    } elsif(my ($src,$dst) = $line =~ /"(\d+)" -> "(\d+)" \[weight=4\];/) {
	my $ed = [$src,$dst];
	push(@edges,$ed);
    }
}
my $tcount = 0;
foreach my $ed (@edges) {
    my ($src,$dst) = @$ed;
    if ($src != 1 && $weight{$dst} >= 300) {
	$tcount++;
	print("          <count src=\"0x".$addrs{$src}."\" dst=\"0x".$addrs{$dst}."\"/>\n");
    }
}
print STDERR "Found ".keys(%addrs)." basic blocks, ".@edges." direct edges and $tcount \"then\" branches\n";
$tcount == $exp or die "Expecting $exp \"then\" branches. Something must have gone wrong.\n";
print("        </add>\n");
print("        <const int=\"".$max."\"/>\n");
print("      </le>\n");
print("    </control-constraint>\n");
print("  </function>\n");
print("</flowfacts>\n");

