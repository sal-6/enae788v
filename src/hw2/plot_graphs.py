from matplotlib import pyplot as plt
import os
import sys
import json

DATA_FOLDER_PATH = "G:\\My Drive\\UMD\\Spring 2023\\ENAE788V\\Code\\enae788v\\data\\hw2"
OUTPUT_FOLDER_PATH = "G:\\My Drive\\UMD\\Spring 2023\\ENAE788V\\Code\\enae788v\\output\\hw2"

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
            p1 = (float(line_info[0]), float(line_info[1]))
            p2 = (float(line_info[2]), float(line_info[3]))
            data.append([p1, p2])
            
    return data
            


def parse_path(file_path):
    pass


def main():
    
    problem = sys.argv[1]
    problem_info = json.load(open(os.path.join(DATA_FOLDER_PATH, "problems.json")))[problem]
    obstacle_path = os.path.join(DATA_FOLDER_PATH, "obstacles.txt")
        
    obstacles = parse_obstacles(obstacle_path)
    
    
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
    plt.text(start_point[0], start_point[1], "Start")
    
    # plot goal region
    goal_center = (problem_info["goal_x"], problem_info["goal_y"])
    goal_radius = problem_info["goal_radius"]
    goal_circle = plt.Circle(goal_center, goal_radius, color='r', fill=False)
    ax.add_patch(goal_circle)
    plt.text(goal_center[0], goal_center[1], "Goal")
    
    # plot the tree
    tree_path = os.path.join(OUTPUT_FOLDER_PATH, f"tree_{problem}.txt")
    tree = parse_tree(tree_path)
    for edge in tree:
        plt.plot([edge[0][0], edge[1][0]], [edge[0][1], edge[1][1]], 'b')
    
    # set aspect ratio to 1
    ax.set_aspect('equal')
    plt.show()
    

if __name__ == "__main__":
    main()