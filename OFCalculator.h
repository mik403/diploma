
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

#define MAX_COUNT 500

class OFCalculator {
public:

	OFCalculator() : ready_to_process(false) {};
	
	void init();
	void calcFlow(Mat &rgbFrames);
	void draw();

	void filterResults();

	vector<Point2f> &getResult() {
		return filtered_points;
	}

private:

	bool ready_to_process;


	Mat frame, grayFrames, prevGrayFrame; 

	bool needToInit;

	vector<Point2f> points1;
	vector<Point2f> points2;
	vector<Point2f> filtered_points;

	vector<uchar> status;
	vector<float> err;

	Size winSize;

};