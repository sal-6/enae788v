from matplotlib import pyplot as plt
import os
import sys
import json
import numpy as np

DATA_FOLDER_PATH = "G:\\My Drive\\UMD\\Spring 2023\\ENAE788V\\Code\\enae788v\\data\\hw3"
OUTPUT_FOLDER_PATH = "G:\\My Drive\\UMD\\Spring 2023\\ENAE788V\\Code\\enae788v\\output\\hw3"

def parse_obstacles(file_path):
    obstacles = []
    with open(file_path, 'r') as f:
        f.readline() # skip the first line
        for line in f:
            obs_info = line.split(",")
            x = float(obs_info[0])
            y = float(obs_info[1])
            radius = float(obs_info[2])
            obstacles.append([x, y, radius])            
            
    return obstacles


def parse_tree(file_path):
    data = []
    with open(file_path, 'r') as f:
        for line in f:
            line_info = line.split(",")
            p1 = {
                "t" : float(line_info[0]),
                "x" : float(line_info[1]),
                "y" : float(line_info[2]),
                "theta" : float(line_info[3]),
                "v" : float(line_info[4]),
                "w" : float(line_info[5]),
            }
            data.append(p1)
            
    return data
            


def parse_path(file_path):
    data = []
    with open(file_path, 'r') as f:
        for line in f:
            line_info = line.split(",")
            p = (float(line_info[0]), float(line_info[1]))
            data.append([p])
            
    return data


def parse_robot(file_path):
    data = []
    with open(file_path, 'r') as f:
        for line in f:
            line_info = line.split(",")
            p = (float(line_info[0]), float(line_info[1]))
            data.append(p)
            
    return data


def compute_robot_points(robot, x, y, theta):
    robot_points = []
    for p in robot:
        x_new = x + p[0] * np.cos(theta) - p[1] * np.sin(theta)
        y_new = y + p[0] * np.sin(theta) + p[1] * np.cos(theta)
        robot_points.append((x_new, y_new))
        
    return robot_points


def main():
    
    problem = sys.argv[1]
    time_step_tolerance = float(sys.argv[2])
    problem_info = json.load(open(os.path.join(DATA_FOLDER_PATH, "problems.json")))[problem]
    obstacle_path = os.path.join(DATA_FOLDER_PATH, "obstacles.txt")
    robot_path = os.path.join(DATA_FOLDER_PATH, "H3_robot.txt")
        
    obstacles = parse_obstacles(obstacle_path)
    robot = parse_robot(robot_path)
    
    fig = plt.figure()
    ax = plt.axes()
    
    # set limits to -50 to 50 in both x and y
    ax.set_xlim(-50, 50)
    ax.set_ylim(-50, 50)
    
    # plot the obstacles
    for obs in obstacles:
        circle = plt.Circle((obs[0], obs[1]), obs[2], color='black')
        ax.add_patch(circle)
    
    # plot start point
    start_point = (problem_info["start_x"], problem_info["start_y"])
    plt.plot(start_point[0], start_point[1], 'go')
    
    # add label
    plt.text(start_point[0] - 5, start_point[1] + 2, "Start")
    
    # plot goal region
    goal_center = (problem_info["goal_x"], problem_info["goal_y"])
    goal_radius = problem_info["goal_radius"]
    goal_circle = plt.Circle(goal_center, goal_radius, color='r', fill=False)
    ax.add_patch(goal_circle)
    plt.text(goal_center[0] - 5, goal_center[1], "Goal")
    
    # plot the tree
    tree_path = os.path.join(OUTPUT_FOLDER_PATH, f"tree_{problem}.csv")
    tree = parse_tree(tree_path)
    for i in range(len(tree) - 1):
        x_s = [tree[i]["x"], tree[i+1]["x"]]
        y_s = [tree[i]["y"], tree[i+1]["y"]]
        time_diff = tree[i+1]["t"] - tree[i]["t"]
        if abs(time_diff) < time_step_tolerance:
            plt.plot(x_s, y_s, 'b')
            
    # plot the path
    """ path_path = os.path.join(OUTPUT_FOLDER_PATH, f"path_{problem}.txt")
    path = parse_path(path_path)
    for point in path:
        plt.plot(point[0][0], point[0][1], 'rx')
        
    i = 0
    while i < len(path) - 1:
        plt.plot([path[i][0][0], path[i+1][0][0]], [path[i][0][1], path[i+1][0][1]], 'r')
        i += 1 """
    
    
    # scatter robot every 500 time steps
    i = 0
    while i < len(tree):
        robot_points = compute_robot_points(robot, tree[i]["x"], tree[i]["y"], tree[i]["theta"])
        x_s = [p[0] for p in robot_points]
        y_s = [p[1] for p in robot_points]
        plt.plot(x_s, y_s, marker=".", markersize=.5, c='pink')
        # plot velocity vector
        plt.quiver(tree[i]["x"], tree[i]["y"], tree[i]["v"] * np.cos(tree[i]["theta"]), tree[i]["v"] * np.sin(tree[i]["theta"]), color='red', scale=5, headwidth=2, headlength=1.5, linewidths=[.05])
        i += 500
    
    
    # set aspect ratio to 1
    ax.set_aspect('equal')
    #plt.savefig(OUTPUT_FOLDER_PATH + f"\\graph_{problem}.png")
    plt.show()
    

if __name__ == "__main__":
    main()