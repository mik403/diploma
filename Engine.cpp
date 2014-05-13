#include "Engine.h"

void Engine::initCapture(std::string file_name) {

	cap.open(file_name);

	if (!cap.isOpened()) {
		return;
	}

	flow.init();
	frame_count = 0;
	resetTrackers();

	namedWindow(rawWindow, CV_WINDOW_AUTOSIZE);
}

void Engine::analyzeDetection(int det_i, Point2f &curr_center) {

	if (!test_tracker[det_i].isLost()) {
		float score_ratio = test_tracker[det_i].getScore().y / abs(test_tracker[det_i].getScore().x);

		cout << det_i << " score: " << test_tracker[det_i].getScore().x << " " << test_tracker[det_i].getScore().y << " " << score_ratio << endl;

		//first, check final position vector, otherwice check scores
		if (initial_tracker_positions[det_i].x > 0.6*rgbFrames.cols &&
			initial_tracker_positions[det_i].x < test_tracker[det_i].getDetCenter().x) {}
		else if (score_ratio > -0.15 && score_ratio < 0.15 && abs(test_tracker[det_i].getScore().x) > 30) {
			cout << endl << "SUCCES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			success_detection = true;
		}
	}

	if (curr_center.x < 0.2*rgbFrames.cols || curr_center.x > 0.85*rgbFrames.cols ||
		curr_center.y < 0.2*rgbFrames.rows || curr_center.y > 0.85*rgbFrames.rows) {
		test_tracker[det_i].deactivate();
	} else {
		test_tracker[det_i].init(rgbFrames, curr_center);
	}
}


void Engine::update() {

	frame_count++;

	//analyze each 3 frame because of high video framerate
	cap >> rgbFrames;
	cap >> rgbFrames;
	cap >> rgbFrames;

	//calc flow points and filter it 
	flow.calcFlow(rgbFrames);
	flow.filterResults();

	vector <Point2f> &filtered_points = flow.getResult();

	//clasterize results with using k-means
	seg.makeSegmentation(filtered_points);

	auto_ptr<Mat> centers = seg.getCenters();
	map<int, int > points_count = seg.getPointsCount();

	if (centers.get() == NULL) {
		return;
	}

	if (frame_count % tact_lenght == 0) {
		resetTrackers();
	}

	//init meanshift
	if (init_tracker) {
		success_detection = false;
		cout << "Tracker scores:" << endl;

		for (int i = 0; i < TRACKERS_N; ++i) {
			Point2f curr_center(centers->at<float>(i, 0), centers->at<float>(i, 1));
			analyzeDetection(i, curr_center);
			initial_tracker_positions[i] = curr_center;
		}

		init_tracker = false;
	}

	//update trackers
	for (int i = 0; i < TRACKERS_N; ++i) {
		test_tracker[i].update(rgbFrames);
	}

	if (success_detection) {
		line(rgbFrames, Point(0, rgbFrames.rows - 5), Point(rgbFrames.cols - 5, rgbFrames.rows - 5),
			Scalar(0, 255, 255), 10);
	}

	for (int i = 0; i < centers->rows; i++) {
		circle(rgbFrames, Point(centers->at<float>(i, 0), centers->at<float>(i, 1)), 10, Scalar(10, 100, 230), 2);
	}


}

void Engine::draw() {

	//auto_ptr<Mat> labels = seg.getLabels();

	//for (int i = 0; i < filtered_points.size(); i++) {
	//	circle(rgbFrames, filtered_points[i], 7, Scalar(labels->at<int>(i, 0) * 20, labels->at<int>(i, 0) * 10, 0), 10, 1,
	//		0);
	//}

	line(rgbFrames, Point(0, 0.2 * rgbFrames.rows), Point(rgbFrames.cols, 0.2 * rgbFrames.rows), Scalar(255, 0, 0), 5);
	line(rgbFrames, Point(0, rgbFrames.rows - 0.2 * rgbFrames.rows), Point(rgbFrames.cols, rgbFrames.rows - 0.2 * rgbFrames.rows), Scalar(255, 0, 0), 5);

	line(rgbFrames, Point(0.2*rgbFrames.cols, 0), Point(0.2*rgbFrames.cols, rgbFrames.rows), Scalar(255, 0, 0), 5);
	line(rgbFrames, Point(0.85*rgbFrames.cols, 0), Point(0.85*rgbFrames.cols, rgbFrames.rows), Scalar(255, 0, 0), 5);

	cv::imshow(rawWindow, rgbFrames);
}

void Engine::drawTrackers() {

}

void Engine::resetTrackers() {

	init_tracker = true;
	for (int i = 0; i < TRACKERS_N; ++i) {
		test_tracker[i].setId(i);
	}

	cout << "LOG: Trackers reseted" << endl;
}