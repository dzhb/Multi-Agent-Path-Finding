
#include "tools.h"
#include <vector>


class MultiAgentSearch{
public:
    double runtime = 0;
    int timeStep = 0;

    bool BoolSat(std::vector<std::vector<int>> pathImg, std::vector<Agent> &Agents);
    bool LinearSat(std::vector<std::vector<int>> pathImg, std::vector<Agent> &Agents);

};

