#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

Mat skeleton(Mat& image, Mat& element){
    Mat input;
    image.copyTo(input);
    Mat result(image.size(), CV_8U, Scalar::all(0));

    while(countNonZero(input)){
        Mat eroded, opened;
        erode(input, eroded, element);
        morphologyEx(eroded, opened, MORPH_OPEN, element);
        result |= eroded - opened;

        input = eroded;
    }

    return result;
}

void showImage(Mat image){
    imshow("image", image);
    waitKey();
}

int main(){
    Mat image = imread("../shapes.jpg", IMREAD_GRAYSCALE);
    Mat input = image < 230;
    showImage(input);

    // morphological dilate
    Mat result;
    Mat element = getStructuringElement(MORPH_CROSS, Size(3,3));
    dilate(input, result, element);
    showImage(result);

    // morphological erode
    erode(input, result, element);
    showImage(result);

    // morphological open
    erode(input, result, element);
    dilate(result, result, element);
    showImage(result);

    // morphological close
    dilate(input, result, element);
    erode(result, result, element);
    showImage(result);

    // inner objects boundary
    erode(input, result, element);
    showImage(input - result);

    // morphology gradient
    morphologyEx(input, result, MORPH_GRADIENT, element);
    showImage(result);

    // morphology skeleton
    result = skeleton(input, element);
    showImage(result);

    return 0;
}
