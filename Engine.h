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

using namespace cv;

class Engine {
public:
	Engine() : rawWindow("Raw Video") {}

	void initCapture(std::string file_name);

	void update();
	void draw();

private:

	Mat rgbFrames;
	VideoCapture cap;

	OFCalculator flow;
	
	string rawWindow;
};

