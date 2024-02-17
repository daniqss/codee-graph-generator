# Description
codee_graph_generator is a Python program that generates a graph from the output of the Codee tool. The graphs are generated using the Matplotlib library and are saved as HTML files using the Plotly library.
[Codee](https://www.codee.com/) is a suite of command-line tools aimed at helping software developers to build better quality parallel software in less time.

# Run Docker containers
To run the codee_graph_generator, you need to have Docker installed on your machine. The following command will build the Docker image and run the codee_graph_generator on the given files.

### In Linux
```bash
    cd buildbot
    chmod +x build_containers.sh
    bash ./build_containers.sh
```

### In Windows
```bash
    cd buildbot
    buildbot\build_containers.bat
```

# Script usage
The run.py program is used to execute Codee on a list of given files.
The CODEE_PATH env variable has to be the executable path
Here is a description of its usage:

```bash
CODEE_PATH=/path/to/codee python run.py <file1> <file2> ... <fileN>
<file1> <file2> ... <fileN>: List of files to be analyzed with Codee.
The program performs the following actions:
```
