#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "fujitsu_algo.h"
#include "stego_exception.hpp"

using namespace std;
using namespace cv;

void RGBtoCMYCtoRGB_test()
{
    Fujitsu_algo f;

    Mat pure_image = imread("lena.png", IMREAD_COLOR);
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
    cout << "Simple difference between images before and after transform is " << sum;

    waitKey(0);
}

void Stego_test(){
    Fujitsu_algo f;

    Mat pure_image = imread("lena.png", IMREAD_COLOR);
    namedWindow( "Pure image", WINDOW_AUTOSIZE );
    imshow( "Pure image", pure_image );

    Mat qr = imread("qr.png", IMREAD_GRAYSCALE);
    namedWindow( "qr", WINDOW_AUTOSIZE );
    imshow( "qr", qr );

    waitKey(0);

    Mat container = f.HideQR(pure_image, qr);
    namedWindow( "container", WINDOW_AUTOSIZE );
    imshow( "container", container );

    Mat diff_image;
    absdiff(container, pure_image, diff_image);
    namedWindow( "Diff image", WINDOW_AUTOSIZE );
    imshow( "Diff image", diff_image );
    MatIterator_<Vec3f> it, end;
    long sum = 0;
    for (it = diff_image.begin<Vec3f>(), end = diff_image.end<Vec3f>(); it != end; it++){
        sum += (*it)[2] + (*it)[1] + (*it)[0];
    }
    cout << "Simple difference between images before and after hiding is " << sum;

    waitKey(0);

    Mat qr_from_container = f.FindQR(container, f.getBlockW(), f.getBlockH());
    namedWindow( "qr from container", WINDOW_AUTOSIZE );
    imshow( "qr from container", qr_from_container );

    waitKey(0);
}

int main()
{
    Stego_test();
}
