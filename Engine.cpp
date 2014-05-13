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

void Engine::update() {

	frame_count++;

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
	map<int, int > points_count = seg.getPointsCount();
	if (centers.get() == NULL) {
		return;
	}


	if (frame_count % 10 == 0) {
		resetTrackers();
	}

	//init meanshift
	if (init_tracker) {
		success_detection = false;
		cout << "Tracker scores:" << endl;

		for (int i = 0; i < TRACKERS_N; ++i) {

			if (!test_tracker[i].isLost()) {
				float score_ratio = test_tracker[i].getScore().y / abs(test_tracker[i].getScore().x);
				
				cout << i << " score: " << test_tracker[i].getScore().x << " " << test_tracker[i].getScore().y << " " <<score_ratio << endl;
				
				if (score_ratio > -0.05 && score_ratio < 0.05 && abs(test_tracker[i].getScore().x) > 30) {
					
					cout << endl << "SUCCES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
					success_detection = true;


				}
				
				
				//if (test_tracker[i].getScore().x > 2000 && test_tracker[i].getScore().x < 3000 &&
				//	test_tracker[i].getScore().y < 1200) {
				//	
				//	cout << endl << "SUCCES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
				//	success_detection = true;


				//}

			}

			Point2f curr_center(centers->at<float>(i, 0), centers->at<float>(i, 1));

			if (curr_center.x < 0.2*rgbFrames.cols || curr_center.x > 0.85*rgbFrames.cols ||
				curr_center.y < 0.2*rgbFrames.rows || curr_center.y > 0.85*rgbFrames.rows) {
				test_tracker[i].deactivate();
			}
			else {
				test_tracker[i].init(rgbFrames, curr_center);
			}
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

	//bool tracked_mask[TRACKERS_N];
	//memset(&tracked_mask, false, TRACKERS_N);

	//for (int i = 0; i < TRACKERS_N; ++i) {

	//	if (tracked_mask[i]) {
	//		continue;
	//	}

	//	Point2f curr_c(centers->at<float>(i, 0), centers->at<float>(i, 1));
	//	tracked_mask[i] = p_trackers[i].update(curr_c);
	//}

	//for (int i = 0; i < TRACKERS_N; ++i) {
	//	for (int j = 0; j < centers->rows; ++j) {

	//		if (tracked_mask[j]) {
	//			continue;
	//		}

	//		Point2f curr_c(centers->at<float>(j, 0), centers->at<float>(j, 1));
	//		tracked_mask[j] = p_trackers[i].update(curr_c);
	//	}

	//	p_trackers[i].nextFrame();

	//}
	//cout << "frame+" << rand()<< endl;

	//drawTrackers();

	//auto_ptr<Mat> labels = seg.getLabels();

	//for (int i = 0; i < filtered_points.size(); i++) {
	//	circle(rgbFrames, filtered_points[i], 7, Scalar(labels->at<int>(i, 0) * 20, labels->at<int>(i, 0) * 10, 0), 10, 1,
	//		0);
	//}

	for (int i = 0; i < centers->rows; i++) {
		circle(rgbFrames, Point(centers->at<float>(i, 0), centers->at<float>(i, 1)), 10, Scalar(10, 100, 230), 2);
	}

	line(rgbFrames, Point(0, 0.2 * rgbFrames.rows), Point(rgbFrames.cols, 0.2 * rgbFrames.rows), Scalar(255, 0, 0), 5);
	line(rgbFrames, Point(0, rgbFrames.rows - 0.2 * rgbFrames.rows), Point(rgbFrames.cols, rgbFrames.rows - 0.2 * rgbFrames.rows), Scalar(255, 0, 0), 5);

	line(rgbFrames, Point(0.2*rgbFrames.cols, 0), Point(0.2*rgbFrames.cols, rgbFrames.rows), Scalar(255, 0, 0), 5);
	line(rgbFrames, Point(0.85*rgbFrames.cols, 0), Point(0.85*rgbFrames.cols, rgbFrames.rows), Scalar(255, 0, 0), 5);


	cv::imshow(rawWindow, rgbFrames);
}

void Engine::draw() {

}

void Engine::drawTrackers() {

	for (int i = 0; i < TRACKERS_N; ++i) {
		if (p_trackers[i].isTracked()) {


			if (p_trackers[i].isPrevTracked()) {

				Scalar curr_color = p_trackers[i].getScore() < 1.0f && p_trackers[i].getScore() > 0.1f ? Scalar(10, 100, 230) : Scalar(0, 0, 0);

				circle(rgbFrames, p_trackers[i].getPoint(), 10, curr_color, 3);
				circle(rgbFrames, p_trackers[i].getPoint(), 60, Scalar(10, 100, 230), 1);

				line(rgbFrames, p_trackers[i].getPoint(), p_trackers[i].getPrevPoint(), Scalar(0, 0, 255), 2);
			}

		}
	}
}

void Engine::resetTrackers() {

	init_tracker = true;

	for (int i = 0; i < TRACKERS_N; ++i) {
		test_tracker[i].setId(i);
	}

	cout << "LOG: Trackers reseted" << endl;
}