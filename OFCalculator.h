
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

	OFCalculator() :rawWindow("Raw Video"), opticalFlowWindow("Optical Flow Window"), ready_to_process(false) {};
	void initCapture(std::string file_name);
	void update();
	void draw();

	vector<Point2f> &getResult() {
		return points1;
	}

private:

	bool ready_to_process;

	VideoCapture cap;

	string rawWindow;
	string opticalFlowWindow;

	Mat frame, grayFrames, rgbFrames, prevGrayFrame; 
	Mat opticalFlow;

	bool needToInit;

	vector<Point2f> points1;
	vector<Point2f> points2;
	vector<uchar> status;
	vector<float> err;

	Size winSize;

};