import sys
import subprocess
import os
import read_data as rd
import stats.category as cat
import stats.checkers as chk
import stats.screening as scr
import to_html as to_html

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
    print(files_str)

    # Run codee
    result = subprocess.run(f'{path} {files_str} --json', shell=True, stdout=subprocess.PIPE)

    # Decode the output to a string
    output = result.stdout.decode('utf-8')
    return output

    


if __name__ == '__main__':
    files, codee_path = obtain_arguments()

    output = run_codee(files, codee_path)
    dt_screening, dt_category, dt_checkers = rd.read_data(output)

    figs = []
    # Add stats to dataframes



    # Make HTML from figs
    to_html(figs)