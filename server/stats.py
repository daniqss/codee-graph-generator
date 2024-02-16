import pandas as pd

def group_by_date(df: pd.DataFrame) :
    df['Fecha'] = pd.to_datetime(df['Fecha'], dayfirst=True)
    df_sorted = df.sort_values(by='Fecha')
    result = df_sorted.groupby(df_sorted['Fecha'].dt.date)['Consumo_KWh'].sum()
    return result

# def group_by_hour(df)

def filter_by_date(df: pd.DataFrame, start_date=None, end_date=None) :
    if start_date and end_date:
        mask = (df['Fecha'] > start_date) & (df['Fecha'] <= end_date)
        return df.loc[mask]
    elif start_date:
        mask = (df['Fecha'] > start_date)
        return df.loc[mask]
    elif end_date:
        mask = (df['Fecha'] <= end_date)
        return df.loc[mask]
    else:
        return df

# def 