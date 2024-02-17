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
     x = range(len(cp_df))
     width = 0.3
     ax.bar(x - width/3, cp_df["L1"], width, label ="L1")
     ax.bar(x , cp_df["L2"], width, label ="L2")
     ax.bar(x + width/3, cp_df["L3"], width, label = "L3")

     ax.legend()
     return fig