
#include "headers/Viewer.hpp"

#if WIN32
#include <windows.h>
#else
#include <X11/Xlib.h>
#endif

using namespace std;
using namespace cv;

// void getScreenResolution(int& width, int& height) {
// #if WIN32
//     width = (int)GetSystemMetrics(SM_CXSCREEN);
//     height = (int)GetSystemMetrics(SM_CYSCREEN);
// #else
//     Display* disp = XOpenDisplay(NULL);
//     Screen* scrn = DefaultScreenOfDisplay(disp);
//     width = scrn->width;
//     height = scrn->height;
// #endif
// }'

Viewer::Viewer() {
    cPName = "current Points";
    cPFtName = "filtered points";
    vis3dName = "3D visualization";
    lineName = "line screen";
    trajName = "trajectory";
    matchName = "match screen";
    // get screen resolution
    // char* command = "xrandr | grep '*'";
    // FILE* fpipe = (FILE*)popen(command, "r");
    // char line[256];
    // while (fgets(line, sizeof(line), fpipe)) {
    //     printf("%s", line);
    //     break;
    // }
    // pclose(fpipe);

    namedWindow(cPName);
    namedWindow(cPFtName);
    namedWindow(lineName);
    namedWindow(trajName);
    namedWindow(matchName);

    moveWindow(cPName, 0, 0);
    moveWindow(cPFtName, FULL_WIDTH / 2, FULL_HEIGHT / 2);
    moveWindow(lineName, 0, FULL_HEIGHT / 2);
    moveWindow(trajName, FULL_WIDTH / 2, FULL_HEIGHT / 2);
    moveWindow(matchName, 0, 300);

    // namedWindow(vis3dName);

    currPoints = Mat(_HEIGHT, _WIDTH, CV_8UC1, Scalar(0));
    filterPts = Mat(_HEIGHT, _WIDTH, CV_8UC3, Scalar(0, 0, 0));
    lineImg = Mat(_HEIGHT, _WIDTH, CV_8UC3, Scalar(0, 0, 0));
    trajImg = Mat(_HEIGHT, _WIDTH, CV_8UC1, Scalar(0));
    matchImg = Mat(_MATCH_HEIGHT, _MATCH_WIDTH, CV_8UC1, Scalar(0));

    // imshow(lineName, lineImg);
    // waitKey(100);
    // cout << "img:" << lineImg.empty() << endl;
    // vis3d = Mat(_HEIGHT, _WIDTH, CV_8UC1, Scalar(0));
}

Viewer::~Viewer() {
}

void Viewer::updateScreens(Scan* points_mm, LineRep* prevRep, LineRep* currRep, vector<int> matchesIDx, Trajectory* trajectory) {
    updateReadScreen(points_mm);
    updateFilterScreen(points_mm);
    updateLinesScreen(currRep);
    updateTrajScreen(trajectory);
    updateMatchScreen(prevRep, currRep, matchesIDx);
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

    int prev_id = -1;

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
    float x_start, x_end, y_start, y_end;

    cout << "line Screen..." << endl;
    lineImg.setTo(Scalar(0, 0, 0));
    cout << "reset done" << endl;

    int H, S, V;
    H = 0;
    S = 255;
    V = 255;

    cout << "line size: " << lineRep->size << endl;

    for (int i = 0; i < lineRep->size; i++) {
        int minX = lineRep->lines[i].minX;
        int maxX = lineRep->lines[i].maxX;
        int minY = lineRep->lines[i].minY;
        int maxY = lineRep->lines[i].maxY;

        if (abs(maxX - minX) > abs(maxY - minY)) {
            y_start = lineRep->lines[i].a * minX + lineRep->lines[i].b;
            y_end = lineRep->lines[i].a * maxX + lineRep->lines[i].b;
            x_start = minX;
            x_end = maxX;
        } else {
            y_start = minY;
            y_end = maxY;
            x_start = (minY - lineRep->lines[i].b) / lineRep->lines[i].a;
            x_end = (maxY - lineRep->lines[i].b) / lineRep->lines[i].a;
        }

        // cout << "ptx: (" << minX << "," << lineRep->lines[i].b << ")" << endl;
        // cout << "pty: (" << lineRep->lines[i].maxX << "," << y_end << ")" << endl;
        // if (lineRep->lines[i].niceLine)
        int objID = lineRep->lines[i].objID;
        line(lineImg, cv::Point2d(x_start, y_start), cv::Point2d(x_end, y_end), Scalar((int)hScale * lineRep->lines[i].objID, S, V), 2, FILLED);

        circle(lineImg, cv::Point2d(20 + _WIDTH / (lineRep->size + 2) * objID, _HEIGHT - 100), 4, Scalar((int)hScale * objID, S, V), FILLED);
    }

    cout << "showing on screen" << endl;

    cvtColor(lineImg, lineImg, COLOR_HSV2BGR);
    imshow(lineName, lineImg);
}

void Viewer::updateTrajScreen(Trajectory* trajectory) {
    trajImg.setTo(0);

    int centerx = _WIDTH / 2;
    int centery = _HEIGHT / 2;

    float lenx = trajectory->maxx - trajectory->minx + 10;
    float leny = trajectory->maxy - trajectory->miny + 10;

    cout << "lenx: " << lenx << endl;

    float scaleTrajx = _WIDTH / (2.0 * lenx);
    float scaleTrajy = _HEIGHT / (2.0 * leny);

    cout << "scaleTrajx: " << scaleTrajx << endl;

    std::list<cv::Point2f>::iterator it;
    for (it = trajectory->traj_list.begin(); it != trajectory->traj_list.end(); ++it) {
        cout << "x: " << it->x * scaleTrajx + centerx << " | y:" << it->y * scaleTrajy + centery << endl;
        circle(trajImg, Point(it->x * scaleTrajx + centerx, it->y * scaleTrajy + centery), 4, Scalar(255), FILLED);
    }

    imshow(trajName, trajImg);
}

void Viewer::updateMatchScreen(LineRep* prevRep, LineRep* currRep, std::vector<int> matchesIDx) {
    const float scaleLine2Matchx = (float)_MATCH_WIDTH / _WIDTH;
    const float scaleLine2Matchy = (float)_MATCH_HEIGHT / _HEIGHT;

    matchImg.setTo(0);

    for (int i = 0; i < matchesIDx.size(); i++) {
        if (matchesIDx[i] == -1) continue;

        float mx_curr = currRep->lines[matchesIDx[i]].mx * scaleLine2Matchx;
        float my_curr = currRep->lines[matchesIDx[i]].my * scaleLine2Matchy;
        float mx_prev = prevRep->lines[i].mx * scaleLine2Matchx;
        float my_prev = prevRep->lines[i].my * scaleLine2Matchy;

        cout << "mx_prev: " << mx_prev << " my_prev: " << my_prev << endl;

        line(matchImg, Point(mx_prev, my_prev), Point(mx_curr, my_curr), Scalar(255), 4);
    }

    imshow(matchName, matchImg);
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
