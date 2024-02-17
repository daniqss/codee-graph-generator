import sys
import subprocess
import os
import read_data as rd
from datetime import datetime as dt
import stats.category as cat
# import stats.checkers as chk
import stats.screening as scr
from to_html import to_html
# from send_to_email import send_to_email

def usage():
    print("Usage: python run.py <file1> <file2> ... <fileN>")
    sys.exit(1)
    

def obtain_arguments():
    # Get codee path from environment variable
    codee_path = os.environ.get('CODEE_PATH')
    if not codee_path:
        print("La variable de entorno CODEE_PATH no está definida.")
        sys.exit(1)

    # Must provide at least one file
    if len(sys.argv) < 2:
        print("Non files provided.")
        sys.exit(1)

    # Obtén la lista de archivos a ejecutar
    files = sys.argv[1:]
    return files, codee_path


def run_codee(files, path):
    # Make string from list of files
    files_str = ' '.join(files)

    # Run codee
    result = subprocess.run(f'{path} {files_str} --json', shell=True, stdout=subprocess.PIPE)

    # Decode the output to a string
    output = result.stdout.decode('utf-8')
    return output


if __name__ == '__main__':
    files, codee_path = obtain_arguments()

    output = run_codee(files, codee_path)
    dt_screening, dt_category, dt_checkers = rd.read_data(output)

    figs = [cat.category_Lrate_graph(dt_category), cat.category_rate_graph(dt_category) , scr.pizza_plot(dt_screening,"Optimizable lines"), scr.bar_plot(dt_screening,"Optimizable lines")]
    # Add stats to dataframes

    operation = "results/" + dt.now().strftime("%Y-%m-%d_%H-%M-%S") + ".html"


    # Make HTML from figs
    message = to_html(operation, figs)

    # Send to email
    # send_to_email(operation, message)
