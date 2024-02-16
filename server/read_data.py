import pandas as pd

# def function read_data(csv_path)
# devuelve un pandas DataFrame


def read_data(csv_path):

try: 
    file = pd.read_csv(csv_path)
    return df 
except FileNotFoundError: 
    print(f"El archivo en la ruta '{csv_path}' no se encontró. ")
    return None 
except Exception as ex:
    print(f"Ocurrio un error al leer el archivo CSV: {ex}")
    return None        


# 