# SPDX-FileCopyrightText: 2024 Gael Garcia Arias, Ivan Afonso Cerdeira, Daniel Queijo Seoane, Luca D'angel Sabin
#
# SPDX-License-Identifier: MIT

from matplotlib import pyplot as plt
import pandas as pd

def category_Lrate(df: pd.DataFrame):
    cp_df = df.copy()
    cp_df = cp_df.iloc[:-1]
    L_sum = cp_df['L1'] + cp_df['L2'] + cp_df['L3']
    L1_cache_rate = cp_df['L1'] / L_sum
    L2_cache_rate = cp_df['L2'] / L_sum
    L3_cache_rate = cp_df['L3'] / L_sum
    return L1_cache_rate, L2_cache_rate, L3_cache_rate

def category_rate(df: pd.DataFrame):
    cp_df = df.copy()
    cp_df = cp_df.iloc[:-1]
    L_sum = cp_df['Scalar'] + cp_df['Vector'] + cp_df['Memory'] + cp_df['Control'] + cp_df['Multi'] + cp_df['Offload'] + cp_df['Quality']
    Scalar_rate = cp_df['Scalar'] / L_sum
    Vector_rate = cp_df['Vector'] / L_sum
    Memory_rate = cp_df['Memory'] / L_sum
    Control_rate = cp_df['Control'] / L_sum
    Multi_rate = cp_df['Multi'] / L_sum
    Offload_rate = cp_df['Offload'] / L_sum
    Quality_rate = cp_df['Quality'] / L_sum
    return Scalar_rate, Vector_rate, Memory_rate, Control_rate, Multi_rate, Offload_rate, Quality_rate

def category_Lrate_graph(df: pd.DataFrame):
    fig, ax = plt.subplots()
    L1_cache_rate, L2_cache_rate, L3_cache_rate = category_Lrate(df)

    bar_width = 0.2

    ax.bar(0, L1_cache_rate, color='r', width=bar_width, alpha=0.7, label='L1')
    ax.bar(0.5, L2_cache_rate, color='g', width=bar_width, alpha=0.7, label='L2')
    ax.bar(1.0, L3_cache_rate, color='b', width=bar_width, alpha=0.7, label='L3')

    ax.set_title(f"Bar plot by target", pad=25)

    ax.set_xticklabels(['L1', 'L2', 'L3'])
    ax.set_xticks([0, 0.5, 1.0])
    ax.set_yticklabels(['0%', '20%', '40%', '60%', '80%', '100%'])
    ax.set_yticks([0, 0.2, 0.4, 0.6, 0.8, 1])
    plt.title('Priority rate graph')
    plt.legend()
    return fig

def category_rate_graph(df: pd.DataFrame):
    fig, ax = plt.subplots()
    Scalar_rate, Vector_rate, Memory_rate, Control_rate, Multi_rate, Offload_rate, Quality_rate = category_rate(df)
   
    bar_width = 0.15
    ax.bar(0, Scalar_rate, color='r', width=bar_width, alpha=0.7, label='Scalar')
    ax.bar(1/6, Vector_rate, color='g', width=bar_width, alpha=0.7, label='Vector')
    ax.bar(2/6, Memory_rate, color='b', width=bar_width, alpha=0.7, label='Memory')
    ax.bar(3/6, Control_rate, color='purple', width=bar_width, alpha=0.7, label='Control')
    ax.bar(4/6, Multi_rate, color='yellow', width=bar_width, alpha=0.7, label='Multi')
    ax.bar(5/6, Offload_rate, color='pink', width=bar_width, alpha=0.7, label='Offload')
    ax.bar(6/6, Quality_rate, color='brown', width=bar_width, alpha=0.7, label='Quality')
    

    ax.set_xticklabels(['Scalar', 'Vextor', 'Memory', 'Control', 'Multi', 'Offload', 'Quality'])
    ax.set_xticks([0, 1/6, 2/6, 3/6, 4/6, 5/6, 6/6])

    ax.set_yticklabels(['0%', '20%', '40%', '60%', '80%', '100%'])
    ax.set_yticks([0, 0.2, 0.4, 0.6, 0.8, 1])

    
    plt.yticks([0, 0.2, 0.4, 0.6, 0.8, 1], ['0%', '20%', '40%', '60%', '80%', '100%'])
    plt.title('Checks rate graph')
    plt.legend()
    return fig


