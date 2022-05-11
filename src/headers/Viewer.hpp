#pragma once

#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "Const.hpp"
#include "Urg_driver.h"

class Viewer {
   private:
    cv::Mat currPoints, filterPts, lineImg, vis3d;
    std::string cPName, cPFtName, lineName, vis3dName;

   public:
    Viewer();
    ~Viewer();
    void updateScreens(Scan* points_mm, LineRep* lineRep);
    void updateReadScreen(Scan* points_mm);
    void updateFilterScreen(Scan* points_mm);
    void updateLinesScreen(LineRep* lineRep);
    void update3DVis(Scan* points_mm);
};