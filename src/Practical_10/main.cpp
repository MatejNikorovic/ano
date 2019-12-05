#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;
using namespace cv;

Point getPoint(Vec3i& model, int x){
    return Point(x, -(model[0]*x + model[2]) / model[1]);
}

void drawModel(Mat& image, Vec3i& model){
    line(image, getPoint(model, 0), getPoint(model, image.cols), Scalar(0, 0, 255));
}

Vec3i ransac(vector<Point>& samples, double epsilon, int iterations){
    Vec3i bestModel;
    int bestCount = 0;

    for(int j=0; j<iterations; j++){
        // choose minimal samples
        int index1 = rand() % (int)(samples.size());
        int index2 = rand() % (int)(samples.size());
        Point p1 = samples[index1];
        Point p2 = samples[index2];
        if(index1 == index2) continue;

        // compute model
        Point direction = p2 - p1;
        Point normal = Point(-direction.y, direction.x);
        int c = -normal.dot(p1); // -(normal.x * p1.x + normal.y * p1.y);
        Vec3i model = Vec3i(normal.x, normal.y, c);

        // count surroundings
        double normalLength = sqrt(normal.dot(normal));
        int count = 0;
        for(unsigned i=0; i<samples.size(); i++){
            Point p = samples[i];
            double distance = abs(model[0]*p.x + model[1]*p.y + model[2]) / normalLength;
            if(distance < epsilon){
                count++;
            }
        }

        // choose better model
        if(count > bestCount){
            bestCount = count;
            bestModel = model;
        }
    }

    return bestModel;
}

int main(){
    srand(time(NULL));

    vector<Point> points;
    Mat image(Size(1200, 800), CV_8UC3, Scalar::all(0));

    // random noise
    for(int i=0; i<10000; i++){
        int x = rand() % image.cols;
        int y = rand() % image.rows;
        points.push_back(Point(x, y));
    }

    // random model
    Vec3i randomModel = Vec3i(1, -1, 0);
    for(int i=0; i<500; i++){
        int x = rand() % image.cols;
        Point p = getPoint(randomModel, x) + Point(rand() % 11 - 5, rand() % 11 - 5);
        points.push_back(p);
    }

    random_shuffle(points.begin(), points.end());

    // visualize points
    for(unsigned i=0; i<points.size(); i++){
        line(image, points[i], points[i], Scalar::all(255));
    }

    Vec3i model = ransac(points, 10.0, 1000);
    drawModel(image, model);

    imshow("image", image);
    waitKey();
    return 0;
}
