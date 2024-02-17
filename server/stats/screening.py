import matplotlib.pyplot as plt

# functions to process 
def get_screening_reports(df):
    '''
    Returns the  reports from screening
    '''
    ret_df = df.copy()[:-1]
    ret_df['optimizable_lines_ratio'] = ret_df['Optimizable lines'] / ret_df['Lines of code']

    return ret_df

def pizza_plot(df, col, colors=['#66b3ff', '#ff9999'], title='Pizza plot', include_total=True, absolute_units=False):
    fig, ax = plt.subplots()
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
        colors=colors,
        )
    ax.axis('equal')
    # wrute total
    ax.set_title(f"{title}{suppl_title} by target", pad=25)
    # increase size of figure
    # fig.set_size_inches(10, 10)
    # give a little more spacing for the title
    return fig