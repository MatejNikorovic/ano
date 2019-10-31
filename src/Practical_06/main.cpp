#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

void regionGrowing(Mat image){
    // convert to grayscale
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // threshloding
    Mat thresh = gray < 230;

    // region growing
    vector<vector<Point> > boudaries, smoothedBoudaries;
    findContours(thresh, boudaries, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    smoothedBoudaries.resize(boudaries.size());
    for(unsigned i=0; i<boudaries.size(); i++){
        approxPolyDP(boudaries[i], smoothedBoudaries[i], 5, true);
        drawContours(image, smoothedBoudaries, i, Scalar(0, 0, 255), 3);
        cout << "Countour #" << i << " boundary points: " << boudaries[i].size();
        cout << " smoothed boundary points: " << smoothedBoudaries[i].size() << endl;
    }

    imshow("Region Growing", image);
}

int main(){
    Mat image = imread("../shapes.jpg");
    regionGrowing(image);
    waitKey();
    return 0;
}
