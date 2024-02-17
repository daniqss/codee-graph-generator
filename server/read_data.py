import pandas as pd
import json

def read_data(json_path):
    file = json.load(open(json_path))
    screening = pd.DataFrame(file['Evaluation'][0])
    category = pd.DataFrame(file['Evaluation'][1])
    checkers = pd.DataFrame(file['Ranking of Checkers'][0])

    return screening, category, checkers
 

