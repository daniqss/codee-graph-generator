# Description
codee_graph_generator is a Python program that generates a graph from the output of the Codee tool. The graphs are generated using the Matplotlib library and are saved as HTML files using the mpld3 library.
[Codee](https://www.codee.com/) is a suite of command-line tools aimed at helping software developers to build better quality parallel software in less time.

# Script usage
The run.py program is used to execute Codee on a list of given files.
The CODEE_PATH env variable has to be the executable path
Here is a description of its usage:

```bash
CODEE_PATH=/path/to/codee python run.py <file1> <file2> ... <fileN> / <dir>
```

# Run Docker containers
To run the codee_graph_generator, you need to have Docker installed on your machine. The following command will build the Docker image and run the codee_graph_generator on the given files.

### In Linux
```bash
    cd buildbot
    chmod +x build_containers.sh
    bash ./build_containers.sh
    GIT_URL=https://github.com/githubuser/repo.git
```

### In Windows
```cmd
    cd buildbot
    set GIT_URL=https://github.com/githubuser/repo.git
    buildbot\build_containers.bat
```

# Contributing
If you want to contribute to the project, please read the [CONTRIBUTING.md](CONTRIBUTING.md) file.

# License
This project is licensed under the terms of the [MIT License](LICENSE.md).
