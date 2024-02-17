import pandas as pd

def category_Lrate(df: pd.DataFrame):
    L_sum = df['L1'] + df['L2'] + df['L3']
    L1_cache_rate = df['L1'] / L_sum
    L2_cache_rate = df['L2'] / L_sum
    L3_cache_rate = df['L3'] / L_sum
    return L1_cache_rate, L2_cache_rate, L3_cache_rate

def category_rate(df: pd.DataFrame):
    L_sum = df['Scalar'] + df['Vector'] + df['Memory'] + df['Control']
    Scalar_rate = df['Scalar'] / L_sum
    Vector_rate = df['Vector'] / L_sum
    Memory_rate = df['Memory'] / L_sum
    Control_rate = df['Control'] / L_sum
    return Scalar_rate, Vector_rate, Memory_rate, Control_rate