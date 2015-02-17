# Makefile name
M:=Makefile
# Script directory
SC:=script
# ARM compiler
ARMCC:=arm-linux-gnueabi-gcc -O0 -nostartfiles -march=armv5

# No implicit rules
MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

# By default: displays the 
all:
	@echo 'Potential targets:'
	@echo '- sparse<N>.<ext>'
	@echo '- allButOne<N>.<ext>'
	@echo '- atMostOneThen<N>.<ext>'
	@echo '- halfOfThen<N>.<ext>'
	@echo 'Where'
	@echo '  <N> is a number'
	@echo '  <ext> is:'
	@echo '    .c for the C file'
	@echo '    .c.cat for displaying the C file'
	@echo '    .arm for the ARM program'
	@echo '    .arm.dot for the CFG of the ARM program'
	@echo '    .arm.xml for the flow facts on the CFG of the ARM program'
	@echo '    .arm.xml.cat for displaying the flow fact'
	@echo '    .arm.dot.xdot for displaying the CFG'

# Do not delete intermediate files
.SECONDARY:

## Kinds of programs generated

allButOne%.c: $(SC)/gen_allButOne.pl $(M)
	perl $< $* > $@

atMostOneThen%.c: $(SC)/gen_atMostOneThen.pl $(M)
	perl $< $* > $@

halfOfThen%.c: $(SC)/gen_halfOfThen.pl $(M)
	perl $< $* > $@

sparse%.c: $(SC)/gen_dragon.pl $(M)
	perl $< $* 1 > $@

# Compiling programs into ARM
%.arm: %.c $(M)
	$(ARMCC) -g -static $< -o $@

# Creating the dot file for a program
%.arm.dot: %.arm $(M)
	dumpcfg -Dids $< main > $@

## Making the FFX files (extension XML to avoid automatic load by dumpcfg)

allButOne%.arm.xml:
	$(error Cannot build $@ automatically)

atMostOneThen%.arm.xml:
	$(error Cannot build $@ automatically)

halfOfThen%.arm.xml: halfOfThen%.arm.dot $(SC)/ffx_halfOfThen.pl $(M)
	perl $(SC)/ffx_halfOfThen.pl $* $< | xmllint - > $@

sparse%.arm.xml: sparse%.arm.dot $(SC)/ffx_dragon.pl $(M)
	perl $(SC)/ffx_dragon.pl 1 $< | xmllint - > $@

%_gen.ffx: %.arm.xml $(M)
	cp $< $@

# Displaying something with cat
.PHONY: %.cat
%.cat: %
	cat $*

# Displaying something with xdot
.PHONY: %.xdot
%.xdot: %
	xdot $*

# Cleaning
.PHONY: clean
clean:
	rm -rf allButOne?*.c atMostOneThen?*.c halfOfThen?*.c sparse?*.c $(SC)/*~ *~ *.arm *.arm.dot *.arm.xml
