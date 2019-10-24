#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

static void onTrackbar(int value, void* data){
    Mat image = *((Mat*)data);
    Mat result;
    // Laplacian(image, result, CV_8U);
    result = image;
    imshow("Segmented", result > value);
}

int main(){
    Mat image = imread("../shapes.jpg", IMREAD_GRAYSCALE);

    // Optimal threshold value selection
    Mat temp;
    int thresh = (int) threshold(image, temp, 0.0, 255, THRESH_TRIANGLE);

    namedWindow("Segmented");
    createTrackbar("Threshold", "Segmented", &thresh, 255, onTrackbar, &image);
    onTrackbar(thresh, &image);

    waitKey();
    return 0;
}
