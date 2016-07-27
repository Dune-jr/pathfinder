#!/bin/bash      

rm config.lua
bam -c
bam -a -j 2 otawa=2 solver=z3 Wall=true

