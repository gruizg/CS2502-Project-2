# CS 3502 Project 2
This project simulates two CPU scheduling algorithms: Shortest Remaining Time First & Multi-Level Feedback Queue.

## Process
The process of running this project is very simple.
1. Create, or use the provided `input.csv` file containing:
   - The number of processes to be scheduled
   - Pairs of **Arrival Time** and **Burst Time** for each process
2. Run `main.cpp` with the `input.csv` in the same directory
3. Run `result.py` in the same directory to generate results

## Imports
In python, import these libraries to generate the graphic results.
```python
import pandas as pd
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import glob
```
## Build
To run this program:
1. Download and place all files in the same folder.
2. Import the necesary python files.
3. Create your own test processes(optional)
4. Follow the process described above.

# Demo Video
https://www.youtube.com/watch?v=lu-w5LPJLbs
