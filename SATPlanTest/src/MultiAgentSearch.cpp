
#include <z3++.h>
#include "../inc/MultiAgentSearch.h"


bool MultiAgentSearch::BoolSat(std::vector<std::vector<int>> pathImg, std::vector<Agent> &Agents) {
    graphCheck(pathImg, Agents);
    z3::context c;
    z3::solver s(c);
    int agentNum = Agents.size();

    z3::check_result isSat = z3::unsat;
    std::string expr_name;

    int rows = pathImg.size();
    int cols = pathImg[0].size();

    int firstTimeSat = maxDistance(Agents);

    clock_t startTime,endTime;
    try {
        startTime = clock();
        for(int agt = 0; agt < agentNum; agt++){
            s.add(c.bool_const((Agents[agt].name + "_t" + std::to_string(timeStep) + "_x" + std::to_string(Agents[agt].startVertex[0]) + "_y" +
                                std::to_string(Agents[agt].startVertex[1])).c_str()));
        }

        while (isSat == z3::unsat) {
            for (int agt = 0; agt < agentNum; agt++) {
                std::vector<std::vector<int>> range = getRange(timeStep,Agents[agt].startVertex[0],Agents[agt].startVertex[1],rows,cols);
                for (int x = range[0][0]; x < range[0][1]; x++) {
                    for (int y = range[1][0]; y < range[1][1]; y++) {
                        if (pathImg[x][y] == 0) {
                            z3::expr expr_A = c.bool_const(
                                    (Agents[agt].name + "_t" + std::to_string(timeStep) + "_x" + std::to_string(x) + "_y" +
                                     std::to_string(y)).c_str());
                            z3::expr_vector exprVector(c);
                            if (x >= 1 && y >= 1 && pathImg[x - 1][y - 1] == 0) {    //左上
                                exprVector.push_back(c.bool_const(
                                        (Agents[agt].name + "_t" + std::to_string(timeStep + 1) + "_x" + std::to_string(x - 1) + "_y" +
                                         std::to_string(y - 1)).c_str()));
                            }
                            if (x >= 1 && pathImg[x - 1][y] == 0) {    //上
                                exprVector.push_back(c.bool_const(
                                        (Agents[agt].name + "_t" + std::to_string(timeStep + 1) + "_x" + std::to_string(x - 1) + "_y" +
                                         std::to_string(y)).c_str()));
                            }
                            if (x >= 1 && y <= cols - 2 && pathImg[x - 1][y + 1] == 0) {    //右上
                                exprVector.push_back(c.bool_const(
                                        (Agents[agt].name + "_t" + std::to_string(timeStep + 1) + "_x" + std::to_string(x - 1) + "_y" +
                                         std::to_string(y + 1)).c_str()));
                            }
                            if (y >= 1 && pathImg[x][y - 1] == 0) {    //左
                                exprVector.push_back(c.bool_const(
                                        (Agents[agt].name + "_t" + std::to_string(timeStep + 1) + "_x" + std::to_string(x) + "_y" +
                                         std::to_string(y - 1)).c_str()));
                            }
                            if (y <= cols - 2 && pathImg[x][y + 1] == 0) {    //右
                                exprVector.push_back(c.bool_const(
                                        (Agents[agt].name + "_t" + std::to_string(timeStep + 1) + "_x" + std::to_string(x) + "_y" +
                                         std::to_string(y + 1)).c_str()));
                            }
                            if (x <= rows - 2 && y >= 1 && pathImg[x + 1][y - 1] == 0) {    //左下
                                exprVector.push_back(c.bool_const(
                                        (Agents[agt].name + "_t" + std::to_string(timeStep + 1) + "_x" + std::to_string(x + 1) + "_y" +
                                         std::to_string(y - 1)).c_str()));
                            }
                            if (x <= rows - 2 && pathImg[x + 1][y] == 0) {    //下
                                exprVector.push_back(c.bool_const(
                                        (Agents[agt].name + "_t" + std::to_string(timeStep + 1) + "_x" + std::to_string(x + 1) + "_y" +
                                         std::to_string(y)).c_str()));
                            }
                            if (x <= rows - 2 && y <= cols - 2 && pathImg[x + 1][y + 1] == 0) {    //右下
                                exprVector.push_back(c.bool_const(
                                        (Agents[agt].name + "_t" + std::to_string(timeStep + 1) + "_x" + std::to_string(x + 1) + "_y" +
                                         std::to_string(y + 1)).c_str()));
                            }
                            exprVector.push_back(c.bool_const(
                                    (Agents[agt].name + "_t" + std::to_string(timeStep + 1) + "_x" + std::to_string(x) + "_y" +
                                     std::to_string(y)).c_str()));
                            s.add(not(expr_A) or mk_or(exprVector));


                            z3::expr_vector exprVector_C(c);
                            z3::expr_vector exprVector_D(c);
                            for(int agt2 = 0; agt2 < agentNum; agt2++){
                                if(agt2 != agt){
                                    exprVector_C.push_back(c.bool_const(
                                            (Agents[agt2].name + "_t" + std::to_string(timeStep) + "_x" + std::to_string(x) + "_y" +
                                             std::to_string(y)).c_str()));
                                    exprVector_D.push_back(c.bool_const(
                                            (Agents[agt2].name + "_t" + std::to_string(timeStep + 1) + "_x" + std::to_string(x) + "_y" +
                                             std::to_string(y)).c_str()));
                                }
                            }
                            // only one agent per location at a time
                            s.add(not(expr_A and mk_or(exprVector_C)));
                            // multiple agents cannot enter or leave a location at the adjacent time
                            s.add(not(expr_A and mk_or(exprVector_D)));
                        }
                    }
                }
            }
            if(timeStep > firstTimeSat){
                s.push();
                for(int agt = 0; agt < agentNum; agt++){
                    s.add(c.bool_const((Agents[agt].name + "_t" + std::to_string(timeStep + 1) + "_x" + std::to_string(Agents[agt].endVertex[0]) + "_y" +
                                        std::to_string(Agents[agt].endVertex[1])).c_str()));
                    for(int x = 0; x < rows; x++){
                        for(int y = 0; y < cols; y++){
                            if(pathImg[x][y] == 0){
                                if(not(x == Agents[agt].endVertex[0] and y == Agents[agt].endVertex[1])){
                                    s.add(not c.bool_const((Agents[agt].name + "_t" + std::to_string(timeStep + 1) + "_x" + std::to_string(x) + "_y" +
                                                            std::to_string(y)).c_str()));
                                }
                            }
                        }
                    }
                }
                isSat = s.check();
                s.pop();
            }
            endTime = clock();
            runtime = double(endTime - startTime)/CLOCKS_PER_SEC;
            std::cout << "第" << timeStep << "步的解：" << isSat<<"\t完成时间："<<runtime << "\n";

            timeStep++;

        }

        if(isSat == z3::sat){
            z3::model m = s.get_model();
//            for (int i = 0; i < m.size(); i++) {
//////                std::cout << m.get_const_interp(m[i])<<"\n";
//                if(m.get_const_interp(m[i]).to_string() == "true"){
//                    std::cout<<m[i].name()<<"\n";
//                }
//            }

//            printf(m.eval(c.bool_const("B_t0_x2_y0"), true).to_string().c_str());
            for(int t = 0; t < timeStep + 2; t++){
                for(int agt = 0; agt < Agents.size(); agt++){
                    for(int x=0; x < rows; x++){
                        for(int y=0; y < cols; y++){
                            if(pathImg[x][y] == VERTEX){
                                if(m.eval(c.bool_const((Agents[agt].name + "_t" + std::to_string(t) + "_x" + std::to_string(x) + "_y" +
                                                        std::to_string(y)).c_str())).to_string() == "true"){
                                    Agents[agt].path.push_back({x,y});
                                }
                            }
                        }
                    }
                }
            }
            return true;
        }
    } catch (z3::exception ex) {
        std::cout << "unexpected error: " << ex << "\n";

    }
    return false;
}


