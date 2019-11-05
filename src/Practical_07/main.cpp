#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

using namespace std;
using namespace cv;

static Point lPoint, rPoint;

static void onMouse(int event, int x, int y, int flags, void* userData){
    if(event != EVENT_LBUTTONDOWN && event != EVENT_RBUTTONDOWN) return;

    if(event == EVENT_LBUTTONDOWN) lPoint = Point(x, y);
    if(event == EVENT_RBUTTONDOWN) rPoint = Point(x, y);

    // input fields
    Mat image = *((Mat*)userData);
    Rect roi = Rect(lPoint, rPoint); // region of interest

    Mat visualization;
    if(roi.area() > 100){
        // graph cut segmentation
        Mat mask, foreground, background;
        grabCut(image, mask, roi, background, foreground, 1, GC_INIT_WITH_RECT);

        // masking
        Mat segmented = mask == GC_PR_FGD;
        image.copyTo(visualization, segmented);
    } else {
        image.copyTo(visualization);
    }

    // visualization
    rectangle(visualization, roi, Scalar(0, 0, 255), 3);
    imshow("visualization", visualization);
}

void circlesHT(Mat image){
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1, 100, 100, 100, 50, 220);
    for(unsigned i=0; i<circles.size(); i++){
        circle(image, Point((int)circles[i][0], (int)circles[i][1]), circles[i][2], Scalar(0, 0, 255), 2);
    }

    imshow("circles", image);
}

int main(){
    circlesHT(imread("../coins.jpg"));

    Mat image = imread("../castle.jpg");
    lPoint = Point(0, 0);
    rPoint = Point(0, 0);

    namedWindow("visualization");
    setMouseCallback("visualization", onMouse, &image);
    onMouse(EVENT_LBUTTONDOWN, 0, 0, 0, &image);

    waitKey();
    return 0;
}
