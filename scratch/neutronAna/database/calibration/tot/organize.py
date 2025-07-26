import pandas as pd
import numpy as np

df_all = [
    pd.read_csv(
        f'parameters{idx}_all.txt',
        names=['det', 'b1','b2','k'],
        skiprows=1,
    ) 
    for idx in range(1,4)
]

df_june = [
    pd.read_csv(f'parameters{idx}_june.txt', names=['det', 'b1','b2','k'], skiprows=1) for idx in range(1,4)
]


def get_value(s):
    try:
        return float(str(s.strip()).split('+/-')[0])
    except:
        return 0.0

df_all_organized = pd.concat(df_all, ignore_index=True)
df_june_organized = pd.concat(df_june, ignore_index=True)

baselines = [0.5, 0.29, 0.25]

for col in df_all_organized.columns[1:]:
    df_all_organized[col] = df_all_organized[col].apply(get_value)

for col in df_june_organized.columns[1:]:
    df_june_organized[col] = df_june_organized[col].apply(get_value)

df_all_organized['baseline'] = [0 for _ in range(len(df_all_organized))]
df_june_organized['baseline'] = [0 for _ in range(len(df_june_organized))]

for idx, subdf in df_all_organized.iterrows():
    det = subdf['det']
    params = subdf[['b1', 'b2', 'k']].values
    if np.allclose(params, [0, 0, 0]):
        df_all_organized.at[idx, 'baseline'] = 1
        df_all_organized.loc[idx, ['b1', 'b2', 'k']] = baselines

for idx, subdf in df_june_organized.iterrows():
    det = subdf['det']
    params = subdf[['b1', 'b2', 'k']].values
    if np.allclose(params, [0, 0, 0]):
        df_june_organized.at[idx, 'baseline'] = 1
        df_june_organized.loc[idx, ['b1', 'b2', 'k']] = baselines


df_all_organized.to_csv('gain-matching-all.txt', index=False, sep='\t')
df_june_organized.to_csv('gain-matching-june.txt', index=False, sep='\t')
