#!/usr/bin/env python3
import re
import subprocess
import argparse
from pathlib import Path

CurrentDir = Path(__file__).parent.resolve()
SamuraiDir = CurrentDir / 'samurai'
HimeDir = CurrentDir / 'hime'
MergeDir = CurrentDir / 'merged'
LogDir = CurrentDir / 'logs'
ExePath = CurrentDir / 'merge.exe'
MergeDir.mkdir(exist_ok=True, parents=True)

##########################################################
# riken ID -> hime ID
# created by first making (hime ID -> riken ID) and then invert it.
run_mapping = {
    i : i + 7 for i in range(1722, 1742)
}

for i in range(1742, 1781):
    run_mapping[i] = i + 8

run_mapping = {v : k for k, v in run_mapping.items()}
##########################################################

def is_dummy(idx, run_mapping):
    return idx in run_mapping.values() and idx not in run_mapping.keys()

def main():

    args = parse_args()
    samurai_files = [
        SamuraiDir / f'{idx:04d}.root' for idx in range(args.start, args.end+1) if not is_dummy(idx, run_mapping)
    ]

    hime_id = [
        idx if idx not in run_mapping.keys() else run_mapping[idx] for idx in range(args.start, args.end+1) if not is_dummy(idx, run_mapping)
    ]

    hime_files = [HimeDir / f'data{idx:04d}.root' for idx in hime_id]

    for hime_file, samurai_file in zip(hime_files, samurai_files):
        if (not hime_file.exists()) or (not samurai_file.exists()):
            print(f'File not found: {hime_file} or {samurai_file}')
            continue
        
        print(f'Merging {hime_file} and {samurai_file}')
        cmd = [
            ExePath,
            '--hime-filename', hime_file,
            '--samurai-filename', samurai_file,
            '--output-filename', MergeDir / f'{samurai_file.stem}.root',

            '>', LogDir / f'{samurai_file.stem}.log 2>&1'
        ]
        cmd = ' '.join(list(map(str, cmd)))
        subprocess.run(cmd, shell=True)

def parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument('--start', type=int, default=1036, help='Start run number (RIKEN-DAQ)')
    parser.add_argument('--end', type=int, default=1048, help='End run number (RIKEN-DAQ)')
    parser.add_argument('--hime-all', action='store_true', help='Merge all run existing in HIME, ignoring start and end')
    return parser.parse_args()

def run_Id(filename):
    return int(re.findall(r'\d+', str(filename))[0])

if __name__ == '__main__':
    main()
