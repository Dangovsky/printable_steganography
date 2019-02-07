#ifndef FUJITSU_ALGO_H
#define FUJITSU_ALGO_H

#include "stego_exception.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv/cv.h>

using namespace cv;

class Fujitsu_algo
{
public:
    Fujitsu_algo();

    Mat HideQR(Mat container, Mat qr);
    Mat FindQR(Mat container);

    Mat RGBtoCMYK(Mat image);
    Mat CMYKtoRGB(Mat image);
private:
};

#endif // FUJITSU_ALGO_H
