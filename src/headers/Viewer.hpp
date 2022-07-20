#pragma once

#include "Const.hpp"
#include "Urg_driver.h"

class Viewer {
   private:
    int width, height;
    cv::Mat currPoints, filterPts, lineImg, vis3d, trajImg, matchImg;
    std::string cPName, cPFtName, lineName, vis3dName, trajName, matchName;

   public:
    Viewer();
    ~Viewer();
    void updateScreens(Scan* points_mm, LineRep* prevRep, LineRep* currRep, std::vector<int> matchesIDx, Trajectory* trajectory);
    void updateReadScreen(Scan* points_mm);
    void updateFilterScreen(Scan* points_mm);
    void updateLinesScreen(LineRep* lineRep);
    void updateTrajScreen(Trajectory* trajectory);
    void updateMatchScreen(LineRep* prevRep, LineRep* currRep, std::vector<int> matchesIDx);
    void update3DVis(Scan* points_mm);
};