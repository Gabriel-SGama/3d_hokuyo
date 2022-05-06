#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Connection_information.h"
#include "Const.hpp"
#include "Urg_driver.h"
#include "math_utilities.h"

// #define HOKUYO_READER 0
// #define FILE_READER 1

// typedef struct points {
//     int x, y;
// } points;

// typedef struct Scan {
//     std::vector<points> pts;
//     int size, max_size;
// } Scan;

class Hokuyo {
   private:
    qrk::Urg_driver urg;
    qrk::Connection_information *info;
    int first_step, last_step, skip_step;
    int max_data_size;

    float angle_min, angle_max;

    std::string rfname, wfname;
    std::ifstream inFile;
    std::ofstream outFile;

    std::vector<long> dataTemp;

    void convertXY(Scan *data);

    int hokuyoReader(Scan *data, long &timeStamp);
    int read_file(Scan *data, long &timeStamp);
    bool mode;

   public:
    Hokuyo(int argc, char *argv[]);
    Hokuyo(std::string filename);
    ~Hokuyo();

    void print_info();

    void setWrite(std::string wfname, int max_data_size);
    void writeFile(Scan *data);
    inline int getMaxSize() {
        return max_data_size;
    };

    int getData(Scan *data, long &timeStamp);
};
