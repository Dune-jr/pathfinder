#!/bin/bash      

rm config.lua
bam -c solver=z3
bam -a -r bsc -j 2 otawa=2 Wall=true

