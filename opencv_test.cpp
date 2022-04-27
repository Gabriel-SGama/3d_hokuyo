#include <fstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
//  g++ -O2 -I /usr/local/include/urg_cpp opencv_test.cpp Connection_information.o /usr/local/lib/liburg_cpp.a -o opencv_test `pkg-config --cflags --libs opencv4`

struct points {
    int x, y;
};

int main(int argc, char** argv) {
    vector<string> lines;
    string line;
    ifstream inFile;

    char lixo;
    int number;

    int xmax = -10000;
    int ymax = -10000;
    int xmin = 10000;
    int ymin = 10000;

    int height = 800;
    int width = 800;

    inFile.open("log.txt");
    if (!inFile) {
        cerr << "Unable to open file datafile.txt";
        exit(1);  // call system to stop
    }

    vector<struct points> points_mm;
    points_mm.resize(4611);  // test purposes

    int i = 0;

    while (inFile >> lixo) {
        // lines.push_back(line);
        inFile >> number;
        points_mm[i].x = number;
        inFile >> lixo;
        inFile >> number;
        points_mm[i].y = number;
        inFile >> lixo;

        if (points_mm[i].x > xmax) {
            xmax = points_mm[i].x;
        }

        if (points_mm[i].y > ymax) {
            ymax = points_mm[i].y;
        }

        if (points_mm[i].x < xmin) {
            xmin = points_mm[i].x;
        }

        if (points_mm[i].y < ymin) {
            ymin = points_mm[i].y;
        }
        i++;
    }

    inFile.close();
    cout << "xmax: " << xmax << " xmin: " << xmin << endl;
    cout << "ymax: " << ymax << " ymin: " << ymin << endl;

    float scalex = (float)width / (xmax - xmin);
    float scaley = (float)height / (ymax - ymin);

    cout << "scalex: " << scalex << endl;
    cout << "scaley: " << scaley << endl;

    int centerx = 0;
    int centery = height / 2;

    cv::namedWindow("points image");
    cv::Mat points = cv::Mat(height, width, CV_8UC1, cv::Scalar(0));

    for (int i = 0; i < points_mm.size(); i++) {
        int x = points_mm[i].x * scalex + centerx;
        int y = -points_mm[i].y * scaley + centery;
        cout << "x: " << x << ", y: " << y << endl;
        cout << "points_mm[i].x: " << points_mm[i].x << ", points_mm[i].y: " << points_mm[i].y << endl;
        // points.at<uchar>(x, y) = 255;
        cv::circle(points, cv::Point(x, y), 4, cv::Scalar(255), cv::FILLED);
    }

    cv::imshow("points image", points);
    cv::waitKey(0);

    return 0;
}
