#!/usr/bin/env python3
import argparse
import subprocess
from pathlib import Path

current_path = Path(__file__).parent

ridf_dir = current_path / 'ridf'
root_dir = current_path / 'unpacked'
log_dir = current_path / 'logs' / 'unpacker'

root_dir.mkdir(exist_ok=True)
log_dir.mkdir(exist_ok=True)
macro = current_path / 'macros' / 'UnpackSamuraiData.C'

def main():

    args = get_arguments()
    ridf_files = []
    if args.all:
        ridf_files = list(ridf_dir.glob('*.ridf'))
    else:
        ridf_files = [
            ridf_dir / f'data{idx:04d}.ridf' for idx in range(args.start, args.end + 1)
        ]

    for ridf_file in ridf_files:

        if not ridf_file.exists():
            print(f'{ridf_file} does not exist, skipping...')
            continue
        
        file_size = ridf_file.stat().st_size
        if file_size < args.file_size_limit:
            print(f'{ridf_file} is too small ({file_size} bytes), skipping...')
            continue

        indata = str(ridf_file.relative_to(current_path))
        outdata = str((root_dir / f'{ridf_file.stem}.root').relative_to(current_path))
        print("outdata = ",outdata)
        logfile = str((log_dir / f'{ridf_file.stem}.log').relative_to(current_path))
        print("logfie=",logfile)
        if args.skip and Path(outdata).exists() and Path(outdata).stat().st_size > 0:
            print(f'{outdata} already exists, skipping...')
            continue

        print(f'Unpacking {ridf_file}...')

        if args.ignore_anaroot_error:
            cmd = f"printf '%s\n''n' | root -q -l -b \'{macro.relative_to(current_path)}(\"{indata}\", \"{outdata}\")\'"
        else:
            cmd = f'root -q -l -b \'{macro.relative_to(current_path)}(\"{indata}\", \"{outdata}\")\''

        if args.log:
            cmd += f' > {logfile} 2>&1'

        if args.debug:
            print(cmd)

        if not args.dry_run:
            subprocess.run(cmd, shell=True)

    print('\nAll done! :)))')
def get_arguments():
    argparser = argparse.ArgumentParser(description='Unpacks RIDF files to ROOT files.')
    argparser.add_argument('-d', '--debug', action='store_true', help='Print debug information')
    argparser.add_argument('--skip', action='store_true', help='Skip existing files')
    argparser.add_argument('--dry-run', action='store_true', help='Do not run the command')

    argparser.add_argument('--start', type=int, help='Start index', default=1036)
    argparser.add_argument('--end', type=int, help='End index', default=1036)
    argparser.add_argument('-a', '--all', action='store_true', help='Unpack all files in the directory, overrides --start and --end')

    # ridf data size = 1.8Kb corresponds to 0 event. 
    # data size smaller than this is not worth unpacking.
    argparser.add_argument('--file-size-limit', type=int, help='Minimum file size (bytes) for us to unpack.', default=0)

    argparser.add_argument('--disable-logging', action='store_false', help='Disable logging', dest='log')
    argparser.add_argument('--ignore-anaroot-error', action='store_true', help='Ignore anaroot error and continue unpacking')

    return argparser.parse_args()


if __name__ == '__main__':
    main()
