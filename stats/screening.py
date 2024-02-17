import matplotlib.pyplot as plt
import pandas as pd


def get_effort_cost_ratio(df):
    '''
    Returns the ratio effort/cost for each target
    '''
    ret_df = df.copy()[:-1]
    ret_df['effort_cost_ratio'] = ret_df['Effort'] / ret_df['Cost']
    return ret_df



def get_screening_reports(df):
    '''
    Returns the  reports from screening
    '''
    ret_df = df.copy()[:-1]
    ret_df['optimizable_lines_ratio'] = ret_df['Optimizable lines'] / ret_df['Lines of code']

    return ret_df

# functions to plot data from processed data over this part of the code
def effort_cost_ratio_plot(df, title='Effort/Cost ratio'):
    fig, ax = plt.subplots(figsize=(12, 6))
    df = get_effort_cost_ratio(df)
    df = df.sort_values(by='effort_cost_ratio', ascending=True)
    ax.bar(df['Target'], df['effort_cost_ratio'], color=plt.cm.Paired.colors)
    # set limit for x axis from 0 to 1
    ax.set_ylim(0, 0.2)
    ax.set_title(f"{title} by target", pad=25)
    ax.set_xticklabels(df['Target'], rotation=75, fontsize=8)
    return fig

# functions to plot data from get_screening_reports 
def optimizable_lines_ratio_plot(df, title='Optimizable lines ratio'):
    fig, ax = plt.subplots(figsize=(12, 6))
    df = get_screening_reports(df)
    df = df.sort_values(by='optimizable_lines_ratio', ascending=True)
    ax.bar(df['Target'], df['optimizable_lines_ratio'], color=plt.cm.Paired.colors)
    ax.set_title(f"{title} by target", pad=25)
    ax.set_xticklabels(df['Target'], rotation=75, fontsize=8)
    return fig

def pizza_plot(df, col, title='Pizza plot', include_total=True, absolute_units=False):
    fig, ax = plt.subplots(figsize=(10, 10))
    total = df[col].sum()
    suppl_title = (
        f" ({df[col].sum()} {absolute_units} total)"
        if include_total and absolute_units
        else ""
    )


    if absolute_units:
        autopct = lambda p: '{:.0f}'.format(p * total / 100) + f" {absolute_units}"
    else:
        autopct = '%1.1f%%'
    ax.pie(
        df[col], 
        labels=df['Target'], 
        autopct=autopct, 
        colors=plt.cm.Paired.colors,
        )
    # ax.axis('equal')
    ax.set_title(f"{title}{suppl_title} by target", pad=25)
    # ax.set_xticklabels(df['Target'], rotation=45, fontsize=8)
    return fig


def bar_plot(df, col, title='Bar plot', include_total=True, max_range=0):
    fig, ax = plt.subplots(figsize=(12, 6))
    total = df[col].sum()

    df = df.sort_values(by=col, ascending=False)
    ax.bar(df['Target'], df[col], color=plt.cm.Paired.colors)
    # add other bar with the total
    ax.set_title(f"{title} by target", pad=25)
    ax.set_ylim(0, max(max_range, df[col].max() * 1.1))
    ax.set_xticklabels(df['Target'], rotation=75, fontsize=8)
    # increase plot size to allow room for long labels
    # fig.set_size_inches(10, 10)
    return fig
