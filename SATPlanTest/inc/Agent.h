#include <string>
#include <vector>


class Agent{
public:
    Agent(std::string AgentName, std::vector<int> start, std::vector<int> end){
        name = AgentName;
        startVertex = start;
        endVertex = end;
    }
    std::string name;
    std::vector<int> startVertex;
    std::vector<int> endVertex;
    std::vector<std::vector<int>> path;
};