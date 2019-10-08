#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

void drawHistogram(Mat image, int scale = 1){
    vector<int> histogram;
    histogram.resize(256, 0);

    // count histogram
    for(int y=0; y<image.rows; y++){
        uchar* row = image.ptr<uchar>(y);
        for(int x=0; x<image.cols; x++){
            histogram[row[x]]++;
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
    imshow("histogram", imgHistogram);
}

int main(){
    // load and show image
    Mat image = imread("../Lenna.png");
    imshow("image", image);

    // amend one pixel using safe access
    image.at<Vec3b>(100, 50) = Vec3b(0, 255, 0);
    imshow("amended", image);
    waitKey();

    // direct access to memory - image inverse
    for(int y=0; y<image.rows; y++){
        Vec3b* row = image.ptr<Vec3b>(y);
        for(int x=0; x<image.cols; x++){
            row[x] = Vec3b(255, 255, 255) - row[x];
        }
    }
    imshow("amended", image);
    waitKey();

    // convert to grayscale
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    imshow("amended", gray);

    // draw histogram
    drawHistogram(gray, 3);
    waitKey();

    return 0;
}
