use strict;
use warnings;

@ARGV == 1 or die "Usage: $0 number\n";

my ($exp) = @ARGV;
eval { $exp = $exp+0; 1; } or die "Error: expecting a number\n";

print "/* Program with $exp+1 (expensive?) infeasible path.\n";
print "   Contains 2^$exp syntactic paths. */\n";
print "int main(int argc, char *argv[]) {\n";
print "  int x = 0;\n";

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
    if ($i < $exp) {
	print "  if (c$i) { x++; }\n";
    } else {
	print "  if (!(";
	for(my $j = 1; $j <= $exp; $j = $j+1) {
	    if ($j < $exp) {
		print "c$j && ";
	    } else {
		print "c$j)";
	    }
	}
	print ") { x++; }\n";
    }
}

print "  return x;\n";
print "}\n";

