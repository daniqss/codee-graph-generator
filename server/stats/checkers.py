import pandas as pd
import json 

def checkersTable(json_data): 
    try: 
        df = pd.DataFrame("../dataset/example.json")
        return df 
    except Exception as ex: 
        print(f"Error al procesar el json {ex}")
        return None 
    
with open("../dataset/example.json") as f:
    json_data = json.load(f)

df = checkersTable(json_data)
print(df)