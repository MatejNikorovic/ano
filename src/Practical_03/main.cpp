#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace cv;
using namespace std;

void drawHistogram(Mat image, string windowName, int scale = 1){
    vector<int> histogram;
    histogram.resize(256, 0);

    // count histogram
    for(int y=0; y<image.rows; y++){
        Vec3b* row = image.ptr<Vec3b>(y);
        for(int x=0; x<image.cols; x++){
            histogram[row[x][0]]++;
        }
    }

    // find max value
    int max = 0;
    for(unsigned i=0; i<histogram.size(); i++){
        if(histogram[i] > max){
            max = histogram[i];
        }
    }

    // draw histogram
    int size = 256 * scale;
    Mat imgHistogram(Size(size, size), CV_8UC3, Scalar::all(0));
    for(unsigned i=0; i<histogram.size(); i++){
        rectangle(imgHistogram,
                  Point(i * scale, size - 1),
                  Point(i * scale + (scale - 1), (size - 1) - (size - 1) * histogram[i] / max),
                  Scalar(0, 0, 255),
                  -1
        );
    }

    // show histogram
    imshow(windowName, imgHistogram);
}

void pepperAndSalt(Mat image, int count){
    srand(time(NULL));

    for(int i=0; i<count; i++){
        int x = rand() % image.cols;
        int y = rand() % image.rows;
        image.ptr<Vec3b>(y)[x] = (rand() % 2) ? Vec3b(0, 0, 0) : Vec3b(255, 255, 255);
    }
}

int main(){
    Mat image = imread("../Lenna.png");

    Mat whiteNoise = Mat(image.size(), CV_8UC3, Scalar::all(0));
    randu(whiteNoise, 0, 32);
    // drawHistogram(whiteNoise, 3);

    Mat gaussNoise = Mat(image.size(), CV_8UC3, Scalar::all(0));
    randn(gaussNoise, 16, 8);
    // drawHistogram(gaussNoise, 3);

    imshow("uniform", image);
    moveWindow("uniform", 0, 0);
    imshow("normal", image);
    moveWindow("normal", 600, 0);
    imshow("pepper", image);
    moveWindow("pepper", 1200, 0);

    waitKey();

    Mat noiseImage1 = image + whiteNoise - 16;
    Mat noiseImage2 = image + gaussNoise - 16;
    Mat noiseImage3;
    image.copyTo(noiseImage3);
    pepperAndSalt(noiseImage3, 10000);
    imshow("uniform", noiseImage1);
    imshow("normal", noiseImage2);
    imshow("pepper", noiseImage3);
    drawHistogram(whiteNoise, "white", 2);
    moveWindow("white", 0, 700);
    drawHistogram(gaussNoise, "gauss", 2);
    moveWindow("gauss", 600, 700);
    waitKey();

    Mat result1, result2, result3;
    blur(noiseImage1, result1, Size(3, 3));
    blur(noiseImage2, result2, Size(3, 3));
    medianBlur(noiseImage3, result3, 3);
    imshow("uniform", result1);
    imshow("normal", result2);
    imshow("pepper", result3);
    waitKey();

    return 0;
}
