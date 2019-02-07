#include "fujitsu_algo.h"

Fujitsu_algo::Fujitsu_algo()
{

}

Mat Fujitsu_algo::HideQR(Mat container, Mat qr)
{
    // accept only char type matrices
    //CV_Assert(container.depth() == CV_8U);

}

Mat Fujitsu_algo::FindQR(Mat container)
{

}

Mat Fujitsu_algo::RGBtoCMYK(Mat rgb_image)
{
    if (rgb_image.type() != 21){
        throw new Stego_exception("Incorrect Mat type (should be CV_32FC3)");
    }

    int w = rgb_image.cols;
    int h = rgb_image.rows;

    Mat cmyk_image = Mat::zeros(h, w, CV_32FC4);

    MatIterator_<Vec3f> rgb_it, rgb_end;
    MatIterator_<Vec4f> cmyk_it, cmyk_end;

    float k, r, g, b;
    for (rgb_it = rgb_image.begin<Vec3f>(), rgb_end = rgb_image.end<Vec3f>(), cmyk_it = cmyk_image.begin<Vec4f>(), cmyk_end = cmyk_image.end<Vec4f>();
         rgb_it != rgb_end; rgb_it++, cmyk_it++){
        r = (*rgb_it)[2];
        g = (*rgb_it)[1];
        b = (*rgb_it)[0];
        //k = std::min(std::min(1 - r, 1 - g), 1 - b);

        (*cmyk_it)[0] = (1.0 - r - k) / (1 - k);
        (*cmyk_it)[1] = (1.0 - g - k) / (1 - k);
        (*cmyk_it)[2] = (1.0 - b - k) / (1 - k);
        (*cmyk_it)[3] = k;
    }

    return cmyk_image;
}

Mat Fujitsu_algo::CMYKtoRGB(Mat cmyk_image)
{
    if (cmyk_image.type() != 29){
        throw new Stego_exception("Incorrect Mat type (should be CV_32FC4)");
    }

    int w = cmyk_image.cols;
    int h = cmyk_image.rows;

    Mat rgb_image = Mat::zeros(h, w, CV_32FC3);

    MatIterator_<Vec3f> rgb_it, rgb_end;
    MatIterator_<Vec4f> cmyk_it, cmyk_end;

    float k = 0;
    for (rgb_it = rgb_image.begin<Vec3f>(), rgb_end = rgb_image.end<Vec3f>(), cmyk_it = cmyk_image.begin<Vec4f>(), cmyk_end = cmyk_image.end<Vec4f>();
         cmyk_it != cmyk_end; rgb_it++, cmyk_it++){
        //k = (*cmyk_it)[3];

        (*rgb_it)[2] = 1.0 - (*cmyk_it)[0] * (1 - k) + k;
        (*rgb_it)[1] = 1.0 - (*cmyk_it)[1] * (1 - k) + k;
        (*rgb_it)[0] = 1.0 - (*cmyk_it)[2] * (1 - k) + k;
    }

    return rgb_image;
}
