#pragma once

#include <opencv2/highgui.hpp>

#include "Const.hpp"

class History {
   private:
    /* data */
    std::list<objects3d> objs;
    float last_x, last_y;

    Trajectory* trajectory;

   public:
    History(/* args */);

    std::vector<int> matching(LineRep* prevLines, LineRep* currLines);
    void updateTrajectory(LineRep* prevLines, LineRep* currLines, std::vector<int> matchesIdx);
    void updateVision3d(LineRep* prevLines, LineRep* currLines, std::vector<int> matchesIdx);

    inline Trajectory* getTraj() {
        return trajectory;
    };

    ~History();
};