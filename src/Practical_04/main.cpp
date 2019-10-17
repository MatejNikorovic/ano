#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

static Mat fourierComplex;

Mat computeFourierTransform(Mat& image){
   Mat paddedImage;
   int newWidth = getOptimalDFTSize(image.cols);
   int newHeight = getOptimalDFTSize(image.rows);
   copyMakeBorder(image, paddedImage, 0, newHeight - image.rows, 0, newWidth - image.cols, BORDER_CONSTANT, Scalar::all(0));
   Mat parts[] = {Mat_<float>(paddedImage), Mat(paddedImage.size(), CV_32F, Scalar::all(0))};
   Mat output;
   merge(parts, 2, output);
   dft(output, output, DFT_COMPLEX_OUTPUT);
   return output;
}

void shiftQuadrants(Mat& image){
    int cx = image.cols / 2;
    int cy = image.rows / 2;

    Mat q0(image, Rect(0, 0, cx, cy));
    Mat q1(image, Rect(cx, 0, cx, cy));
    Mat q2(image, Rect(0, cy, cx, cy));
    Mat q3(image, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

Mat complexToLogScale(Mat& complex){
    Mat parts[2];
    split(complex, parts);
    Mat distances;
    magnitude(parts[0], parts[1], distances);
    distances += Scalar::all(1);
    log(distances, distances);
    normalize(distances, distances, 1, 0, NORM_INF);
    shiftQuadrants(distances);
    return distances;
}

Mat computeInverseFourierTransform(Mat& complex){
    Mat result, output;
    idft(complex, result);
    Mat parts[2];
    split(result, parts);
    magnitude(parts[0], parts[1], output);
    normalize(output, output, 0, 1, NORM_MINMAX);
    return output;
}

static void onMouse(int event, int x, int y, int flags, void* userData){
    if (event != EVENT_LBUTTONDOWN) return;

    // mask creation
    Mat mask(fourierComplex.size(), CV_32FC2, Scalar::all(1));
    circle(mask, Point(x, y), 4, Scalar::all(0), -1);
    circle(mask, Point(fourierComplex.cols - 1 - x, fourierComplex.rows - 1 - y), 4, Scalar::all(0), -1);
    shiftQuadrants(mask);

    // filtering
    Mat result;
    mulSpectrums(fourierComplex, mask, result, DFT_ROWS);
    fourierComplex = result;

    // vizualization
    Mat fourierLogSpace = complexToLogScale(fourierComplex);
    imshow("fourier", fourierLogSpace);

    Mat image = computeInverseFourierTransform(fourierComplex);
    imshow("filtered", image);
}

int main(){
    Mat image = imread("../fourier.png", IMREAD_GRAYSCALE);
    imshow("image", image);

    fourierComplex = computeFourierTransform(image);
    Mat fourierLogSpace = complexToLogScale(fourierComplex);
    imshow("fourier", fourierLogSpace);
    setMouseCallback("fourier", onMouse);

    waitKey();
    return 0;
}
