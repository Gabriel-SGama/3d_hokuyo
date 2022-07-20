#pragma once

#include <opencv2/highgui.hpp>

#include "Const.hpp"

class History {
   private:
    /* data */
    float last_x, last_y;

    Trajectory* trajectory;

   public:
    History(/* args */);

    std::vector<int> matching(LineRep* prevLines, LineRep* currLines);
    void updateTrajectory(LineRep* prevLines, LineRep* currLines, std::vector<int> matchesIdx);

    inline Trajectory* getTraj() {
        return trajectory;
    };

    ~History();
};
