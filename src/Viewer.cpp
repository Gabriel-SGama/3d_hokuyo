
#include "headers/Viewer.hpp"

#if WIN32
#include <windows.h>
#else
#include <X11/Xlib.h>
#endif

using namespace std;
using namespace cv;

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
    namedWindow(vis3dName);

    moveWindow(cPName, 0, 0);
    moveWindow(cPFtName, _WIDTH, 0);
    moveWindow(lineName, 2 * _WIDTH, 0);
    moveWindow(matchName, 0, _HEIGHT);
    moveWindow(vis3dName, _WIDTH, _HEIGHT);
    moveWindow(trajName, 2 * _WIDTH, _HEIGHT);

    currPoints = Mat(_HEIGHT, _WIDTH, CV_8UC1, Scalar(0));
    filterPts = Mat(_HEIGHT, _WIDTH, CV_8UC3, Scalar(0, 0, 0));
    lineImg = Mat(_HEIGHT, _WIDTH, CV_8UC3, Scalar(0, 0, 0));
    trajImg = Mat(_HEIGHT, _WIDTH, CV_8UC1, Scalar(0));
    matchImg = Mat(_MATCH_HEIGHT, _MATCH_WIDTH, CV_8UC1, Scalar(0));
    vis3d = Mat(_HEIGHT, _WIDTH, CV_8UC1, Scalar(0));

    // imshow(lineName, lineImg);
    // waitKey(100);
    // cout << "img:" << lineImg.empty() << endl;
}

Viewer::~Viewer() {
}

void Viewer::updateScreens(Scan* points_mm, LineRep* prevRep, LineRep* currRep, vector<int> matchesIDx, Trajectory* trajectory, list<objects3d> objs) {
    updateReadScreen(points_mm);
    updateFilterScreen(points_mm);
    updateLinesScreen(currRep);
    updateTrajScreen(trajectory);
    updateMatchScreen(prevRep, currRep, matchesIDx);
    update3DVis(objs);
    waitKey(40);
}

void Viewer::updateReadScreen(Scan* points_mm) {
    int centerx = 0;
    int centery = _HEIGHT / 2;

    currPoints.setTo(0);

    cout << "updating reading screen" << endl;
    cout << "points size: " << points_mm->size << endl;

    for (int i = 0; i < points_mm->size; i++) {
        int x = points_mm->pts[i].x_img;
        int y = points_mm->pts[i].y_img;
        // cout << "read xy" << endl;
        circle(currPoints, Point(x, y), 1, Scalar(255), FILLED);
        // cout << "draw circle" << endl;
    }

    cout << "show img" << endl;
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

        float y_start = lineRep->lines[i].y_start;
        float y_end = lineRep->lines[i].y_end;
        float x_start = lineRep->lines[i].x_start;
        float x_end = lineRep->lines[i].x_end;

        // cout << "ptx: (" << minX << "," << lineRep->lines[i].b << ")" << endl;
        // cout << "pty: (" << lineRep->lines[i].maxX << "," << y_end << ")" << endl;
        // if (lineRep->lines[i].niceLine)

        int objID = lineRep->lines[i].objID;
        line(lineImg, cv::Point2d(x_start, y_start), cv::Point2d(x_end, y_end), Scalar((int)hScale * lineRep->lines[i].objID, S, V), 2, FILLED);

        // circle(lineImg, cv::Point2d(20 + _WIDTH / (lineRep->size + 2) * objID, _HEIGHT - 100), 4, Scalar((int)hScale * objID, S, V), FILLED);
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

    float scaleTraj = scaleTrajx < scaleTrajy ? scaleTrajx : scaleTrajy;

    std::list<cv::Point2f>::iterator it;
    for (it = trajectory->traj_list.begin(); it != trajectory->traj_list.end(); ++it) {
        // cout << "x: " << it->x * scaleTraj + centerx << " | y:" << it->y * scaleTraj + centery << endl;
        circle(trajImg, Point(it->x * scaleTraj + centerx, it->y * scaleTraj + centery), 4, Scalar(255), FILLED);
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

void Viewer::update3DVis(list<objects3d> objs) {
    const float scalex3dVis = scaley;
    const float scaley3dVis = _HEIGHT / (_MAX_DIST * angle_sin);

    const float scaleBright = 255.0 / _MAX_DIST;

    std::list<objects3d>::iterator it;

    vis3d.setTo(0);

    for (it = objs.begin(); it != objs.end(); ++it)
        rectangle(vis3d, Rect(it->tleft, it->bright), Scalar(255 - scaleBright * it->real_my), FILLED);

    imshow(vis3dName, vis3d);
}
