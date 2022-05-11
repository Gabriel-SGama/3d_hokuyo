
#include "headers/Viewer.hpp"

using namespace std;
using namespace cv;

Viewer::Viewer() {
    cPName = "current Points";
    cPFtName = "filtered points";
    vis3dName = "3D visualization";
    lineName = "line screen";

    namedWindow(cPName);
    namedWindow(cPFtName);
    namedWindow(lineName);
    // namedWindow(vis3dName);

    currPoints = Mat(_HEIGHT, _WIDTH, CV_8UC1, Scalar(0));
    filterPts = Mat(_HEIGHT, _WIDTH, CV_8UC3, Scalar(0, 0, 0));
    lineImg = Mat(_HEIGHT, _WIDTH, CV_8UC3, Scalar(0, 0, 0));
    // imshow(lineName, lineImg);
    // waitKey(100);
    // cout << "img:" << lineImg.empty() << endl;
    // vis3d = Mat(_HEIGHT, _WIDTH, CV_8UC1, Scalar(0));
}

Viewer::~Viewer() {
}

void Viewer::updateScreens(Scan* points_mm, LineRep* lineRep) {
    // calculates points on img
    // for (int i = 0; i < points_mm->size; i++) {
    //     // int x = points_mm->pts[i].x * scalex + centerx;
    //     // int y = -points_mm->pts[i].y * scaley + centery;

    //     // int x = points_mm->pts[i].y * scaley + _WIDTH / 2;
    //     // int y = points_mm->pts[i].x * scalex;

    //     points_mm->pts[i].x_img = points_mm->pts[i].y * scaley + _WIDTH / 2;
    //     points_mm->pts[i].y_img = points_mm->pts[i].x * scalex;
    // }

    updateReadScreen(points_mm);
    updateFilterScreen(points_mm);
    updateLinesScreen(lineRep);
    // this->update3DVis(points_mm);
    waitKey(0);
}

void Viewer::updateReadScreen(Scan* points_mm) {
    int centerx = 0;
    int centery = _HEIGHT / 2;

    currPoints.setTo(0);

    for (int i = 0; i < points_mm->size; i++) {
        int x = points_mm->pts[i].x_img;
        int y = points_mm->pts[i].y_img;

        circle(currPoints, Point(x, y), 1, Scalar(255), FILLED);
    }

    imshow(cPName, currPoints);
}

void Viewer::updateFilterScreen(Scan* points_mm) {
    int centerx = 0;
    int centery = _HEIGHT / 2;

    float hScale = 179.0 / points_mm->nIDs;

    filterPts.setTo(0);

    int H, S, V;
    H = 0;
    S = 255;
    V = 255;

    for (int i = 0; i < points_mm->size; i++) {
        int id = points_mm->pts[i].objID;

        if (id == -1)
            continue;

        int x = points_mm->pts[i].x_img;
        int y = points_mm->pts[i].y_img;

        circle(filterPts, Point(x, y), 4, Scalar((int)hScale * id, S, V), FILLED);
    }

    cvtColor(filterPts, filterPts, COLOR_HSV2BGR);
    imshow(cPFtName, filterPts);
}

void Viewer::updateLinesScreen(LineRep* lineRep) {
    float hScale = 179.0 / lineRep->size;

    cout << "line Screen..." << endl;
    lineImg.setTo(Scalar(0, 0, 0));
    cout << "reset done" << endl;

    int H, S, V;
    H = 0;
    S = 255;
    V = 255;

    cout << "line size: " << lineRep->size << endl;
    for (int i = 0; i < lineRep->size; i++) {
        float y_start = lineRep->lines[i].a * lineRep->lines[i].minX + lineRep->lines[i].b;
        float y_end = lineRep->lines[i].a * lineRep->lines[i].maxX + lineRep->lines[i].b;

        y_end = round(y_end);

        // cout << "ptx: (" << lineRep->lines[i].minX << "," << lineRep->lines[i].b << ")" << endl;
        // cout << "pty: (" << lineRep->lines[i].maxX << "," << y_end << ")" << endl;

        line(lineImg, cv::Point2d(lineRep->lines[i].minX, y_start), cv::Point2d(lineRep->lines[i].maxX, y_end), Scalar((int)hScale * lineRep->lines[i].objID, S, V), 2, FILLED);
    }

    cout << "showing on screen" << endl;

    cvtColor(lineImg, lineImg, COLOR_HSV2BGR);
    imshow(lineName, lineImg);
}

void Viewer::update3DVis(Scan* points_mm) {
    // TODO: make currPoints into the same orientation
    int centerx = _WIDTH / 2;
    int centery = _HEIGHT;

    // vis3d.setTo(0);

    // TODO: SAVE DIST FROM HOKUYO
    for (int i = 0; i < points_mm->size; i++) {
        int x = points_mm->pts[i].x;
        int y = points_mm->pts[i].y;
        float dist = x * x + y * y;

        x = x * scalex + centerx;
        y = y * scaley + centery;

        // int x = points_mm->pts[i].x * scalex + centerx;
        // int y = -points_mm->pts[i].y * scaley + centery;
        // cout << "x: " << x << ", y: " << y << endl;
        // cout << "points_mm[i].x: " << points_mm[i].x << ", points_mm[i].y: " << points_mm[i].y << endl;
        // Points.at<uchar>(x, y) = 255;
        circle(currPoints, Point(x, y), 4, Scalar(255), FILLED);
    }
}
