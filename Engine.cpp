#include "Engine.h"

void Engine::initCapture(std::string file_name) {
	
	cap.open(file_name);

	if (!cap.isOpened()) {
		return;
	}

	flow.init();

	namedWindow(rawWindow, CV_WINDOW_AUTOSIZE);
}

void Engine::update() {
	 
	cap >> rgbFrames;
	cap >> rgbFrames;
	cap >> rgbFrames;

	flow.calcFlow(rgbFrames);
	flow.filterResults();
	vector <Point2f> &filtered_points = flow.getResult();
	Segmentator seg;
	seg.makeSegmentation(filtered_points);

	auto_ptr<Mat> centers = seg.getCenters();

	if (centers.get() == NULL) {
		return;
	}

	auto_ptr<Mat> labels = seg.getLabels();

	for (int i = 0; i < filtered_points.size(); i++) {
		circle(rgbFrames, filtered_points[i], 7, Scalar(labels->at<int>(i, 0) * 20, labels->at<int>(i, 0) * 10, 0), 10, 1,
			0);
	}

	for (int i = 0; i < centers->rows; i++) {
		circle(rgbFrames, Point(centers->at<float>(i, 0), centers->at<float>(i, 1)), 10, Scalar(10, 100, 230));
	}

	cv::imshow(rawWindow, rgbFrames);
}

void Engine::draw() {}