#include "headers/History.hpp"

using namespace std;
using namespace cv;

History::History() {
    last_x = 0;
    last_y = 0;

    trajectory = new Trajectory();

    trajectory->maxx = 0;
    trajectory->maxy = 0;

    trajectory->minx = 0;
    trajectory->miny = 0;
}

History::~History() {
}

vector<int> History::matching(LineRep* prevLines, LineRep* currLines) {
    vector<int> matchesIdx(prevLines->size, -1);

    if (prevLines->size == 0)  // first matching
        return matchesIdx;

    for (int i = 0; i < prevLines->size; i++) {
        if (!prevLines->lines[i].niceLine)
            continue;

        float bestDist = 100000.0;
        float bestIdx = -1;

        int prevLinecx = prevLines->lines[i].mx;
        int prevLinecy = prevLines->lines[i].my;

        float prev_a = prevLines->lines[i].a;
        float angle_diff = 0;

        for (int j = 0; j < currLines->size; j++) {
            if (!currLines->lines[j].niceLine)
                continue;

            int currLinecx = currLines->lines[j].mx;
            int currLinecy = currLines->lines[j].my;

            float curr_a = currLines->lines[j].a;

            float diffc = pow(currLinecx - prevLinecx, 2) + pow(prevLinecy - currLinecy, 2);  // dist between centers

            // cout << "i: " << i << " | j: " << j << " | diffc: " << diffc << " | angle_diff: " << angle_diff << endl;

            if (diffc < bestDist) {
                bestDist = diffc;
                bestIdx = j;
                angle_diff = abs(prev_a - curr_a);
            }
        }

        if (bestDist < _MATCHING_CENTER_DIST) {
            // cout << "bestDist: " << bestDist;
            matchesIdx[i] = bestIdx;
        }
        cout << endl;
    }

    for (int i = 0; i < matchesIdx.size(); i++)
        std::cout << matchesIdx.at(i) << ' ';

    return matchesIdx;
}

void History::updateTrajectory(LineRep* prevLines, LineRep* currLines, vector<int> matchesIdx) {
    int nmatches = 0;
    dist_changex = 0;
    dist_changey = 0;

    for (int i = 0; i < matchesIdx.size(); i++) {
        if (matchesIdx[i] == -1)  // no match;
            continue;

        dist_changex += currLines->lines[matchesIdx[i]].mx - prevLines->lines[i].mx;
        dist_changey += currLines->lines[matchesIdx[i]].real_my - prevLines->lines[i].real_my;

        nmatches += 1;
    }

    if (nmatches > 0) {
        dist_changex /= nmatches;
        dist_changey /= nmatches;
    }

    last_x += dist_changex;
    last_y += dist_changey;

    trajectory->maxx = last_x > trajectory->maxx ? last_x : trajectory->maxx;
    trajectory->maxy = last_y > trajectory->maxy ? last_y : trajectory->maxy;
    trajectory->minx = last_x < trajectory->minx ? last_x : trajectory->minx;
    trajectory->miny = last_y < trajectory->miny ? last_y : trajectory->miny;

    // cout << "dist x: " << dist_changex << " | y: " << dist_changey << endl;
    // cout << "last_x: " << last_x << " | y: " << last_y << endl;
    trajectory->traj_list.push_back(cv::Point2f(last_x, last_y));
}

