#!/bin/bash

# merging the data I analyzed and the old bdc file from Park (a few of them are updated due to broken files)
# this should eventually be replaced by the regenerated bdc files.

# 2025.3.28 : Please switch to bdc_3.12 files (updated by Park)

rikenDir="riken"
bdcDir="bdc"
# bdcDir="bdc_3.12"
outDir="riken_bdc"
mkdir -p $outDir

for fileName in $bdcDir/*.root; do
    idx=$(basename $fileName .root | sed 's/bdc_//')
    if [ ! -f $rikenDir/data$idx.root ]; then
        echo "Directory $rikenDir/data$idx does not exist. Skipping ..."
        continue
    fi

    # if [ -f $outDir/$idx.root ]; then
    #     echo "File $outDir/$idx.root already exists. Skipping ..."
    #     continue
    # fi

    echo "Processing File $bdcDir/bdc_$idx.root ..."
    root -b -l -q "macros/mergeRikenBdc.C( \
        $idx, \
        \"$bdcDir\", \
        \"$rikenDir\", \
        \"$outDir/$idx.root\" \
    )"
        
done
