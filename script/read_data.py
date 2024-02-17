# SPDX-FileCopyrightText: 2024 Gael Garcia Arias, Ivan Afonso Cerdeira, Daniel Queijo Seoane, Luca D'angel Sabin
#
# SPDX-License-Identifier: MIT

import pandas as pd
import sys
import json
from constants import *

def convert_types_screening(df):
    df['Cost'] = df['Cost'].apply(lambda x: x[:-1])
    df['Effort'] = df['Effort'].apply(lambda x: x.split(' ')[0])
    df['Analysis time'] = df['Analysis time'].apply(lambda x: x.split(' ')[0])
    for k, v in TYPE_MAPPINGS_SCREENING.items():
        df[k] = df[k].astype(v, errors='ignore')
    # Rename
    df.rename({'Effort': 'Effort (h)'}, inplace=True)
    df.rename({'Analysis time': 'Analysis time (h)'}, inplace=True)
    
    return df

def convert_types_category(df):
    for k, v in TYPE_MAPPINGS_CATEGORY.items():
        df[k] = df[k].astype(v, errors='ignore')

    #search for string n/a and replace with 0 in columns Multi and Offload
    df['Multi'] = df['Multi'].replace('n/a', 0)
    df['Offload'] = df['Offload'].replace('n/a', 0)

    return df

def convert_types_checkers(df):
    for k, v in TYPE_MAPPINGS_CHECKERS.items():
        df[k] = df[k].astype(v, errors='ignore')
    
    return df

def __read_json(json_obj):
    screening = pd.DataFrame(json_obj['Evaluation'][0])
    category = pd.DataFrame(json_obj['Evaluation'][1])
    checkers = pd.DataFrame(json_obj['Ranking of Checkers'][0])
    return convert_types_screening(screening), convert_types_category(category), convert_types_checkers(checkers)

def read_data(string):
    print(string)
    file = json.loads(string)
    return __read_json(file)

def read_data_from_file(json_path):
    try:
        file = json.load(open(json_path))
    except Exception as e:
        print(f'Error: {e}')
        sys.exit(1)
    return __read_json(file) 

