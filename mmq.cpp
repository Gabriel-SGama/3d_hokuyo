/*!
  \example get_distance.cpp Obtains distance data
  \author Satofumi KAMIMURA

  $Id$
*/

//  g++ mmq.cpp -o main `pkg-config --cflags --libs opencv4`

#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define _HEIGHT 800
#define _WIDTH 800

// const float scalex = _WIDTH / 4000.0;
// const float scaley = _HEIGHT / 4000.0;

using namespace std;
using namespace cv;

typedef struct Points {
    cv::Point2d pt;
} Points;

int main(int argc, char* argv[]) {
    Mat image = Mat(_HEIGHT, _WIDTH, CV_8UC1, Scalar(0));

    vector<cv::Point2d> points;

    // points.push_back(cv::Point2d(100, 100));
    // points.push_back(cv::Point2d(200, 200));
    // points.push_back(cv::Point2d(300, 300));
    // points.push_back(cv::Point2d(400, 400));
    // points.push_back(cv::Point2d(500, 500));
    // points.push_back(cv::Point2d(650, 400));

    points.push_back(cv::Point2d(100, 100));
    points.push_back(cv::Point2d(101, 200));
    points.push_back(cv::Point2d(120, 300));
    points.push_back(cv::Point2d(100, 400));
    points.push_back(cv::Point2d(140, 500));
    points.push_back(cv::Point2d(150, 600));

    for (int i = 0; i < points.size(); i++) {
        circle(image, points[i], 1, Scalar(255), FILLED);
    }

    int n = points.size();

    float num, den, x, y, mx, my;
    num = 0.f;
    den = 0.f;
    mx = 0.f;
    my = 0.f;

    for (int i = 0; i < n; i++) {
        mx += points[i].x;
        my += points[i].y;
    }

    mx /= n;
    my /= n;

    cout << "media x: " << mx << endl;
    cout << "media y: " << my << endl;

    for (int i = 0; i < n; i++) {
        x = points[i].x;
        y = points[i].y;

        num += x * (y - my);
        den += x * (x - mx);
    }

    float a = num / den;
    float b = my - a * mx;

    cout << "a: " << a << endl;
    cout << "b: " << b << endl;

    float y_end = a * points[n - 1].x + b;

    line(image, cv::Point2d(0, b), cv::Point2d(points[n - 1].x, y_end), Scalar(125), 3);

    imshow("points", image);
    waitKey(0);
}
