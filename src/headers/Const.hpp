#pragma once

#define HOKUYO_READER 0
#define FILE_READER 1

#define _HEIGHT 800
#define _WIDTH 800

const float scalex = _WIDTH / 4000.0;
const float scaley = _HEIGHT / 4000.0;

typedef struct points {
    int x, y;
} points;

typedef struct Scan {
    std::vector<points> pts;
    int size, max_size;
} Scan;
