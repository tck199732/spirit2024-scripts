import os
import sys
import json
import numpy as np
import pandas as pd
import itertools
import matplotlib as mpl
import matplotlib.pyplot as plt
from pathlib import Path
from collections import OrderedDict
from scipy.optimize import curve_fit

MPL_CUSTOM = {
    'font.family': 'serif',
    'mathtext.fontset': 'cm',
    'figure.dpi': 120,
    'figure.figsize': (4, 3.5),
    'figure.facecolor': 'white',
    'xtick.top': True,
    'xtick.direction': 'in',
    'xtick.minor.visible': True,
    'ytick.right': True,
    'ytick.direction': 'in',
    'ytick.minor.visible': True,
}

for k, v in MPL_CUSTOM.items():
    mpl.rcParams[k] = v


def main():

    n_layers = 3
    n_modules = 72
    bar_width = 0.04
    bar_gap = 0.002
    n_modules_per_layer = n_modules // n_layers


    # first layer
    x = []
    xtop = n_modules_per_layer // 2 * bar_width + (n_modules_per_layer // 2 - 0.5) * bar_gap
    for i in range(n_modules_per_layer):
        x.append(
            xtop - i * (bar_width + bar_gap) - bar_width / 2
        )

    # second layer
    xright = xtop
    for i in range(n_modules_per_layer):
        x.append(
            xright - i * (bar_width + bar_gap) - bar_width / 2
        )

    # third layer
    for i in range(n_modules_per_layer):
        x.append(
            xtop - i * (bar_width + bar_gap) - bar_width / 2
        )

    
    df = pd.DataFrame({
        'id' : np.arange(n_modules),
        'layer_id' : np.concatenate([[i] * n_modules_per_layer for i in range(n_layers)]),
        'x' : np.array(x),
    })            

    # add a title in the first row, 
    df.to_csv('positions2.dat', index=False, sep=' ', float_format='%.3f')
    

    # fig, ax = plt.subplots(1,1,dpi=300,figsize=(4.5,3.5), constrained_layout=True)
    # df_layer = df[df['layer_id'] == 0]
    
    # print(df_layer)
    # y0 = df_layer['x'].values * 1000
    # x0 = -500


    # ax.barh(y0[:2], 1000, height=bar_width * 1000, color='gray', alpha=0.9, left=x0, edgecolor='w', linewidth=0.5)

    # ax.barh(y0[2:], 1000, height=bar_width * 1000, color='r', alpha=0.5, left=-500, edgecolor='w', linewidth=0.5)    

    # for i in range(len(df_layer)):
    #     ax.text(0, y0[i], f'det {i:02d}', ha='center', va='center', fontsize=5)

    # ax.set_xlim(-600, 600)
    # ax.set_title('Layer 0 (front)')
    # ax.set_xlabel('x [mm]')
    # ax.set_ylabel('y [mm]')


    # fig, ax = plt.subplots(1,1,dpi=300,figsize=(4.5,3.5), constrained_layout=True)
    # df_layer = df[df['layer_id'] == 1]
    
    # print(df_layer)
    # y0 = df_layer['x'].values * 1000
    # x0 = -500


    # ax.bar(y0, 1000, width=bar_width * 1000, color='b', alpha=0.5, bottom=x0, edgecolor='w', linewidth=0.5)    

    # for i in range(len(df_layer)):
    #     ax.text(y0[i], 0, f'{(i+24):02d}', ha='center', va='center', fontsize=5)

    # ax.set_ylim(-600, 600)
    # ax.set_title('Layer 1 (middle)')
    # ax.set_xlabel('x [mm]')
    # ax.set_ylabel('y [mm]')

    fig, ax = plt.subplots(1,1,dpi=300,figsize=(4.5,3.5), constrained_layout=True)
    df_layer = df[df['layer_id'] == 2]
    
    print(df_layer)
    y0 = df_layer['x'].values * 1000
    x0 = -500


    ax.barh(y0, 1000, height=bar_width * 1000, color='r', alpha=0.5, left=-500, edgecolor='w', linewidth=0.5)    

    for i in range(len(df_layer)):
        ax.text(0, y0[i], f'{(i+48):02d}', ha='center', va='center', fontsize=5)

    ax.set_xlim(-600, 600)
    ax.set_title('Layer 2 (back)')
    ax.set_xlabel('x [mm]')
    ax.set_ylabel('y [mm]')




if __name__ == '__main__':
    main()
        





