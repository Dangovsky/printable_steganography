#ifndef FUJITSU_ALGO_H
#define FUJITSU_ALGO_H

#include "stego_exception.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv/cv.h>

using namespace cv;

class Fujitsu_algo
{
    Mat container;
    Mat qr;
    int block_h;
    int block_w;
public:
    Fujitsu_algo();

    int getBlockH();
    int getBlockW();

    Mat HideQR(Mat container, Mat qr, int min_block_size = 2);
    Mat FindQR(Mat container, int block_w, int block_h);

    Mat RGBtoCMYK(Mat image);
    Mat CMYKtoRGB(Mat image);
private:
    uchar MeanYellow(Point block);
    void SwapYellow(Point block);
};

#endif // FUJITSU_ALGO_H
