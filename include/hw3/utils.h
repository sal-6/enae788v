#include <list>
#include <string>

#define MAX_VELOCITY 5.0
#define MAX_STEEERING_VELOCITY 3.141592*0.5
#define MAX_ACCELERATION 2.0
#define MAX_STEEERING_ACCELERATION 3.141592*0.5
#define MAX_EULER_ITERATIONS 1000
#define CONTROL_ACCERATION_FIDELITY 0.5
#define CONTROL_STEEERING_FIDELITY 0.5
#define MAX_RRT_ITERATIONS 10000


class Node {
    public:
        float x;
        float y;
        
        Node* parent;
        
        Node(float x, float y);
};


struct RobotState {
    float t;
    float x;
    float y;
    float theta;
    float v;
    float w;
    float a;
    float gamma;
};


class RobotTrajectory {
    public:
        RobotState initial_state;
        RobotState final_state;
        std::list<RobotState> states;
        bool is_valid;
        
        RobotTrajectory* parent;
        
        RobotTrajectory(RobotState start);
        void log_info();
        bool propogate_until_distance(float distance, float time_step);
        void export_trajectory(std::string filename);
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
        bool is_trajectory_in_collision(RobotTrajectory* trajectory, float distance_step);
        
};


class Obstacles {
    public:
        std::list<Collidable*> obstacles;
        
        Obstacles();
        void log_info();
        void parse_from_obstacle_file(std::string filename);    
        bool is_point_in_collision(Node* n);
        bool is_segment_in_collision(Node* n1, Node* n2, int divisions = 10);
        bool is_trajectory_in_collision(RobotTrajectory* trajectory, float distance_step);
};


class SearchTree {
    public:
        std::list<RobotTrajectory*> trajectories;
        
        SearchTree();
        void add_trajectory(RobotTrajectory* trajectory);
        RobotTrajectory* get_closest_trajectory_end(Node node);
        bool export_tree(std::string filename);
};


// function prototypes
Node random_node();
float distance_between_nodes(Node* n1, Node* n2);
Node get_node_in_direction(Node* n1, Node* n2, float distance);

