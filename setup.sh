#!/bin/bash

# NO NEED TO RUN THIS SCRIPT IF YOU ARE ON RIKEN MACHINE WITH ANAROOTLOGIN COMMAND (s063)
# FOR LOCAL USE ONLY

# deactivate conda environment as it may contains ROOT installation
while [ ! -z $CONDA_PREFIX ]; do
    conda deactivate
done

# CHANGE TO YOUR ROOT INSTALLATION PATH
source ~/software/root/bin/thisroot.sh
# CHANGE TO YOUR ANAROOT INSTALLATION PATH
export TARTSYS=$(pwd)/src/anaroot/install
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TARTSYS/lib