#include "OFCalculator.h"

void OFCalculator::initCapture(std::string file_name) {
	cap.open(file_name);

	if (!cap.isOpened()) {
		return;
	}

	ready_to_process = true;
	namedWindow(rawWindow, CV_WINDOW_AUTOSIZE);

	opticalFlow = Mat(cap.get(CV_CAP_PROP_FRAME_HEIGHT),
		cap.get(CV_CAP_PROP_FRAME_HEIGHT), CV_32FC3);
	winSize.height = winSize.width = 31;

	needToInit = true;
}

void OFCalculator::update() {
	calcFlow();
	filterResults();
	segmentResults(true);
}

void OFCalculator::calcFlow() {
	goodFeaturesToTrack(grayFrames, points1, MAX_COUNT, 0.01, 10, Mat(),
		3, 0, 0.04);

	std::swap(points2, points1);
	points1.clear();
	grayFrames.copyTo(prevGrayFrame);

	TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);

	cap >> frame;
	cap >> frame;
	cap >> frame;

	frame.copyTo(rgbFrames);
	cvtColor(rgbFrames, grayFrames, CV_BGR2GRAY);

	if (needToInit) {
		goodFeaturesToTrack(grayFrames, points1, MAX_COUNT, 0.01, 3, Mat(),
			3, 0, 0.04);
		if (!points1.empty()) {
			needToInit = false;
		}
	}
	else if (!points2.empty()) {

		calcOpticalFlowPyrLK(prevGrayFrame, grayFrames, points2, points1,
			status, err, winSize, 3, termcrit, 0, 0.001);
	}
}


void OFCalculator::filterResults() {

	int i, k;

	points_to_segment.clear();

	for (i = k = 0; i < points2.size(); i++) {


		if (fabs(points1[i].x - points2[i].x) <= 1) {
			continue;
		}

		points_to_segment.push_back(points1[i]);
	}
}

void OFCalculator::segmentResults(bool draw) {

	int clusterCount = 5;

	if (points_to_segment.empty() || points_to_segment.size() < clusterCount) {
		return;
	}


	Mat points(points_to_segment.size(), 2, CV_32F, Scalar(100));
	Mat labels;
	Mat centers(clusterCount, 1, points.type());

	for (int i = 0; i < points.rows; i++)
	{
		points.at<float>(i, 0) = points_to_segment.at(i).x;
		points.at<float>(i, 1) = points_to_segment.at(i).y;
	}


	kmeans(points, clusterCount, labels, TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0), 30, KMEANS_PP_CENTERS, centers);

	if (draw) {
		for (int i = 0; i < clusterCount; i++) {
			circle(rgbFrames, Point(centers.at<float>(i, 0), centers.at<float>(i, 1)), 10, Scalar(10, 100, 230));
		}
	}

}

void OFCalculator::draw() {

	int i, k;

	for (i = k = 0; i < points2.size(); i++) {

		// not effective, but it works
		if (fabs(points1[i].x - points2[i].x) <= 1) {
			continue;
		}

		if ((points1[i].x - points2[i].x) > 0) {
			line(rgbFrames, points1[i], points2[i], Scalar(0, 0, 255),
				1, 1, 0);

			circle(rgbFrames, points1[i], 2, Scalar(255, 0, 0), 1, 1,
				0);
		}
		else {
			line(rgbFrames, points1[i], points2[i], Scalar(0, 255, 0),
				1, 1, 0);

			circle(rgbFrames, points1[i], 2, Scalar(255, 0, 0), 1, 1,
				0);

		}
		points1[k++] = points1[i];

	}

	cv::imshow(rawWindow, rgbFrames);
}