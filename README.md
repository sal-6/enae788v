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

### Instructions

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

## Homework 2

The goal of this homework was to implement a basic version of the RRT algorithm. As inputs, the implemented code will require an obstacles file structured as seen below:

obstacles.txt

```
{NUM OBSTACLES}
{x}, {y}, {radius}
{x}, {y}, {radius}
{x}, {y}, {radius}
```

### Instructions

1. Copy the Makefile from `./makefiles/hw2` to the root dir.
2. Run the `make` command to compile the code. A `rrt.exe` file should be created in the root dir.
   1. Run `make clean` to remove unneeded files
3. Place obstacles file named properly in `./data/hw2`
   1. The given file is already there
4. Place a JSON file outlining the start and end positions by problem number in `./data/hw2`
   1. A file has been placed there for the given problems as an example
5. From the root dir, run `./rrt.exe {#}`
   1. Output files will be created in `./output/hw2`
      1. `path_#.txt` will contain the path
      2. `tree_#.txt` will contain the tree
   
The paths and search tree can be visualized via a plot.
1. Navigate to `./src/hw2`
2. Open the plot_graphs.py script and adjust the absolute paths at the top of the script
3. Create a plot by running `py ./plot_graphs.py {#}`

The HW consists of 5 given problems, if on windows, they can all be run by executing the batch file in `./runs`.
1. Change working dir into runs
2. Run `./hw2.bat`

### Implementation Notes

The implementation of RRT can be found in `./src/hw2/hw2.cpp`. The only external dependency used is a utility used to parse JSON data. This is implemented in `./include/hw2/json.hpp` and was obtained from the following source: https://github.com/nlohmann/json. The declarations of all he utilities used can be found in `./include/hw2/utils.h`. This header file gives a pretty good idea of how the RRT algorithm was implemented. The implementation of all the declarations can be found in `./src/hw2/utils.cpp`. The implementation of RRT can be found in the `main` function of `./src/hw2/hw2.cpp`. Some considerations were made such as, rather than looping until the goal region condition was met, I preferred to loop until some max interation count and simply break from the loop if the goal region condition is met. Additionally, I am not doing continuous line segment collision checks, rather, I am dividing the line segment into some number of points and checking each point against all the obstacles. Hypothetically, this could break down if a line is almost but not quite tangent to an obstacle, while it is not seen in any of the outputted examples (even if in the PNG it looks close, all the cases I zoomed in on did not have any collisions). That being said, I do 100 divisions currently for the small comparitively small epsilon values so this is very unlikely to occur. If more fidelity is desired the number of divisions has been left as an argument and can be adjusted as needed. Or if one really would like, they can implement a continous check for a line and circle. Furthermore, the nearest neighbor check is currently a loop through all other nodes and the minimum of the Euclidean distance is found. While this isnt effecient, for the scale of the problems given it still runs in seconds. That being said, a quadtree or kd tree can be used to make the search for the closest node more effecient. The final note is that points ON the edge of an obstacle are considered invalid in my implementation. This can be changed easily if needed in the collision check function.

The outputted paths, search trees, and plots can be found in `./output/hw2`. In each plot the Black represents the obstacles, the Blue represent the tree, the Red path outlines the found path, the Green shows the start point, and the Red circle shows the end region. The plot naming format is 'graph_<#>.png' where <#> is the problem number. There are also some cases called 'detail_<#>.png' which show some zoomed in cases of where the tree might almost contact an obstalce. Here <#> has no significance. This is shown just to further show that there are no collisions as the zoomed out plots render such that some edges are very close.

## Homework 3

The goal of this homework was to implement a version of the RRT algorithm which accounts for the volume and dynamics of a half car like robot. As inputs, the implemented code will require an obstacles file and robot definition file, both structured as seen below:

obstacles.txt

```
{NUM OBSTACLES}
{x}, {y}, {radius}
{x}, {y}, {radius}
{x}, {y}, {radius}
```

H3_robot.txt
```
{x}, {y}
{x}, {y}
{x}, {y}
```


### Instructions

1. Copy the Makefile from `./makefiles/hw3` to the root dir.
2. Run the `make` command to compile the code. A `rrt.exe` file should be created in the root dir.
   1. Run `make clean` to remove unneeded files
3. Place obstacles file named properly in `./data/hw3`
   1. The given file is already there
4. Place a JSON file outlining the start and end positions by problem number in `./data/hw3`
   1. A file has been placed there for the given problems as an example
