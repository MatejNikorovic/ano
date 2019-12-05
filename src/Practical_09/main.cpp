#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace cv;
using namespace std;
using namespace cv::ml;

static vector<vector<float> > descriptors;
static vector<float> classes;
static vector<string> labels;

vector<float> calcHistogram(Mat& image){
    vector<float> histogram;
    histogram.resize(256, 0);

    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    for(int y=0; y<gray.rows; y++){
        uchar* row = gray.ptr<uchar>(y);
        for(int x=0; x<gray.cols; x++){
            histogram[row[x]]++;
        }
    }

    histogram[0] = 0.f;
    histogram[255] = 0.f;

    float max = 0;
    for(unsigned i=0; i<histogram.size(); i++){
        if(histogram[i] > max) max = histogram[i];
    }

    for(unsigned i=0; i<histogram.size(); i++){
        histogram[i] /= max;
    }

    return histogram;
}

void initSamples(string path, string name, string extension, int count){
    int classId = labels.size();
    labels.push_back(name);

    for(int i=0; i<count; i++){
        stringstream ss;
        ss << path << "/" << name << i << extension;
        cout << ss.str() << endl;
        Mat image = imread(ss.str());
        descriptors.push_back(calcHistogram(image));
        classes.push_back(classId);
    }
}


void initWindow(string winName, int x, int y){
    namedWindow(winName);
    moveWindow(winName, x, y);
}

Mat getSamples(){
    Mat result(Size(256, descriptors.size()), CV_32F);

    for(int y=0; y<descriptors.size(); y++){
        float* row = result.ptr<float>(y);
        for(int x=0; x<256; x++){
            row[x] = descriptors[y][x];
        }
    }
    return result;
}

Mat getClasses(){
    return Mat(classes).t();
}

void predict(Mat& image, Ptr<KNearest>& classifier){
    Mat sample = Mat(calcHistogram(image)).t();
    Mat result;
    classifier->predict(sample, result);
    float response = (result.ptr<float>(0)[0]);
    int classId = (int)(response);
    cout << labels[classId] << endl;
}

int main(int argc, char *argv[]){
    initSamples("../recognition", "person", ".jpg", 8);
    initSamples("../recognition", "nature", ".jpg", 8);
    initSamples("../recognition", "castle", ".jpg", 8);
    initSamples("../recognition", "snow", ".jpg", 8);

    Ptr<KNearest> classifier = KNearest::create();
    classifier->setDefaultK(1);

    classifier->train(getSamples(), ROW_SAMPLE, getClasses());
    cout << classifier->isTrained() << endl;

    Mat image = imread("../castle.jpg");

    Mat rotateMatrix = getRotationMatrix2D(Point2f(image.cols / 2.f, image.rows / 2.f), 30, 0.5);
    Mat rotated;
    warpAffine(image, rotated, rotateMatrix, image.size());

    predict(image, classifier);
    predict(rotated, classifier);

    initWindow("image", 600, 100);
    imshow("image", rotated);
    waitKey();
}
