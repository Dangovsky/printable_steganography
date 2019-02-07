#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "fujitsu_algo.h"
#include "stego_exception.hpp"

using namespace std;
using namespace cv;

int RGBtoCMYCtoRGB_test()
{
    Fujitsu_algo f;

    Mat pure_image = imread("lena.png", cv::IMREAD_COLOR);
    namedWindow( "Pure image", WINDOW_AUTOSIZE );
    imshow( "Pure image", pure_image );

    Mat float_image = Mat(pure_image.rows, pure_image.cols, CV_32FC3);
    pure_image.convertTo(float_image, CV_32FC3, 1/255.0);
    namedWindow( "Float image", WINDOW_AUTOSIZE );
    imshow( "Float image", float_image );

    Mat test_image = Mat(pure_image.rows, pure_image.cols, CV_8UC3);
    (f.CMYKtoRGB(f.RGBtoCMYK(float_image))).convertTo(test_image, CV_8UC3, 255.0);
    namedWindow( "Test image", WINDOW_AUTOSIZE );
    imshow( "Test image", test_image );

    Mat diff_image = pure_image - test_image;
    //namedWindow( "Diff image", WINDOW_AUTOSIZE );
    //imshow( "Diff image", diff_image );
    MatIterator_<Vec3f> it, end;
    long sum = 0;
    for (it = diff_image.begin<Vec3f>(), end = diff_image.end<Vec3f>(); it != end; it++){
        sum += (*it)[2] + (*it)[1] + (*it)[0];
    }
    cout << sum;

    waitKey(0);
}

int main()
{
    RGBtoCMYCtoRGB_test();
}
