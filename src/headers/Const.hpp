#pragma once

#include <iostream>
#include <vector>

#define HOKUYO_READER 0
#define FILE_READER 1

#define _HEIGHT 800
#define _WIDTH 800
#define _MAX_DIST 4000.0

#define _MAX_ANGLE_DIFF 0.1
#define _MAX_DIST_DIFF 150  // 20cm
#define _MIN_PTS_2OBJ 8

const float scalex = _WIDTH / _MAX_DIST;
const float scaley = _HEIGHT / _MAX_DIST;
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
    int minX, maxX;
    int minY, maxY;
    int objID;
} Line;

typedef struct LineRep {
    std::vector<Line> lines;
    int size;
} LineRep;

typedef struct Scan {
    std::vector<Points> pts;
    int size, max_size, nIDs;
} Scan;
