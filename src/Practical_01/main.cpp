#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

int main(){
    Mat image = imread("../Lenna.png");
    imshow("image", image);
    waitKey();
    return 0;
}
