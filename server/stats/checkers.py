import pandas as pd
import json
import numpy as np
import matplotlib.pyplot as plt
import read_data

def checker_LevelRate(df: pd.DataFrame):
     cp_df = df.copy()
     cp_df = cp_df.iloc[:-1]
     level_counts = df["Level"].value_counts()
     return level_counts

def checker_LRate(df: pd.DataFrame):
     fig, ax = plt.subplots(1,2)
     cp_df = df.copy()
     cp_df = cp_df.iloc[:-1]
     ax[0].bar(cp_df.index, cp_df["L3"])
     ax[0].set_title('Gráfico de Barras para L3')
     ax[0].set_ylabel('Valor de L3')
     ax[1].bar(cp_df.index, cp_df["L1"])
     ax[1].set_title('Gráfico de Barras para L1')
     ax[1].set_ylabel('Valor de L1')
     return fig