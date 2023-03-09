#include <list>
#include <string>

#define MAX_VELOCITY 5.0
#define MAX_STEEERING_VELOCITY 3.141592*0.5
#define MAX_ACCERATION 2.0
#define MAX_STEEERING_ACCELERATION 3.141592*0.5
#define MAX_EULER_ITERATIONS 1000
#define CONTROL_ACCERATION_FIDELITY 0.5
#define CONTROL_STEEERING_FIDELITY 0.5


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


struct RobotState {
    float t;
    float x;
    float y;
    float theta;
    float v;
    float w;
};

class RobotTrajectory {
    public:
        RobotState initial_state;
        std::list<RobotState> states;
        bool is_valid;
        
        RobotTrajectory(RobotState start);
        void log_info();
        void propogate_until_distance(float acceleration, float steering_acceleration, float distance, float time_step);
        void export_trajectory(std::string filename, float acceleration, float steering_acceleration);
};


// function prototypes
Node random_node();
float distance_between_nodes(Node* n1, Node* n2);
Node get_node_in_direction(Node* n1, Node* n2, float distance);