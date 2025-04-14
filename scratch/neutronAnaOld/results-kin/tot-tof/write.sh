#!/bin/bash

root -b -q -l "WriteTH2D.C(\"hime.root\", \"hTotVsTof\", \"hTotVsTof.txt\")"
root -b -q -l "WriteTH2D.C(\"hime.root\", \"hTotVsTofLayer0\", \"hTotVsTofLayer0.txt\")"
root -b -q -l "WriteTH2D.C(\"hime.root\", \"hTotVsTofLayer1\", \"hTotVsTofLayer1.txt\")"
root -b -q -l "WriteTH2D.C(\"hime.root\", \"hTotVsTofLayer2\", \"hTotVsTofLayer2.txt\")"
