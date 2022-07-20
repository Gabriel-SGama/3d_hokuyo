#include "headers/Logic.hpp"

#include <cmath>

using namespace std;

Logic::Logic() {
    nID = 0;
    beginSize = 100;  // max size of lines in one frame
    lineRep = new LineRep();
    lineRep->lines.resize(beginSize);
    lineRep->size = 0;
}

Logic::~Logic() {
}

bool checkLineDiff(int maxX, int minX, int maxY, int minY, int a, int b, int x_img, int y_img, bool niceLine) {
    int diffmaxX = x_img - maxX;
    int diffminX = x_img - minX;
    int diffmaxY = y_img - maxY;
    int diffminY = y_img - minY;

    float errY = abs(y_img - a * x_img + b);

    // cout << "errY: " << errY << endl;
    // return ;
    return errY < 7000;
}

void Logic::reverseLine(Scan* data, int begin, int end, int& objID) {
    int x = data->pts[end].x_img;
    int y = data->pts[end].y_img;
    int dist = data->pts[end].dist;
    int quantPts = 1;
    int maxX = data->pts[end].x_img;
    int minX = data->pts[end].x_img;
    int maxY = data->pts[end].y_img;
    int minY = data->pts[end].y_img;

    float angle = data->pts[end].angle;
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

    for (int i = end; i > begin; i--) {
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
                ypred = data->pts[i + 1 + k].x_img * apred + bpred;
                yread = data->pts[i + 1 + k].y_img;
                resi += (ypred - yread) * (ypred - yread);
            }

            resi /= quantPts;

            if (resi > 20)
                niceLine = false;
            // float y_end = a * maxX + b;
        }

        if (abs(dist - data->pts[i].dist) > _MAX_DIST_DIFF || i == data->size - 1 || (linePred && !checkLineDiff(maxX, minX, maxY, minY, apred, bpred, x, y, niceLine))) {
            linePred = false;

            if (quantPts < _MIN_PTS_2OBJ) {
                for (int k = 0; k < quantPts; k++) {
                    data->pts[i + 1 + k].objID = -1;
                }

            } else {  // detect object successfully

                // if (objID > beginSize) { //do this the right way
                //     beginSize *= 2;
                //     line.reserve(beginSize);
                // }
                cout << "defined object " << objID << endl;

                mx = sumx / quantPts;
                my = sumy / quantPts;

                num = mxy - sumx * my;
                den = mxx - sumx * mx;

                float a = num / den;
                float b = my - a * mx;

                // float y_end = a * maxX + b;

                // cout << "a: " << a << " b: " << b << " minX: " << minX << " maxX: " << maxX << " objID" << objID << endl;

                lineRep->lines[objID].a = a;
                lineRep->lines[objID].b = b;
                lineRep->lines[objID].minX = minX;
                lineRep->lines[objID].maxX = maxX;
                lineRep->lines[objID].minY = minY;
                lineRep->lines[objID].maxY = maxY;
                lineRep->lines[objID].objID = objID;  // last object
                lineRep->lines[objID].niceLine = niceLine;
                // line(image, cv::Point2d(0, b), cv::Point2d(points[n - 1].x, y_end), Scalar(125), 3);

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

        if (abs(dist - data->pts[i].dist) > _MAX_DIST_DIFF || i == data->size - 1) {  //|| (linePred && !checkLineDiff(maxX, minX, maxY, minY, apred, bpred, x, y, niceLine))) {
            if (quantPts < _MIN_PTS_2OBJ) {
                for (int k = 0; k < quantPts; k++) {
                    data->pts[i - 1 - k].objID = -1;
                }

            } else {  // detect object successfully
                cout << "defined object " << objID << endl;
                // niceLine = checkLineDiff(maxX, minX, maxY, minY, apred, bpred, x, y, niceLine);

                // if (!niceLine) {
                //     ;
                //     // reverseLine(data, i - 1 - quantPts, i + quantPts, objID);
                // } else {
                mx = sumx / quantPts;
                my = sumy / quantPts;

                num = mxy - sumx * my;
                den = mxx - sumx * mx;

                float a = num / den;
                float b = my - a * mx;

                // float y_end = a * maxX + b;

                // cout << "a: " << a << " b: " << b << " minX: " << minX << " maxX: " << maxX << " objID" << objID << endl;

                lineRep->lines[objID].a = a;
                lineRep->lines[objID].b = b;
                lineRep->lines[objID].mx = mx;
                lineRep->lines[objID].my = my;
                lineRep->lines[objID].minX = minX;
                lineRep->lines[objID].maxX = maxX;
                lineRep->lines[objID].minY = minY;
                lineRep->lines[objID].maxY = maxY;
                lineRep->lines[objID].objID = objID;  // last object
                lineRep->lines[objID].niceLine = niceLine;
                // line(image, cv::Point2d(0, b), cv::Point2d(points[n - 1].x, y_end), Scalar(125), 3);

                objID += 1;

                // if (objID > beginSize) { //do this the right way
                //     beginSize *= 2;
                //     line.reserve(beginSize);
                // }
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

void Logic::descriptor(Scan* data) {
}

void Logic::matching(Scan* data) {
}

void Logic::predDist(Scan* data) {
}