from matplotlib import pyplot as plt
import os
import sys

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
    pass


def parse_path(file_path):
    pass


def main():
    obstacle_path = os.path.join(DATA_FOLDER_PATH, "obstacles.txt")
    
    obstacles = parse_obstacles(obstacle_path)
    
    
    fig = plt.figure()
    ax = plt.axes()
    
    # set limits to -50 to 50 in both x and y
    ax.set_xlim(-50, 50)
    ax.set_ylim(-50, 50)
    
    # plot the obstacles
    for obs in obstacles:
        circle = plt.Circle((obs[0], obs[1]), obs[2], color='r')
        ax.add_patch(circle)
        
    # set aspect ratio to 1
    ax.set_aspect('equal')
    plt.show()
    

if __name__ == "__main__":
    main()