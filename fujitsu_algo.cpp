#include "fujitsu_algo.h"

Fujitsu_algo::Fujitsu_algo()
{

}

int Fujitsu_algo::getBlockH(){
    return block_h;
}

int Fujitsu_algo::getBlockW(){
    return block_w;
}

Mat Fujitsu_algo::HideQR(Mat container, Mat qr, int min_block_size)
{
    if (container.type() != 16){
        throw new Stego_exception("Incorrect container Mat type (should be CV_8UC3)");
    }
    if (qr.type() != 0){
        throw new Stego_exception("Incorrect Qr image Mat type (should be CV_8UC1)");
    }
    if (container.data == NULL){
        throw new Stego_exception("Container is empty");
    }
    if (qr.data == NULL){
        throw new Stego_exception("Qr is empty");
    }

    this->container = container;
    this->qr = qr;

    block_h = container.rows / qr.rows;
    block_w = container.cols / (qr.cols * 2);

    if (block_h < min_block_size || block_w < min_block_size){
        throw new Stego_exception("Qr image size too big for this container and minimum block size");
    }

    Point block = {0, 0};
    MatIterator_<uchar> it, end;
    for (it = qr.begin<uchar>(), end = qr.end<uchar>(); it != end; it++){
        if (block.x + (block_w * 2) > container.cols){
            block.y += block_h;
            block.x = 0;
            if (block.y + block_h > container.rows)
                throw new Stego_exception("Block pointer out of container");
        }
        if (MeanYellow(block) > MeanYellow(Point(block.x + block_w, block.y)) && *it > 10
                || MeanYellow(block) < MeanYellow(Point(block.x + block_w, block.y)) && *it < 10 ){
            SwapYellow(block);
        }
        block.x += (block_w * 2);
    }
    return container;
}

Mat Fujitsu_algo::FindQR(Mat container, int block_w, int block_h)
{
    if (container.type() != 16){
        throw new Stego_exception("Incorrect container Mat type (should be CV_8UC3)");
    }
    if (container.data == NULL){
        throw new Stego_exception("Container is empty");
    }

    this->container = container;
    this->qr = Mat::zeros(container.rows / block_h, container.cols / (block_w * 2), CV_8U);

    Point block = {0, 0};
    MatIterator_<uchar> it, end;
    for (it = qr.begin<uchar>(), end = qr.end<uchar>(); it != end; it++){
        if (block.x + (block_w * 2) > container.cols){
            block.y += block_h;
            block.x = 0;
            if (block.y + block_h > container.rows)
                throw new Stego_exception("Block pointer out of container");
        }
        if (MeanYellow(block) > MeanYellow(Point(block.x + block_w, block.y))){
            *it = 0;
        }
        block.x += (block_w * 2);
    }

    return qr;
}

uchar Fujitsu_algo::MeanYellow(Point block){
    unsigned int mean = 0;
    int cnt = 0;
    for (int y = block.y; y < block.y + block_h; y++){
        for (int x = block.x; x < block.x + block_w; x++){
            mean += container.at<Vec3b>(y,x)[0];
            cnt++;
        }
    }
    return static_cast<uchar>(mean / cnt);
}

void Fujitsu_algo::SwapYellow(Point block){
    for (int y = block.y; y < block.y + block_h; y++){
        for (int x = block.x; x < block.x + block_w; x++){
            swap(container.at<Vec3b>(y, x) , container.at<Vec3b>(y, x + block_w));
        }
    }
}

Mat Fujitsu_algo::RGBtoCMYK(Mat rgb_image)
{
    if (rgb_image.type() != 21){
        throw new Stego_exception("Incorrect Mat type (should be CV_32FC3)");
    }

    int w = rgb_image.cols;
    int h = rgb_image.rows;

    Mat cmyk_image;

    MatIterator_<Vec3f> rgb_it, rgb_end;
    MatIterator_<Vec4f> cmyk_it, cmyk_end;

    float k = 0, r, g, b;
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

    Mat rgb_image;

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
