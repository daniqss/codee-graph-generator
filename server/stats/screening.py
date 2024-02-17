

# functions to process 
def get_screening_reports(df):
    '''
    Returns the  reports from screening
    '''
    ret_df = df.copy()
    opmitimizable_lines_ratio = df['Optimizable lines'] / df['Lines of code']