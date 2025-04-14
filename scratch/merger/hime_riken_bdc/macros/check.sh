#!/bin/bash

ori_dir=./taras-marco
cp_dir=./

for idx in $(seq 1036 1048); do
    ori_file=$ori_dir/data$idx.root
    cp_file=$cp_dir/data$idx.root
    root -l -b -q "checkEntries.C(\"$cp_file\", \"$ori_file\")"
done