5. From the root dir, run `./rrt.exe {#}`
   1. Output files will be created in `./output/hw3`
      1. `path_#.txt` will contain the path
      2. `tree_#.txt` will contain the tree
   
The paths and search tree can be visualized via a plot.
1. Navigate to `./src/hw3`
2. Open the plot_graphs.py script and adjust the absolute paths at the top of the script
3. Create a plot by running `py ./plot_graphs.py {#} {tol}` where `tol` is the time tolerance for the tree. `tol` will allow for jumps in the tree output that can be accounted for by the time parameter of each trajectory. Should be slightly larger than the timestep used for the Euler propogation. 

The HW consists of 5 given problems, if on windows, they can all be run by executing the batch file in `./runs`.
1. Change working dir into runs
2. Run `./hw3.bat`

### Implementation Notes

The implementation of RRT can be found in `./src/hw2/hw3.cpp`. The only external dependency used is a utility used to parse JSON data. This is implemented in `./include/hw3/json.hpp` and was obtained from the following source: https://github.com/nlohmann/json. The declarations of all he utilities used can be found in `./include/hw3/utils.h`. This header file gives a pretty good idea of how the RRT algorithm was implemented. The implementation of all the declarations can be found in `./src/hw3/utils.cpp`. The implementation of RRT can be found in the `rrt_with_dynamics_and_volume` function of `./src/hw3/hw3.cpp`. Some considerations were made such as, rather than looping until the goal region condition was met, I preferred to loop until some max interation count and simply break from the loop if the goal region condition is met.


The algorithm works as follows. We start the tree with the initial start point. A random point (x, y) in the workspace is generated. Given that the only control parameters are acceleration and steering acceleration for any given edge, and that those parameters are bounded, every combination of the two parameters is generated from `-MAX_ACCELERATION` to `MAX_ACCELERATION` and from `-MAX_STEEERING_ACCELERATION` to `MAX_STEEERING_ACCELERATION` with a step of `CONTROL_ACCERATION_FIDELITY` and `CONTROL_STEEERING_FIDELITY`, respectively. A `RobotTrajectory` is created for each of the possible combination of acceleration and steering acceleration. The initial parameters of the trajectory (i.e. t, x, y, v, w, ...) are set as that of the the `RobotTrajectory` in the current `SearchTree` with the closest end point Euclidean distance to the randomly generated point. Each of these `RobotTrajectory` objects is propogated using an Euler approximation with a given time step until the length of the curve is about equal to epsilon. The details of this are in `RobotTrajectory::propogate_until_distance`. The "best trajectory" is determined as the `RobotTrajectory` whose end point is closest to the randomly genereated point. This `RobotTrajectory` is added to the tree. As such, this can be categorized as 'solving' with an initial value boundary condition. This process is then repeated until the max allowable iterations are reached or a trajectory end point falls in the goal region. Additionally, the volume of the robot is accounted for by creating a `Robot` which contains the point cloud representing the robot. At every coordinate (x, y, theta) in a `RobotTrajectory`, collisions are checked for transformed points. That is, given that 0, 0 is the rotational axis of the robot, at every point the `Robot` coordinates are rotated about the center and collision checks are performed. Furthermore, various other constraints are set on the validity of a propogated `RobotTrajectory` such as all points in the trajectory must lay in the -50 to 50 space in the x and y, max velocities must not be exceeded, etc.

That being said, while this code does meet the requirements, having written it, I can attest to how ineffecient it is and list a multitude of potential improvments ranging from how it was written to actual implementation decisions. I perform many nearest neigbor searches which can be sped up with the use of a quadtree or kd tree. Currently for every collision check with the `Robot`, all the points in the cloud are looped through. This can be made better with some quick precursor checks that may reduce be quick to test that result in a collision. This can include something like a bounding circle around the points, which, if is not in collision with an obstacle, could allow forgoing having to loop through all the points for some cases. Additionally, there are various code architecture improvements that can be made but are likely irrelvant.

The outputted paths, search trees, and plots can be found in `./output/hw3`. The plot naming format is 'graph_<#>.png' where <#> is the problem number. There are also some cases called 'detail_<#>.png' which show some zoomed in cases of where the tree might almost contact an obstalce. Here <#> has no significance. This is shown just to further show that there are no collisions as the zoomed out plots render such that some edges are very close.



## Randoms TODOs I Might Do at Some Point, IDK
- [ ] Organize header files for HW1 in subfolder
- [ ] Implement an actual data structure for hw2 closest neighbor
- [ ] Do a continous check for collision between a circle and an arbitrary curve for HW2
- [ ] Literally so much on HW3