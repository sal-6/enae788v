#include <list>
#include <string>

class Node {
    public:
        float x;
        float y;
        float z;
        
        Node(float x, float y, float z);
};


class Tree {};


class Collidable {
    public:
        float x;
        float y;
        float radius;
        
        Collidable(float x, float y, float radius);
        void log_info();
};


class Obstacles {
    public:
        std::list<Collidable*> obstacles;
        
        Obstacles();
        void log_info();
        void parse_from_obstacle_file(std::string filename);    
};