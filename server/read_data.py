import pandas as pd

# def function read_data(csv_path)
# devuelve un pandas DataFrame


def read_data(json_path):
    try:
        file = pd.read_json(json_path)
        return file
    except FileNotFoundError: 
        print(f"El archivo en la ruta '{json_path}' no se encontró. ")
        return None 
    except Exception as ex:
        print(f"Ocurrio un error al leer el archivo CSV: {ex}")
        return None        

