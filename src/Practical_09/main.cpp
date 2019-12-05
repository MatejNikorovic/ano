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

static vector<vector<float> > samples;
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
    for(int i=0; i<histogram.size(); i++){
        if(histogram[i] > max) max = histogram[i];
    }

    for(int i=0; i<histogram.size(); i++){
        histogram[i] /= max;
    }

    return histogram;
}

void initSamples(string path, string name, string extension, int count){
    int classID = labels.size();
    labels.push_back(name);

    for(int i=0; i<count; i++){
        stringstream ss;
        ss << path << "/" << name << i << extension;
        cout << ss.str() << endl;
        Mat image = imread(ss.str());
        samples.push_back(calcHistogram(image));
        classes.push_back(classID);
    }
}


void initWindow(char* winName, int x, int y){
    namedWindow(winName);
    moveWindow(winName, x, y);
}

Mat getSamples(){
    Mat result(Size(256, samples.size()), CV_32F);

    for(int y=0; y<samples.size(); y++){
        float* row = result.ptr<float>(y);
        for(int x=0; x<256; x++){
            row[x] = samples[y][x];
        }
    }
    return result;
}

Mat getClasses(){
    return Mat(classes).t();
}

string predict(Mat& image, Ptr<KNearest>& classifier){
    Mat sample = Mat(calcHistogram(image)).t();
    Mat result;
    classifier->predict(sample, result);
    float response = (result.ptr<float>(0)[0]);
    cout << response << endl;
    int classID = (int)(response);
    return labels[classID];
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

    Mat image = imread("../recognition/snow3.jpg");

    int sideLength = (int)sqrt(image.cols * image.cols + image.rows * image.rows);
    Mat temp, temp2;
    int xBorder = (sideLength - image.cols) / 2;
    int yBorder = (sideLength - image.rows) / 2;
    copyMakeBorder(image, temp, yBorder, yBorder, xBorder, xBorder, BORDER_CONSTANT);

    Mat rotateMatrix = getRotationMatrix2D(Point2f(temp.cols / 2.f, temp.rows / 2.f), 30, 0.5);
    Mat rotated;
    warpAffine(temp, rotated, rotateMatrix, Size(sideLength, sideLength));
    //floodFill(rotated, temp2, Point(1, 1), Scalar::all(255));

    cout << predict(image, classifier) << endl;
    cout << predict(temp, classifier) << endl;
    cout << predict(rotated, classifier) << endl;

    initWindow("image", 600, 100);
    imshow("image", rotated);
    waitKey();
}
