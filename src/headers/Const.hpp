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

#define _HEIGHT 400
#define _WIDTH 400
#define _MAX_DIST 4000.0

#define _MATCH_HEIGHT 200
#define _MATCH_WIDTH 200

#define FULL_WIDTH 1920
#define FULL_HEIGHT 1080

#define _VISION_ANGLE 45

#define _MAX_ANGLE_DIFF 0.1
#define _MAX_DIST_DIFF 40  // mm
#define _MIN_PTS_2OBJ 20

#define _MATCHING_CENTER_DIST 20

const float scalex = _HEIGHT / (2 * _MAX_DIST);
const float scaley = _WIDTH / (_MAX_DIST * sin(_VISION_ANGLE * M_PI / 180.0));
// const float scaley = _HEIGHT / (2 * _MAX_DIST);

// const float scaleBright = 255.0 / _MAX_DIST;
const float hkAngle = 15.0 * M_PI / 180.0;

const float angle_sin = sin(hkAngle);
const float angle_cos = cos(hkAngle);

typedef struct Points {
    int real_x, dist;  // 3d dimension (angle pre defined)
    int x, y;          // converted to the xy plane
    int x_img, y_img;
    float angle;
    int objID;
} Points;

typedef struct Line {
    float a, b;
    float mx, my;  // mean cord values
    float std;
    float resi;
    float x_start, x_end, y_start, y_end;

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

typedef struct objects3d {
    int lastObjID;

    float avg_angle;
    float mx, my;
    float avg_width;
    float start_my;
    float dist;

    bool resetID;

    cv::Point2f tleft, bright;

} objects3d;