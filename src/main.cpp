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

    // hokuyo->setWrite("NAME_OF_FILE.dat", max_size);
    cout << "starting..." << endl;

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

        viewer->updateScreens(data, prevRep, currRep, matchesIdx, hist->getTraj(), hist->get3dobjs());

        *prevRep = *currRep;  // clone content
    }
}
