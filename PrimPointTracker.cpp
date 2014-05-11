#include "PrimPointTracker.h"


void PrimPointTracker::update(Point2f _p) {

	if (status == LOST) { //one-way tracker
		return;
	}
	else if (status == INIT) {
		tracked_point = _p;
		lost_fr_count = 0;
		status = TRACK;
	}
	else {

		int curr_dist = sqrt(pow(_p.x - tracked_point.x, 2) + pow(_p.y - tracked_point.y, 2));

		if (curr_dist < radius && curr_dist < min_dist) {
			min_dist = curr_dist;
			tracked_point = _p;
			updated_on_curr_frame = true;
		}

	}

}

void PrimPointTracker::nextFrame() {
	if (!updated_on_curr_frame) {
		lost_fr_count++;

		if (lost_fr_count >= max_lost_frames) {
			status = LOST;
		}
	}
	else {
		lost_fr_count = 0;
	}

	updated_on_curr_frame = false;
	min_dist = INT_MAX;
}

void PrimPointTracker::reset() {
	status = INIT;
	lost_fr_count = 0;
	updated_on_curr_frame = true;
	min_dist = INT_MAX;
}