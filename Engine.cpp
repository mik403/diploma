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

	//calc flow point and filter it 
	flow.calcFlow(rgbFrames);
	flow.filterResults();

	vector <Point2f> &filtered_points = flow.getResult();

	//clasterize results with using k-means
	seg.makeSegmentation(filtered_points);

	auto_ptr<Mat> centers = seg.getCenters();

	if (centers.get() == NULL) {
		return;
	}


	for (int i = 0; i < 25; ++i) {
		Point2f curr_c(centers->at<float>(i, 0), centers->at<float>(i, 1));
		p_trackers[i].update(curr_c);
	}

	for (int i = 0; i < 25; ++i) {
		for (int j = 0; j < centers->rows; ++j) {
			Point2f curr_c(centers->at<float>(j, 0), centers->at<float>(j, 1));
			p_trackers[i].update(curr_c);
		}

		p_trackers[i].nextFrame();

	}
	cout << "frame+" << rand()<< endl;

	drawTrackers();

	auto_ptr<Mat> labels = seg.getLabels();

	//for (int i = 0; i < filtered_points.size(); i++) {
	//	circle(rgbFrames, filtered_points[i], 7, Scalar(labels->at<int>(i, 0) * 20, labels->at<int>(i, 0) * 10, 0), 10, 1,
	//		0);
	//}

	//for (int i = 0; i < centers->rows; i++) {
	//	circle(rgbFrames, Point(centers->at<float>(i, 0), centers->at<float>(i, 1)), 10, Scalar(10, 100, 230));
	//}

	cv::imshow(rawWindow, rgbFrames);
}

void Engine::draw() {

}

void Engine::drawTrackers() {
	for (int i = 0; i < 25; ++i) {
		if (p_trackers[i].isTracked()) {
			circle(rgbFrames, p_trackers[i].getPoint(), 10, Scalar(10, 100, 230), 3);
			circle(rgbFrames, p_trackers[i].getPoint(), 20, Scalar(10, 100, 230), 1);
		}
	}
}

void Engine::resetTrackers() {
	for (int i = 0; i < 25; ++i) {
		p_trackers[i].reset();
	}
}