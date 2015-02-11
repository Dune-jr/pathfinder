use strict;
use warnings;

@ARGV == 1 or die "Usage: $0 number\n";

my ($exp) = @ARGV;
eval { $exp = $exp+0; 1; } or die "Error: expecting a number\n";

print "/* Program with ".($exp % 2 == 0 ? "more than ":"")."half of its path feasible.\n";
print "   Contains 2^$exp syntactic paths. */\n";
print "int a[$exp];\n";
print "int main(int argc, char *argv[]) {\n";
print "  int x = 1;\n";
print "  int tmax = ".int($exp/2).";\n";

print "  int " unless $exp==0;
for(my $i = 1; $i <= $exp; $i = $i+1) {
    print "c$i";
    if ($i < $exp) {
	print ", ";
    } else {
	print ";\n";
    }
}


for(my $i = 1; $i <= $exp; $i = $i+1) {
    print "  if (c$i && tmax) { x+=$i; a[".($i-1)."]=x%$exp; tmax--; }\n";
}

print "  return x;\n";
print "}\n";

