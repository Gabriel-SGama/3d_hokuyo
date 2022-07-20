#pragma once

// c++
#include <iostream>
#include <list>
#include <vector>

// Opencv
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define HOKUYO_READER 0
#define FILE_READER 1

#define _HEIGHT 800
#define _WIDTH 800
#define _MAX_DIST 4000.0

#define _MATCH_HEIGHT 200
#define _MATCH_WIDTH 200

#define FULL_WIDTH 1920
#define FULL_HEIGHT 1080

#define _MAX_ANGLE_DIFF 0.1
#define _MAX_DIST_DIFF 70  // mm
#define _MIN_PTS_2OBJ 20

#define _MATCHING_CENTER_DIST 15

const float scalex = _WIDTH / (2 * _MAX_DIST);
const float scaley = _HEIGHT / (2 * _MAX_DIST);

const float scaleBright = 255.0 / _MAX_DIST;
// const float hkAngle = 15.0 * 3.14159265358979323846 / 180.0;

typedef struct Points {
    int x, y, dist;
    int x_img, y_img;
    float angle;
    int objID;
} Points;

typedef struct Line {
    float a, b;
    float mx, my;  // mean cord values
    float std;
    float resi;
    int minX, maxX;
    int minY, maxY;
    int objID;
    bool niceLine;
} Line;

typedef struct LineRep {
    std::vector<Line> lines;
    int size;
} LineRep;

typedef struct Scan {
    std::vector<Points> pts;
    int size, max_size, nIDs;
} Scan;

typedef struct Trajectory {
    std::list<cv::Point2f> traj_list;
    std::list<Line> savedLines;

    float maxx, maxy;
    float minx, miny;
} Trajectory;