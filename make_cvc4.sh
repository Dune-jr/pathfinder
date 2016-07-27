#!/bin/bash      

rm config.lua
bam -c
bam -a -j 2 otawa=2 solver=cvc4 Wall=true