bool MultiAgentSearch::LinearSat(std::vector<std::vector<int>> pathImg, std::vector<Agent> &Agents) {
    std::vector<std::vector<int>> graph;
    for(int x=0; x < pathImg.size()+2; x++){
        std::vector<int> r;
        for(int y=0; y < pathImg[0].size()+2; y++){
            r.push_back(OBSTACLE);
        }
        graph.push_back(r);
    }
    for(int x=0; x < pathImg.size(); x++)
        for(int y=0; y < pathImg[0].size(); y++)
            graph[x+1][y+1] = pathImg[x][y];

    for(int agt = 0; agt < Agents.size(); agt++){
        Agents[agt].startVertex[0]++;
        Agents[agt].startVertex[1]++;
        Agents[agt].endVertex[0]++;
        Agents[agt].endVertex[1]++;
    }

    graphCheck(graph, Agents);

    z3::context c;
    z3::solver s(c);
    int agentNum = Agents.size();

    z3::check_result isSat = z3::unsat;
    std::string expr_name;

    int rows = graph.size();
    int cols = graph[0].size();

    int firstTimeSat = maxDistance(Agents);

    clock_t startTime,endTime;
    try {
        startTime = clock();
        for(int agt = 0; agt < agentNum; agt++){
            s.add(c.int_const((Agents[agt].name + "_t" + std::to_string(timeStep)).c_str()) == Agents[agt].startVertex[0] * cols + Agents[agt].startVertex[1]);
        }

        while (isSat == z3::unsat) {
            z3::expr_vector constraint_vector(c);
            for (int agt = 0; agt < agentNum; agt++) {
                z3::expr current = c.int_const((Agents[agt].name + "_t" + std::to_string(timeStep)).c_str());
                z3::expr next = c.int_const((Agents[agt].name + "_t" + std::to_string(timeStep+1)).c_str());
                z3::expr MvUp = current - cols;
                z3::expr MvDown = current + cols;
                z3::expr MvLeft = current - 1;
                z3::expr MvRight = current + 1;
                z3::expr MvLeftUp = current - cols -1;
                z3::expr MvRightUp = current - cols + 1;
                z3::expr MvLeftDown = current + cols -1;
                z3::expr MvRightDown = current + cols + 1;
                s.add(next == current or next == MvUp or next == MvDown or next == MvLeft or next == MvRight
                or next == MvLeftUp or next == MvLeftDown or next == MvRightUp or next == MvRightDown);

                s.add(next >= 0);

                z3::expr_vector constraintArr_graph(c);
                std::vector<std::vector<int>> range = getRange(timeStep,Agents[agt].startVertex[0],Agents[agt].startVertex[1],rows,cols);
                for (int x = range[0][0]; x < range[0][1]; x++) {
                    for (int y = range[1][0]; y < range[1][1]; y++) {
                        if (graph[x][y] == 0)
                            constraintArr_graph.push_back(c.int_const((Agents[agt].name + "_t" + std::to_string(timeStep+1)).c_str()) == x * cols + y);
                    }
                }
                s.add(mk_or(constraintArr_graph));

                for(int agt2 = 0; agt2 < agentNum; agt2++){
                    if(agt != agt2)
                        s.add(c.int_const((Agents[agt2].name + "_t" + std::to_string(timeStep+1)).c_str()) != c.int_const((Agents[agt].name + "_t" + std::to_string(timeStep)).c_str()));
                    if(agt2 > agt)
                        s.add(c.int_const((Agents[agt2].name + "_t" + std::to_string(timeStep+1)).c_str()) != c.int_const((Agents[agt].name + "_t" + std::to_string(timeStep+1)).c_str()));
                }

//                constraint_vector.push_back(next);
            }

//            for(int c1 = 0; c1 < constraint_vector.size(); c1++){
//                for(int c2 = 0; c2 < constraint_vector.size(); c2++) {
//                    if(c1 != c2)
//                        s.add(constraint_vector[c1] != constraint_vector[c2]);
//                }
//            }
            if(timeStep > firstTimeSat){
                s.push();
                for(int agt = 0; agt < agentNum; agt++){
                    s.add(c.int_const((Agents[agt].name + "_t" + std::to_string(timeStep+1)).c_str()) == Agents[agt].endVertex[0] * cols + Agents[agt].endVertex[1]);
                    for(int x = 0; x < rows; x++){
                        for(int y = 0; y < cols; y++){
                            if(graph[x][y] == 0){
                                if(not(x == Agents[agt].endVertex[0] and y == Agents[agt].endVertex[1])){
                                    s.add(c.int_const((Agents[agt].name + "_t" + std::to_string(timeStep+1)).c_str()) != x * cols + y);
                                }
                            }
                        }
                    }
                }
                isSat = s.check();
                s.pop();
            }
            endTime = clock();
            runtime = double(endTime - startTime)/CLOCKS_PER_SEC;
            std::cout << "第" << timeStep << "步的解：" << isSat<<"\t完成时间："<<runtime << "\n";

            timeStep++;

        }

        if(isSat == z3::sat){
            timeStep++;
            z3::model m = s.get_model();

//            for (int i = 0; i < m.size(); i++) {
//////                std::cout << m.get_const_interp(m[i])<<"\n";
//                if(m.get_const_interp(m[i]).to_string() == "true"){
//                    std::cout<<m[i].name()<<"\n";
//                }
//            }

//            printf(m.eval(c.bool_const("B_t0_x2_y0"), true).to_string().c_str());

            for(int t = 0; t < timeStep; t++){
                for(int agt = 0; agt < Agents.size(); agt++){
                    int n = std::stoi(m.eval(c.int_const((Agents[agt].name + "_t" + std::to_string(t)).c_str())).to_string());
//                    std::cout<<n<<std::endl;
                    Agents[agt].path.push_back({n/cols-1,n%cols-1});
                }

            }
            for(int agt = 0; agt < Agents.size(); agt++){
                Agents[agt].startVertex[0]--;
                Agents[agt].startVertex[1]--;
                Agents[agt].endVertex[0]--;
                Agents[agt].endVertex[1]--;
            }
            return true;
        }
    } catch (z3::exception ex) {
        std::cout << "unexpected error: " << ex << "\n";

    }
    return false;
}

