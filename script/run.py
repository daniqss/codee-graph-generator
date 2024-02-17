# SPDX-FileCopyrightText: 2024 Gael Garcia Arias, Ivan Afonso Cerdeira, Daniel Queijo Seoane, Luca D'angel Sabin
#
# SPDX-License-Identifier: MIT

import sys
import subprocess
import os
import read_data as rd
from datetime import datetime as dt
import stats.category as cat
import stats.checkers as chk
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
    dir = sys.argv[1]
    return dir, codee_path


def run_codee(dir, codee_path):
    # Make string from list of files
    files_str = []
    # Last time fix, sorry!
    for root, dirs, files in os.walk(dir):
        for file in files:
            if file.endswith(".c"):
                files_str.append(os.path.join(root, file))
    # Run codee
    result = subprocess.run(f'{codee_path} {files_str} --json', shell=True, stdout=subprocess.PIPE)

    # Decode the output to a string
    output = result.stdout.decode('utf-8')
    return output


if __name__ == '__main__':
    cat_figs = []
    scr_figs = []
    chk_figs = []

    dir, codee_path = obtain_arguments()

    output = run_codee(dir, codee_path)
    dt_screening, dt_category, dt_checkers = rd.read_data_from_file(output)

    cat_figs = [cat.category_Lrate_graph(dt_category), cat.category_rate_graph(dt_category)]
    scr_figs = [scr.pizza_plot(dt_screening,"Optimizable lines"), scr.bar_plot(dt_screening,"Optimizable lines"),
                scr.effort_cost_ratio_plot(dt_screening), scr.optimizable_lines_ratio_plot(dt_screening)]
    chk_figs = [chk.bar_plot(dt_checkers, "Priority"), chk.bar_plot(dt_checkers, "#")]
    # Add stats to dataframes

    operation_name = dt.now().strftime("%Y-%m-%d_%H-%M-%S")


    # Make HTML from figs
    message = to_html(operation_name, cat_figs, scr_figs, chk_figs)

    # Send to email
    # send_to_email(operation, message)
