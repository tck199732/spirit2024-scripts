#!/usr/bin/env python3
import re
from pathlib import Path


current_dir = Path(__file__).resolve().parent.absolute()
source_dir = current_dir / 'himeRaw' 
out_dir = current_dir / 'hime'
out_dir.mkdir(exist_ok=True, parents=True)


# hime_xe136_data1790_24313232942.root
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

    # nov runs
    '2024-11-06',
    '2024-11-07',
    '2024-11-08',
    '2024-11-09',
    '2024-11-10',
]

data_dir_list = [source_dir / d for d in data_dir_list]

for data_dir in data_dir_list:
    hime_files = sorted([f for f in data_dir.glob("*.root")])
    for hime_file in hime_files:
        print(f'Linking {hime_file.name}')  
        try:
            runID = list(re.findall(r'\d+', hime_file.name))
            if len(runID) == 0:
                raise Exception('No runID found')
            elif len(runID) == 1:
                runID= int(runID[0])
            elif len(runID) == 3:
                runID = int(runID[1])
            else:
                raise Exception('RunID not found')
        except:
            print('unable to parse runID')
            continue

        symlink = out_dir / f'data{runID:04d}.root'
        if symlink.exists():
            symlink.unlink()
        symlink.symlink_to(hime_file)
        
