#include "headers/Logic.hpp"

#include <cmath>

using namespace std;

Logic::Logic() {
    nID = 0;
    beginSize = 50;  // max size of lines in one frame
    lineRep = new LineRep();
    lineRep->lines.resize(beginSize);
    lineRep->size = 0;
}

Logic::~Logic() {
}

void Logic::defineLimit(Scan* data) {
    int x = data->pts[0].x_img;
    int y = data->pts[0].y_img;
    int dist = data->pts[0].dist;
    int objID = 0;
    int quantPts = 1;
    int maxX = data->pts[0].x_img;
    int minX = data->pts[0].x_img;
    int maxY = data->pts[0].y_img;
    int minY = data->pts[0].y_img;

    float angle = data->pts[0].angle;
    float num, den, mx, my, sumx, sumy, mxx, mxy;

    bool linePred = false;
    bool niceLine = false;
    float apred, bpred;

    num = 0.f;
    den = 0.f;
    mx = 0.f;
    my = 0.f;
    sumx = 0.f;
    sumy = 0.f;
    mxx = 0.f;
    mxy = 0.f;
    quantPts = 0;

    cout << "max vector size: " << lineRep->lines.size() << endl;

    for (int i = 0; i < data->size; i++) {
        if (quantPts > _MIN_PTS_2OBJ && !niceLine) {  // inicial line prediction
            linePred = true;
            niceLine = true;

            mx = sumx / quantPts;
            my = sumy / quantPts;

            num = mxy - sumx * my;
            den = mxx - sumx * mx;

            apred = num / den;
            bpred = my - apred * mx;
            float resi = 0;
            float ypred, yread;

            for (int k = 0; k < quantPts; k++) {
                ypred = data->pts[i - 1 - k].x_img * apred + bpred;
                yread = data->pts[i - 1 - k].y_img;
                resi += (ypred - yread) * (ypred - yread);
            }

            resi /= quantPts;

            if (resi > 17)
                niceLine = false;
            // float y_end = a * maxX + b;
        }

        if (abs(dist - data->pts[i].dist) > _MAX_DIST_DIFF || i == data->size - 1) {
            if (quantPts < _MIN_PTS_2OBJ) {
                for (int k = 0; k < quantPts; k++) {
                    data->pts[i - 1 - k].objID = -1;
                }

            } else {  // detect object successfully
                cout << "defined object " << objID << endl;

                mx = sumx / quantPts;
                my = sumy / quantPts;

                num = mxy - sumx * my;
                den = mxx - sumx * mx;

                float a = num / den;
                float b = my - a * mx;

                lineRep->lines[objID].y_start = a * minX + b;
                lineRep->lines[objID].y_end = a * maxX + b;
                lineRep->lines[objID].x_start = minX;
                lineRep->lines[objID].x_end = maxX;

                // cout << "a: " << a << " b: " << b << " minX: " << minX << " maxX: " << maxX << " objID" << objID << endl;

                lineRep->lines[objID].a = a;
                lineRep->lines[objID].b = b;
                lineRep->lines[objID].mx = mx;
                lineRep->lines[objID].my = my;
                lineRep->lines[objID].real_my = (my - _HEIGHT / 2.0) / -scalex;  // converts back to mm
                lineRep->lines[objID].minX = minX;
                lineRep->lines[objID].maxX = maxX;
                lineRep->lines[objID].minY = minY;
                lineRep->lines[objID].maxY = maxY;
                lineRep->lines[objID].objID = objID;  // last object
                lineRep->lines[objID].niceLine = niceLine;

                objID += 1;
            }

            // reset mmq values
            num = 0.f;
            den = 0.f;
            mx = 0.f;
            my = 0.f;
            sumx = 0.f;
            sumy = 0.f;
            mxx = 0.f;
            mxy = 0.f;
            maxX = data->pts[i].x_img;
            minX = data->pts[i].x_img;
            maxY = data->pts[i].y_img;
            minY = data->pts[i].y_img;

            quantPts = 0;
        }

        x = data->pts[i].x_img;
        y = data->pts[i].y_img;
        // cout << "x: " << x << " y: " << y << endl;

        minX = x < minX ? x : minX;
        maxX = x > maxX ? x : maxX;

        minY = y < minY ? y : minY;
        maxY = y > maxY ? y : maxY;

        sumx += x;
        sumy += y;

        mxx += x * x;
        mxy += x * y;

        data->pts[i].objID = objID;
        dist = data->pts[i].dist;
        quantPts += 1;
        niceLine = false;
    }

    data->nIDs = objID;
    lineRep->size = objID;
    cout << "Number of objects: " << data->nIDs << endl;
    cout << "Number of points: " << data->size << endl;
}
