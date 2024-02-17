import matplotlib.pyplot as plt

# functions to process 
def get_screening_reports(df):
    '''
    Returns the  reports from screening
    '''
    ret_df = df.copy()[:-1]
    ret_df['optimizable_lines_ratio'] = ret_df['Optimizable lines'] / ret_df['Lines of code']

    return ret_df

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