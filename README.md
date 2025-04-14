# Scripts for Spirit 2024 experiment at RIKEN

This repository stores useful scripts and ROOT macros for unpacking and analyzing data. 

## directories
- [macros](./) : stores ROOT macros for unpacking raw data in `ridf` format and perform simple analysis. `Anaroot` source files are not uploaded here. 

- [database](./database/) : stores useful data files for the experiment.

- [logs](./logs/) : stores log files generated when unpacking `ridf` files.

- [scratch](./scratch/) : further analysis happens here
    - [ana](./scratch/ana) : converts unpacked data to ROOT files with observables such as `tof, tdiff, multi, ...`
    - [merger](./scratch/merger/) : merging analyzed data from [ana](./scratch/ana) and data from [`tDiff`](https://github.com/MarcoKnoesel/himeAna) from Hime detector. 
    - [rateCheck](./scratch/rateCheck/) : a simple calculation for downscaler factor based on kyoto multiplicities in run `1045`.
    - [neutronAnaOld](./scratch/neutronAnaOld/) : Old analysis scripts used in June. 
    - [neutronAna](./scratch/neutronAna/) : Updated analysis scripts used in June. 

## Instruction

### 0. Start
---------
Copy the required files or clone the repository `git clone https://github.com/tck199732/spirit2024-scripts.git`. Assume you are running the scripts on `s063@ridfana04`, run `anarootlogin ${your_account_name}` to source everything you need. 

### 1. Unpacker
----------------
The first step is to unpack the `ridf` files using the macro located at [`macros/UnpackSamuraiData.C`](./macros/UnpackSamuraiData.C). Create symlinks to the directory containing the data and output in your machine. 
``` {bash}
ln -s ${your_ridf_directory} ridf
ln -s ${your_output_directory} unpacked
```
To unpack multiple files, use the script [`unpacker.py`](./unpack.py). 

``` {bash}
# run `./unpack.py --help` to see options
./unpack.py --start 1036 --end 1048
```

By default, log files are produced in `./log/unpacked/data${idx}.log`. If there is any issue, there will be error message such as `RIDF:FrameError`. In such case, the data should not be analyzed. In commissioning run, there is error from `babieeasiroc`

```
RIBFROOT-Info : [/home/s063/svn/2024SAMURAI.s063/sources/Core/src/TArtDecoderVmeEasyroc.cc] Unknown data 0
```

Such error occurs in only a few events and those events are skipped in the unpacker. To check error in all log files, see [./check.py](./logs/unpacker/check.py) 

### 2. Ana
---------
The next step is to process the unpacked file to get observables.Nagvigate to [`scratch/ana`](./scratch/ana), compile the program with `make`. This creates the executable `ana.exe`. (Again, create the symlink or create directory for your outputs.) To analyze multiple files generated in `step 1`, run 
``` {bash}
ln -s ${output_location} analyzed
./a.sh ${start-run-idx} ${end-run-idx}
```
This produces ROOT files with observables such as `tdiff, tof, multi, detectorId, scaler ...` stored in [`analyzed`](./scratch/ana/analyzed/).

### 3. Merging 
-----------------
In `scratch/merger/riken_bdc`, bdc files generated from Park are merged to the analyzed data in last step. (They should have the same number of events and all timestamps should match.)

Now, we need to merge the data from samurai daq and hime daq. To do this, first we copy files from the Hime side. This is done in [`scratch/merger/hime_riken_bdc/hime`](./scratch/merger/hime_riken_bdc/hime/). If you are on shift, you can run `link_himefiles.py` to create sym link to the hime data files. 

Next, compile the merger program in the merger directory, i.e. `make`. Run the `a.py` script to merge files according to time stamp. Modify the `runId` in the script and simply `./a.py`. Pay attention that there is a few fall runs where the run number is out of sync with riken daq since the daq crashed. A short fix is in `a.py`.

Users are suggested to check the merging by running the executable on single run number with verbose mode `./merge.exe -i ${hime-file} -s ${samurai-file} --verbose`. In run `1150`, for example, the merging is not always complete, below shows part of the output 

```
...
...
...

skipping event.
diff > window : 18446744041425467180 at hime 35294 , samurai 23480 , skipping event.
diff > window : 18446744041424106714 at hime 35295 , samurai 23480 , skipping event.
diff > window : 18446744041419285633 at hime 35296 , samurai 23480 , skipping event.
entriesHime = 35297     entriesSamurai = 33876  mergedEvents = 23480
```

### 4. analysis
Further analysis can be done on the merged files, see [`neutronAna`](./scratch/neutronAna/macros). The most useful one is `shadowAna.C` which analyzes the hit in all detectors. A list of useful runs can be found in the bash scripts `runShadowCounts.sh`. To get the 1D spectra for the shadows of specific modules, check the jupyter notebook in the directory `plottings`. To use them, simply source root installation and then activate your python environment.

