/*!
  \example get_distance.cpp Obtains distance data
  \author Satofumi KAMIMURA

  $Id$
*/

//  g++ -O2 -I /usr/local/include/urg_cpp main.cpp Connection_information.o /usr/local/lib/liburg_cpp.a -o main `pkg-config --cflags --libs opencv4`

#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "Urg_driver.h"
#include "headers/Connection_information.h"
#include "headers/Const.hpp"
#include "headers/History.hpp"
#include "headers/Hokuyo.hpp"
#include "headers/Logic.hpp"
#include "headers/Viewer.hpp"
#include "math_utilities.h"

#define _CAP_TIMES 30000
// #define _HEIGHT 800
// #define _WIDTH 800

// const float scalex = _WIDTH / 4000.0;
// const float scaley = _HEIGHT / 4000.0;

using namespace qrk;
using namespace std;

int main(int argc, char* argv[]) {
    Hokuyo* hokuyo;
    Logic* logic = new Logic();
    Scan* data = new Scan();
    Viewer* viewer = new Viewer();
    History* hist = new History();

    long timeStamp = 0;
    int max_size;

    if (argc == 1) {
        hokuyo = new Hokuyo(argc, argv);
    } else if (argc == 2) {
        hokuyo = new Hokuyo(argv[1]);
    } else {
        cout << "Usage: exe filename.txt" << endl;
        return 1;
    }

    max_size = hokuyo->getMaxSize();
    data->pts.resize(max_size);

    // hokuyo->setWrite("wall_plane_v2.dat", max_size);
    cout << "starting..." << endl;
    // hokuyo->getData(data, timeStamp);  // clear zeros -> still zeros

    LineRep* prevRep = new LineRep();
    LineRep* currRep;

    vector<int> matchesIdx;

    Trajectory* trajectory;

    while (hokuyo->getData(data, timeStamp)) {
        // hokuyo->writeFile(data);
        // LOGIC
        logic->defineLimit(data);
        currRep = logic->getLineRep();
        matchesIdx = hist->matching(prevRep, currRep);

        // VISUALIZATION
        hist->updateTrajectory(prevRep, currRep, matchesIdx);
        hist->updateVision3d(prevRep, currRep, matchesIdx);

        viewer->updateScreens(data, prevRep, currRep, matchesIdx, hist->getTraj());

        *prevRep = *currRep;  // clone content
        cout << "scalex: " << scalex << " | scaley: " << scaley << endl;
    }
}
