#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace cv;
using namespace std;


class Segmentator {
public:

	auto_ptr<Mat> makeSegmentation(vector<Point2f> &points_to_segment);

private:
		const int clusterCount = 10;
};