# Overview

This repo will (hopefully) contain all HW code written for ENAE788V. This README will outline the process for running and using the relavant code.

# Prerequisites
- Python3
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

node_<#>.txt
`
{NUM NODES}
{node_id}, {x}, {y}
{node_id}, {x}, {y}
{node_id}, {x}, {y}
`

edges_with_costs_<#>.txt
`
{NUM EDGES}
{start_node_id}, {end_node_id}, {length}
{start_node_id}, {end_node_id}, {length}
{start_node_id}, {end_node_id}, {length}
{start_node_id}, {end_node_id}, {length}
`

Note the # in the file names. Below are the steps to run.

1. Copy the Makefile from ./makefiles/hw1 to the root dir.
2. Run the `make` command to compile the code. A srch.exe file should be created in the root dir.
   1. Run `make clean` to remove unneeded files
3. Place node/edge files named properly in ./data/hw1
   1. The given files are already there
4. From the root dir, run `./srch {#} {start_node_id} {end_node_id}`
   1. Output files will be created in ./output/hw1
      1. output_path_#.txt will contain the path
      2. search_tree_#.txt will contain the tree
   
The paths and search tree can be visualized via a plot.
1. Navigate to ./src/hw1
2. Open the plot_graphs.py script and adjust the absolute paths at the top of the script
3. Create a plot by running `py ./plot_graphs.py {#} {start_node_id} {end_node_id}`

### Some Notes

The standard C++ library std::priority_queue was utilized to implement A*. The Node class is given a 
priority attribute. The std:priority_queue used is given a Comparator that sort returns the Node with
the smaller priority value.