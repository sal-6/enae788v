# Overview

This repo will (hopefully) contain all HW code written for ENAE788V. This README will outline the process for running and using the relavant code.

# Prerequisites
- python3
- g++
- make
  
## Setup
1. Install `make` based on system
2. Install some C++ compiler
   1. All makefiles assume that it will be referred to as g++
3. Configure Python
   1. Create a virtual env in root dir `py -m venv venv`
   2. Activate environment
      1. `./venv/Scripts/activate` on windows
   3. Install `requirements.txt`
      1. `py -m pip install -r requirements.txt`



# Homeworks   
## Homework 1

The goal of this HW was to implement the A* algorithm for an arbitrary graph. Using this code, files containing nodes and edges of the following format can
be read in. A start node id and goal node id can be passed. The search path and search tree will be outputted.

nodes_<#>.txt

```
{NUM NODES}
{node_id}, {x}, {y}
{node_id}, {x}, {y}
{node_id}, {x}, {y}
```

edges_with_costs_<#>.txt

```
{NUM EDGES}
{start_node_id}, {end_node_id}, {length}
{start_node_id}, {end_node_id}, {length}
{start_node_id}, {end_node_id}, {length}
{start_node_id}, {end_node_id}, {length}
```

Note the # in the file names. Below are the steps to run.

1. Copy the Makefile from `./makefiles/hw1` to the root dir.
2. Run the `make` command to compile the code. A srch.exe file should be created in the root dir.
   1. Run `make clean` to remove unneeded files
3. Place node/edge files named properly in `./data/hw1`
   1. The given files are already there
4. From the root dir, run `./srch {#} {start_node_id} {end_node_id}`
   1. Output files will be created in `./output/hw1`
      1. `output_path_#.txt` will contain the path
      2. `search_tree_#.txt` will contain the tree
   
The paths and search tree can be visualized via a plot.
1. Navigate to ./src/hw1
2. Open the plot_graphs.py script and adjust the absolute paths at the top of the script
3. Create a plot by running `py ./plot_graphs.py {#} {start_node_id} {end_node_id}`

The HW consists of 5 given problems, if on windows, they can all be run by executing the batch file in `./runs`.
1. Change working dir into runs
2. Run `./hw1.bat`

### Implementation Notes

The implementation of A* can be found in `./src/hw1/simple_graph_search.cpp`. It utilizes a slightly modified form of the 'graph.h' provided by Michael Otte, specifically, modifying the original Node struct. This was changed to a class with a method to obtain each nodes ID (as opposed to index). Additionally we track each nodes cost from the start node at a given point in time. We also track each nodes priority which accounts for a Euclidean distance heuristic for that node to reach the end node. Nodes are searched in order of that with the lowest priority value.

The standard C++ library `std::priority_queue` was utilized to implement A*. Its documentation can be found here (https://en.cppreference.com/w/cpp/container/priority_queue). The Node class is given a 
priority attribute. The `std:priority_queue` used is given a Compare parameter that changes the default behaviour and returns the Node with the smaller priority attribute, that is, it behaves as a min priority queue.


The outputted paths, search trees, and plots can be found in './output/hw1'. In each plot, the Blue dots represent the Nodes, the Black lines represent each edge (since the given graphs are not directional no arrows are shown as they would be bi-directional), the Red path outlines the found optimal path, and the Green shows the search tree. The plot naming format is 'graph_<#>_s<start_node_id>_e<end_node_id>'.