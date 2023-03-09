from matplotlib import pyplot as plt
import os
import sys
import json

DATA_FOLDER_PATH = "G:\\My Drive\\UMD\\Spring 2023\\ENAE788V\\Code\\enae788v\\data\\hw3"
OUTPUT_FOLDER_PATH = "G:\\My Drive\\UMD\\Spring 2023\\ENAE788V\\Code\\enae788v\\output\\hw3"


def main():
    traj = OUTPUT_FOLDER_PATH + "\\tests\\trajectory.csv"
    
    x = []
    y = []
    with open(traj, 'r') as f:
        for line in f:
            line_info = line.split(",")
            x.append(float(line_info[1]))
            y.append(float(line_info[2]))
            
    fig = plt.figure()
    ax = plt.axes()
    
    # set limits to -50 to 50 in both x and y
    ax.set_xlim(-50, 50)
    ax.set_ylim(-50, 50)
    
    # create line plot
    ax.plot(x, y)
    
    plt.show()
    
    
    
if __name__ == "__main__":
    main()