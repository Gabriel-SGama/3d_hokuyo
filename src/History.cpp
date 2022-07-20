#include "headers/History.hpp"

using namespace std;

History::History(/* args */) {
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
            // float angle_diff = abs(prev_a - curr_a);

            cout << "i: " << i << " | j: " << j << " | diffc: " << diffc << " | angle_diff: " << angle_diff << endl;

            // TODO: better matching distance - test in other places
            if (diffc < bestDist) {
                bestDist = diffc;
                bestIdx = j;
                angle_diff = abs(prev_a - curr_a);
            }
        }

        if (bestDist < _MATCHING_CENTER_DIST) {
            cout << "bestDist: " << bestDist;
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
    float dist_changex = 0, dist_changey = 0;

    for (int i = 0; i < matchesIdx.size(); i++) {
        if (matchesIdx[i] == -1)  // no match;
            continue;

        dist_changex += currLines->lines[matchesIdx[i]].mx - prevLines->lines[i].mx;
        dist_changey += currLines->lines[matchesIdx[i]].my - prevLines->lines[i].my;

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

    cout << "dist x: " << dist_changex << " | y: " << dist_changey << endl;
    cout << "last_x: " << last_x << " | y: " << last_y << endl;
    trajectory->traj_list.push_back(cv::Point2f(last_x, last_y));
}
