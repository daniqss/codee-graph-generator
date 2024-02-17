from matplotlib import pyplot as plt
import pandas as pd
import json

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
    L_sum = cp_df['Scalar'] + cp_df['Vector'] + cp_df['Memory'] + cp_df['Control']
    Scalar_rate = cp_df['Scalar'] / L_sum
    Vector_rate = cp_df['Vector'] / L_sum
    Memory_rate = cp_df['Memory'] / L_sum
    Control_rate = cp_df['Control'] / L_sum
    return Scalar_rate, Vector_rate, Memory_rate, Control_rate

def category_Lrate_graph(df: pd.DataFrame):
    fig, ax = plt.subplots()
    L1_cache_rate, L2_cache_rate, L3_cache_rate = category_Lrate(df)

    ax.bar(0, L1_cache_rate, color='r', width=0.2, alpha=0.7, label='L1')
    ax.bar(0.5, L2_cache_rate, color='g', width=0.2, alpha=0.7, label='L2')
    ax.bar(1.0, L3_cache_rate, color='b', width=0.2, alpha=0.7, label='L3')

    ax.set_xticks([0, 0.5, 1.0])
    ax.set_xticklabels(['L1', 'L2', 'L3'])

    plt.yticks([0, 0.2, 0.4, 0.6, 0.8, 1], ['0%', '20%', '40%', '60%', '80%', '100%'])
    plt.title('Lrate graph')
    plt.legend()
    return fig

def category_rate_graph(df: pd.DataFrame):
    fig, ax = plt.subplots()
    Scalar_rate, Vector_rate, Memory_rate, Control_rate = category_rate(df)
   
    ax.bar(0, Scalar_rate, color='r', width=0.2, alpha=0.7, label='Scalar')
    ax.bar(0.33, Vector_rate, color='g', width=0.2, alpha=0.7, label='Vextor')
    ax.bar(0.66, Memory_rate, color='b', width=0.2, alpha=0.7, label='Memory')
    ax.bar(1, Control_rate, color='b', width=0.2, alpha=0.7, label='Control')
    

    ax.set_xticks([0, 0.33, 0.66, 1.0])
    ax.set_xticklabels(['Scalar', 'Vextor', 'Memory', 'Control'])

    
    plt.yticks([0, 0.2, 0.4, 0.6, 0.8, 1], ['0%', '20%', '40%', '60%', '80%', '100%'])
    plt.title('Rate graph')
    plt.legend()
    return fig