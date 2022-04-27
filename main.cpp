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

#include "Connection_information.h"
#include "Urg_driver.h"
#include "math_utilities.h"

#define _CAP_TIMES 30000
#define _HEIGHT 800
#define _WIDTH 800

const float scalex = _WIDTH / 4000.0;
const float scaley = _HEIGHT / 4000.0;

using namespace qrk;
using namespace std;

struct points {
    int x, y;
};

struct points_info {
    int xmin, xmax, ymin, ymax;
};

void updateScreen(string screen_name, cv::Mat& points, vector<struct points>& points_mm) {
    int centerx = 0;
    int centery = _HEIGHT / 2;

    points.setTo(0);

    // cout << "scalex: " << scalex << endl;
    // cout << "scaley: " << scaley << endl;

    for (int i = 0; i < points_mm.size(); i++) {
        int x = points_mm[i].x * scalex + centerx;
        int y = -points_mm[i].y * scaley + centery;
        // cout << "x: " << x << ", y: " << y << endl;
        // cout << "points_mm[i].x: " << points_mm[i].x << ", points_mm[i].y: " << points_mm[i].y << endl;
        // points.at<uchar>(x, y) = 255;
        cv::circle(points, cv::Point(x, y), 4, cv::Scalar(255), cv::FILLED);
    }

    cv::imshow("points image", points);
    cv::waitKey(1);
}

void print_data(const Urg_driver& urg, const vector<long>& data, long time_stamp, cv::Mat& points) {
    // Prints the X-Y coordinates for all the measurement points
    long min_distance = urg.min_distance();
    long max_distance = urg.max_distance();
    size_t data_n = data.size();

    vector<struct points> points_mm;

    points_mm.resize(data_n);

    for (size_t i = 0; i < data_n; ++i) {
        long l = data[i];
        if ((l <= min_distance) || (l >= max_distance)) {
            continue;
        }

        double radian = urg.index2rad(i);
        long x = static_cast<long>(l * cos(radian));
        long y = static_cast<long>(l * sin(radian));
        // cout << "(" << x << ", " << y << ")" << endl;

        points_mm[i].x = x;
        points_mm[i].y = y;
    }
    // cout << endl;
    updateScreen("points image", points, points_mm);
}

int main(int argc, char* argv[]) {
    cv::namedWindow("points image");
    cv::Mat points = cv::Mat(_HEIGHT, _WIDTH, CV_8UC1, cv::Scalar(0));

    Urg_driver urg;
    int ret;
    long* length_data;
    int length_data_size;

    Connection_information information(argc, argv);

    // Connects to the sensor
    if (!urg.open(information.device_or_ip_name(),
                  information.baudrate_or_port_number(),
                  information.connection_type())) {
        cout << "Urg_driver::open(): "
             << information.device_or_ip_name() << ": " << urg.what() << endl;
        return 1;
    }

    // config scan parameters
    int first_step = urg.deg2step(-90);
    int last_step = urg.deg2step(+90);
    int skip_step = 0;

    urg.set_scanning_parameter(first_step, last_step, skip_step);

    urg.start_measurement(Urg_driver::Distance, Urg_driver::Infinity_times, 0);
    for (int i = 0; i < _CAP_TIMES; ++i) {
        vector<long> data;
        long time_stamp = 0;

        if (!urg.get_distance(data, &time_stamp)) {
            cout << "Urg_driver::get_distance(): " << urg.what() << endl;
            return 1;
        }
        print_data(urg, data, time_stamp, points);
    }
}
