
#include "headers/Viewer.hpp"

using namespace std;
using namespace cv;
Viewer::Viewer() {
    namedWindow("currPoints");
    currPoints = cv::Mat(_HEIGHT, _WIDTH, CV_8UC1, cv::Scalar(0));
}

Viewer::~Viewer() {
}

void Viewer::updateScreen(Scan* points_mm) {
    int centerx = 0;
    int centery = _HEIGHT / 2;

    currPoints.setTo(0);

    // cout << "scalex: " << scalex << endl;
    // cout << "scaley: " << scaley << endl;

    for (int i = 0; i < points_mm->size; i++) {
        int x = points_mm->pts[i].x * scalex + centerx;
        int y = -points_mm->pts[i].y * scaley + centery;
        // cout << "x: " << x << ", y: " << y << endl;
        // cout << "points_mm[i].x: " << points_mm[i].x << ", points_mm[i].y: " << points_mm[i].y << endl;
        // points.at<uchar>(x, y) = 255;
        cv::circle(currPoints, cv::Point(x, y), 4, cv::Scalar(255), cv::FILLED);
    }

    cv::imshow("currPoints", currPoints);
    cv::waitKey(1);
}
