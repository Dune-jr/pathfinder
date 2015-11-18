#!/bin/bash      

bam -c
rm config.lua
bam -j 2 solver=z3
