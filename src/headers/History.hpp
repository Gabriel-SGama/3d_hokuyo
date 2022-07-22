#pragma once

#include <opencv2/highgui.hpp>

#include "Const.hpp"

class History {
   private:
    std::list<objects3d> objs;
    float last_x, last_y;
    float dist_changex, dist_changey;

    Trajectory* trajectory;

   public:
    History();

    std::vector<int> matching(LineRep* prevLines, LineRep* currLines);
    void updateTrajectory(LineRep* prevLines, LineRep* currLines, std::vector<int> matchesIdx);
    void updateVision3d(LineRep* prevLines, LineRep* currLines, std::vector<int> matchesIdx);

    inline Trajectory* getTraj() {
        return trajectory;
    };

    inline std::list<objects3d> get3dobjs() {
        return objs;
    }

    ~History();
};
