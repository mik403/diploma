#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "OFCalculator.h"
#include "Segmentator.h"
#include "PrimPointTracker.h"
#include "ShiftTracker.h"

using namespace cv;

#define TRACKERS_N 10

class Engine {
public:
	Engine() : rawWindow("Raw Video"), init_tracker(false) {}

	void initCapture(std::string file_name);

	void update();
	void draw();
	void drawTrackers();
	void resetTrackers();
private:

	Mat rgbFrames;
	VideoCapture cap;
	
	OFCalculator flow;
	Segmentator seg;

	PrimPointTracker p_trackers[TRACKERS_N];

	string rawWindow;

	ShiftTracker test_tracker[TRACKERS_N];
	bool init_tracker;
};

