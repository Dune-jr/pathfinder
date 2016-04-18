#!/bin/bash      

bam -c
rm config.lua
bam -j 2 otawa=2 solver=cvc4 Wall=true

