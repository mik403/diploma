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

	void makeSegmentation(vector<Point2f> &points_to_segment);
	auto_ptr<Mat> getCenters() { return res_centers; }
	auto_ptr<Mat> getLabels() { return res_labels; }
private:
		const int clusterCount = 10;
		
		auto_ptr<Mat> res_centers;
		auto_ptr<Mat> res_labels;
};