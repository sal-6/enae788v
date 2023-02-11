from matplotlib import pyplot as plt
import os
import json

DATA_FOLDER_PATH = "G:\\My Drive\\UMD\\Spring 2023\\ENAE788V\\Code\\788v\\data\\Hw1"
OUTPUT_FOLDER_PATH = "G:\\My Drive\\UMD\\Spring 2023\\ENAE788V\\Code\\788v\\src\\788v\\Hw1\\Output"

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
    
def plot_search_path(nodes, edges, path):
    for i in range(len(path)-1):
        node_1 = nodes[int(path[i])]
        node_2 = nodes[int(path[i+1])]
        x = [node_1[0], node_2[0]]
        y = [node_1[1], node_2[1]]
        plt.plot(x, y, color='green', linewidth=5)
        

def plot_search_tree(nodes, edges, tree):
    for exp in tree:
        node_1 = nodes[int(exp[0])]
        node_2 = nodes[int(exp[1])]
        x = [node_1[0], node_2[0]]
        y = [node_1[1], node_2[1]]
        plt.plot(x, y, color='pink', linewidth=3)
        

def main():
    problem = 5
    
    node_file_path = os.path.join(DATA_FOLDER_PATH, f"nodes_{problem}.txt")
    edges_file_path = os.path.join(DATA_FOLDER_PATH, f"edges_with_costs_{problem}.txt")
    path_file_path = os.path.join(OUTPUT_FOLDER_PATH, f"path_{problem}.json")
    tree_file_path = os.path.join(OUTPUT_FOLDER_PATH, f"tree_{problem}.json")
    
    
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

    # plot the search tree
    with open(tree_file_path, 'r') as f:
        data = json.load(f)

    plot_search_tree(nodes, edges, data["tree"])
            
    # parse json from the output file
    with open(path_file_path, 'r') as f:
        data = json.load(f) 
    
    plot_search_path(nodes, edges, data["path"])
    
    
    plt.show()

if __name__ == '__main__':
    main()