from matplotlib import pyplot as plt
import os
import sys

DATA_FOLDER_PATH = "G:\\My Drive\\UMD\\Spring 2023\\ENAE788V\\Code\\enae788v\\data\\hw1"
OUTPUT_FOLDER_PATH = "G:\\My Drive\\UMD\\Spring 2023\\ENAE788V\\Code\\enae788v\\output\\hw1"

def parse_nodes(file_path):
    nodes = {}
    with open(file_path, 'r') as f:
        f.readline() # skip the first line
        for line in f:
            node_info = line.split(",")
            node_id = int(node_info[0])
            x = float(node_info[1])
            y = float(node_info[2])
            nodes[node_id] = (x, y)
    return nodes

def parse_edges(file_path):
    edges = set()
    
    with open(file_path, 'r') as f:
        f.readline() # skip the first line
        for line in f:
            edge_info = line.split(",")
            node_1 = int(edge_info[0])
            node_2 = int(edge_info[1])
            cost = float(edge_info[2])
            sorted_edge = tuple(sorted([node_1, node_2]))
            edges.add(sorted_edge)
            
    return edges
    

def parse_path(file_path):
    
    search_path = []
    with open(file_path, 'r') as f:
        for line in f:
            search_path.append(int(line.split(",")[0]))
            
    return search_path

def parse_tree(file_path):
    
    search_tree = []
    with open(file_path, 'r') as f:
        for line in f:
            data = line.split(",")
            search_tree.append((int(data[0]), int(data[3])))
            
    return search_tree
    
        

def main():
    problem = sys.argv[1]
    start = int(sys.argv[2])
    end = int(sys.argv[3])
    
    node_file_path = os.path.join(DATA_FOLDER_PATH, f"nodes_{problem}.txt")
    edges_file_path = os.path.join(DATA_FOLDER_PATH, f"edges_with_costs_{problem}.txt")
    path_file_path = os.path.join(OUTPUT_FOLDER_PATH, f"output_path_{problem}_test2.txt")
    tree_file_path = os.path.join(OUTPUT_FOLDER_PATH, f"search_tree_{problem}_test2.txt")
    
    
    nodes = parse_nodes(node_file_path)
    edges = parse_edges(edges_file_path)
    
    fig = plt.figure()
    
    for edge in edges:
        node_1 = nodes[edge[0]]
        node_2 = nodes[edge[1]]
        x = [node_1[0], node_2[0]]
        y = [node_1[1], node_2[1]]
        plt.plot(x, y, color='black')
        
    for node in nodes:
        x = nodes[node][0]
        y = nodes[node][1]
        plt.scatter(x, y, color='blue')
        # add a label to the node with its id
        #plt.annotate(node, (x+1, y+1))
        
    search_path = parse_path(path_file_path)
    search_tree = parse_tree(tree_file_path)
        
    for edge in search_tree:
        node_1 = nodes[edge[0]]
        node_2 = nodes[edge[1]]
        x = [node_1[0], node_2[0]]
        y = [node_1[1], node_2[1]]
        plt.plot(x, y, color='green')
        
    for i in range(len(search_path)-1):
        node_1 = nodes[search_path[i]]
        node_2 = nodes[search_path[i+1]]
        x = [node_1[0], node_2[0]]
        y = [node_1[1], node_2[1]]
        plt.plot(x, y, color='red')
        
    start_node = nodes[start]
    end_node = nodes[end]
    # label start and end nodes
    plt.scatter(start_node[0], start_node[1], color='red')
    plt.annotate("Start", (start_node[0]+1, start_node[1]+1))
    plt.scatter(end_node[0], end_node[1], color='red')
    plt.annotate("End", (end_node[0]+1, end_node[1]+1))
        
    plt.show()

if __name__ == '__main__':
    main()