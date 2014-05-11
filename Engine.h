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

using namespace cv;

class Engine {
public:
	Engine() : rawWindow("Raw Video") {}

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

	PrimPointTracker p_trackers[25];

	string rawWindow;
};

