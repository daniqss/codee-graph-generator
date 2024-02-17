import pandas as pd
import json

def read_data(json_path):
    try:
        file = json.load(open("dataset/example.json"))
        screening = pd.DataFrame(file['Evaluation'][0])
        category = pd.DataFrame(file['Evaluation'][1])
        checkers = pd.DataFrame(file['Ranking of Checkers'][0])

        return screening, category, checkers

    except FileNotFoundError: 
        print(f"El archivo en la ruta '{json_path}' no se encontró. ")
        return None 
    except Exception as ex:
        print(f"Ocurrio un error al leer el archivo CSV: {ex}")
        return None        

