#include "ShiftTracker.h"

#include <iostream>

void ShiftTracker::init(Mat &image, Point2f start) {
	
	//check boundaries first
	if (start.y - 25 < 0 || start.y + 25 >= image.rows ||
		start.x - 25 < 0 || start.x + 25 >= image.cols) {
		
		inited = false;
		lost = true;
		return;
	}

	track_window = Rect(start.x - 25, start.y - 25, 50, 50);

	prev_wnd_center.x = track_window.x;
	prev_wnd_center.y = track_window.y;

	roi = image(Range(start.y - 25, start.y + 25),
		Range(start.x - 25, start.x + 25));

	float hranges[] = { 0, 180 };
	const float* phranges = hranges;
	int histSize = 256;

	cvtColor(roi, hsv_roi, COLOR_BGR2HSV);

	inRange(hsv_roi, Scalar(0, 60, 32), Scalar(180, 255, 255), mask);

	int ch[] = { 0, 0 };
	Mat hue(hsv_roi.size(), hsv_roi.depth());
	mixChannels(&hsv_roi, 1, &hue, 1, ch, 1);

	calcHist(&hue, 1, 0, mask, roi_hist, 1, &histSize, &phranges);
	normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX);
	term_crit = TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1);

	inited = true;
	lost = false;

	score = Point2f(0, 0);
}

void ShiftTracker::update(Mat &frame) {

	if (!inited) {
		return;
	}

	cvtColor(frame, hsv, COLOR_BGR2HSV);
	float hranges[] = { 0, 180 };
	const float* phranges = hranges;

	int ch[] = { 0, 0 };
	Mat hue(hsv.size(), hsv.depth());
	mixChannels(&hsv, 1, &hue, 1, ch, 1);

	calcBackProject(&hue, 1, 0, roi_hist, dst, &phranges);

	int strange_number = meanShift(dst, track_window, term_crit);

	if (strange_number < 1) {
		lost = true;
	} else {

		rectangle(frame, track_window, Scalar(0, 255, 0), 2);
		putText(frame, std::to_string(id), getDetCenter(), CV_FONT_HERSHEY_SIMPLEX, 1.0f, Scalar(0, 255, 0), 2);

		lost = false;

		score.x += track_window.x - prev_wnd_center.x;// no matter which side it is moving
		score.y += track_window.y - prev_wnd_center.y;

		prev_wnd_center.x = track_window.x;
		prev_wnd_center.y = track_window.y;
	}

}