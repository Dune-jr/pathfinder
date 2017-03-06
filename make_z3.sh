#!/bin/bash      

rm config.lua
bam -c solver=z3
bam config
bam -a -r bsc -j 2 Wall=true

