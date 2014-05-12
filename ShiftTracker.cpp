#include "ShiftTracker.h"

#include <iostream>

void ShiftTracker::init(Mat &image, Point2f start) {
	float hranges[] = { 0, 180 };
	const float* phranges = hranges;
	int histSize = 256;
	roi = image(Range(start.y - 15, start.y + 15),
		Range(start.x - 15, start.x + 15));
	
	track_window = Rect(start.x - 15,start.y - 15, 30, 30);

	cvtColor(roi, hsv_roi, COLOR_BGR2HSV);

	inRange(hsv_roi, Scalar(0, 60, 32), Scalar(180, 255, 255), mask);

	int ch[] = { 0, 0 };
	Mat hue(hsv_roi.size(), hsv_roi.depth());
	mixChannels(&hsv_roi, 1, &hue, 1, ch, 1);



	calcHist(&hue, 1, 0, mask, roi_hist, 1, &histSize, &phranges);
	normalize(roi_hist, roi_hist, 0, 255, NORM_MINMAX);
	term_crit = TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1);

	inited = true;

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

	int strange_number =  meanShift(dst, track_window, term_crit);

	if (strange_number < 1) {
		rectangle(frame, track_window, Scalar(0, 0, 255), 2);
	}
	else {
		rectangle(frame, track_window, Scalar(0, 255, 0), 2);
	}

	//RotatedRect trackBox = CamShift(dst, track_window, term_crit);

	//ellipse(frame, trackBox, Scalar(0, 0, 255), 3, CV_AA);
}