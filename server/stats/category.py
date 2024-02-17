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
    L1_cache_rate, L2_cache_rate, L3_cache_rate = category_Lrate(df)
    L1_cache_rate.plot(kind='bar', color='r', alpha=0.7, label='L1')
    L2_cache_rate.plot(kind='bar', color='g', alpha=0.7, label='L2')
    L3_cache_rate.plot(kind='bar', color='b', alpha=0.7, label='L3')
    plt.xlabel('')
    plt.ylabel('%')
    plt.title('Gráfico de Barras')
    plt.legend()
    plt.show()

def category_rate_graph(df: pd.DataFrame):
    Scalar_rate, Vector_rate, Memory_rate, Control_rate = category_rate(df)
    Scalar_rate.plot(kind='bar', color='r', alpha=0.7, label='Scalar')
    Vector_rate.plot(kind='bar', color='g', alpha=0.7, label='Vector')
    Memory_rate.plot(kind='bar', color='b', alpha=0.7, label='Memory')
    Control_rate.plot(kind='bar', color='y', alpha=0.7, label='Control')
    plt.yticks([0, 0.2, 0.4, 0.6, 0.8, 1], ['0%', '20%', '40%', '60%', '80%', '100%'])
    plt.title('Gráfico de Barras')
    plt.legend()
    plt.show()