#!/usr/bin/env python3
import re
import subprocess
from pathlib import Path


current_dir = Path(__file__).resolve().parent.absolute()
source_dir = current_dir / 'himeRaw' 
out_dir = current_dir / 'hime'
out_dir.mkdir(exist_ok=True, parents=True)

data_dir_list = [
    # commissioning runs
    '2024-06-17_sbt_shadow',
    '2024-06-17_shadow',

    # 6.27 - 7.1 physics runs
    '2024-06-27',
    '2024-06-28',
    '2024-06-29',
    '2024-06-30',
    '2024-07-01',
]
data_dir_list = [source_dir / d for d in data_dir_list]

for data_dir in data_dir_list:
    hime_files = sorted([f for f in data_dir.glob("*.root")])
    for hime_file in hime_files:
        try:
            runID = int(re.findall(r'\d+', hime_file.name)[0])
            print(f'Parsing runID {runID}')
        except:
            print('Error in parsing runID')
            continue

        symlink = out_dir / f'data{runID:04d}.root'
        print(f'Creating symlink {symlink} -> {hime_file}')
        if symlink.exists():
            print(f'{symlink} already exists')
            symlink.unlink()
            symlink.symlink_to(hime_file)
        
        
