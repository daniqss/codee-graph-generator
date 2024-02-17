import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

# Only one type of char, but we should be able to use it for priority and #

def bar_plot(df, col, title='Bar plot', include_total=True, max_range=0):
    df = df.sort_values(by=[col, 'Checker'], ascending=[True, True])

    fig, ax = plt.subplots(figsize=(12, 6))

    ax.bar(df['Checker'], df[col], color=plt.cm.Paired.colors)
    ax.set_title(f"{title} shorted by {col}", pad=25)
    ax.set_xticklabels(df['Checker'], rotation=75, fontsize=8)
    return fig
