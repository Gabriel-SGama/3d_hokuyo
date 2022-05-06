#pragma once

#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "Const.hpp"
#include "Urg_driver.h"

class Viewer {
   private:
    cv::Mat currPoints;

   public:
    Viewer();
    ~Viewer();
    void updateScreen(Scan* points_mm);
};