void History::updateVision3d(LineRep* prevLines, LineRep* currLines, vector<int> matchesIdx) {
    const float scalex3dVis = scaley;
    const float scaley3dVis = _HEIGHT / (_MAX_DIST * sin(hkAngle));

    list<objects3d>::iterator it_sort;

    if (objs.empty()) {  // adds current Lines as objs
        for (int i = 0; i < currLines->size; i++) {
            if (!currLines->lines[i].niceLine) continue;

            objects3d currObj;

            currObj.avg_angle = currLines->lines[i].a;
            currObj.lastObjID = currLines->lines[i].objID;
            currObj.mx = currLines->lines[i].mx;
            currObj.my = currLines->lines[i].my;
            currObj.real_my = currLines->lines[i].real_my;
            currObj.start_my = currObj.real_my;
            currObj.resetID = false;
            currObj.avg_width = (currLines->lines[i].maxX - currLines->lines[i].minX);

            int y = currObj.real_my * scaley3dVis;

            y *= sin(hkAngle);

            currObj.tleft = Point(currObj.mx - currObj.avg_width / 2, _HEIGHT - y);
            currObj.bright = Point(currObj.mx + currObj.avg_width / 2, _HEIGHT - y - 2);

            // objs.push_back(currObj);

            // insert sorted for 3d plox
            for (it_sort = objs.begin(); it_sort != objs.end() & it_sort->real_my > currObj.real_my; it_sort++)
                ;

            objs.insert(it_sort, currObj);
        }

        return;
    }

    list<objects3d>::iterator it;

    for (it = objs.begin(); it != objs.end(); ++it)
        it->resetID = true;

    bool isObj;

    for (int i = 0; i < matchesIdx.size(); i++) {
        if (matchesIdx[i] == -1) continue;

        isObj = false;

        for (it = objs.begin(); it != objs.end(); ++it) {
            if (it->lastObjID != i || !it->resetID) continue;

            isObj = true;
            it->resetID = false;

            const int currIdx = matchesIdx[i];

            it->lastObjID = currLines->lines[currIdx].objID;

            it->mx = (it->mx - dist_changex + currLines->lines[currIdx].mx) / 2.0;

            float curr_widht = (currLines->lines[currIdx].maxX - currLines->lines[currIdx].minX) * (it->start_my / currLines->lines[currIdx].real_my);

            it->avg_width = 0.9 * it->avg_width + 0.1 * curr_widht;

            it->real_my = (it->real_my - dist_changey + currLines->lines[currIdx].real_my) / 2.0;
            int y = it->real_my * scaley3dVis;

            float height = _HEIGHT - y * sin(hkAngle);

            it->tleft.y = (it->tleft.y > height) ? it->tleft.y : height;
            it->bright.y = (it->bright.y < height) ? it->bright.y : height;

            it->tleft = Point(it->mx - it->avg_width / 2, it->tleft.y);
            it->bright = Point(it->mx + it->avg_width / 2, it->bright.y);
            break;
        }

        if (isObj) continue;

        // adds new object
        objects3d currObj;
        const int currIdx = matchesIdx[i];

        currObj.avg_angle = currLines->lines[currIdx].a;
        currObj.lastObjID = currLines->lines[currIdx].objID;
        currObj.mx = currLines->lines[currIdx].mx;
        currObj.my = currLines->lines[currIdx].my;
        currObj.real_my = currLines->lines[currIdx].real_my;
        currObj.start_my = currObj.real_my;

        currObj.resetID = false;

        currObj.avg_width = currLines->lines[currIdx].maxX - currLines->lines[currIdx].minX;

        int y = currObj.real_my * scaley3dVis;

        y *= sin(hkAngle);

        currObj.tleft = Point(currObj.mx - currObj.avg_width / 2, _HEIGHT - y);
        currObj.bright = Point(currObj.mx + currObj.avg_width / 2, _HEIGHT - y - 2);

        for (it_sort = objs.begin(); it_sort != objs.end() & it_sort->real_my > currObj.real_my; it_sort++)
            ;

        objs.insert(it_sort, currObj);
    }

    // updates position for non matched objects
    // based on the assumption that they dont end before the ground
    for (it = objs.begin(); it != objs.end(); ++it) {
        if (it->resetID) {
            it->lastObjID = -1;

            it->mx -= -dist_changex;

            it->real_my += dist_changey;

            int y = it->real_my * scaley3dVis;

            float height = _HEIGHT - y * sin(hkAngle);

            it->tleft.y = (it->tleft.y > height) ? it->tleft.y : height;
            it->bright.y = (it->bright.y < height) ? it->bright.y : height;

            it->tleft = Point(it->mx - it->avg_width / 2, it->tleft.y);
            it->bright = Point(it->mx + it->avg_width / 2, it->bright.y);
        }

        it->resetID = true;
    }
}