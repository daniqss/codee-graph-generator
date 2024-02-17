# Description
codee_graph_generator is a Python program that generates a graph from the output of the Codee tool. The graphs are generated using the Matplotlib library and are saved as HTML files using the Plotly library.
[Codee](https://www.codee.com/) is a suite of command-line tools aimed at helping software developers to build better quality parallel software in less time.

# Usage
The run.py program is used to execute Codee on a list of given files.
The CODEE_PATH env variable has to be the executable path
Here is a description of its usage:

```bash
CODEE_PATH=/path/to/codee python run.py <file1> <file2> ... <fileN>
<file1> <file2> ... <fileN>: List of files to be analyzed with Codee.
The program performs the following actions:
```
