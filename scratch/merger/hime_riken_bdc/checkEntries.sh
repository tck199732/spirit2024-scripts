#!/bin/bash

for idx in $(seq 1601 1918); do
    root -b -l -q -e ".x macros/checkEntries.C($idx)"
done