#include <list>
#include <string>

class Node {
    public:
        float x;
        float y;
        
        Node* parent;
        
        Node(float x, float y);
};


class Tree {
    public:
        std::list<Node*> nodes;
        
        Tree();
        void log_info();
        Node* closest_node(Node* node);
        void add_node(Node* node);
        bool export_tree(std::string filename);
        bool export_path(std::string filename, Node* goal);
};


class Collidable {
    public:
        float x;
        float y;
        float radius;
        
        Collidable(float x, float y, float radius);
        void log_info();
        bool is_point_in_collision(Node* n);
        bool is_segment_in_collision(Node* n1, Node* n2, int divisions = 10);
        
};


class Obstacles {
    public:
        std::list<Collidable*> obstacles;
        
        Obstacles();
        void log_info();
        void parse_from_obstacle_file(std::string filename);    
        bool is_point_in_collision(Node* n);
        bool is_segment_in_collision(Node* n1, Node* n2, int divisions = 10);
};


// function prototypes
Node random_node();
float distance_between_nodes(Node* n1, Node* n2);
Node get_node_in_direction(Node* n1, Node* n2, float distance);