#include "PrimPointTracker.h"


bool PrimPointTracker::update(Point2f _p) {

	//if (status == LOST) { //one-way tracker
	//	return;
	//}
	//else 
	if (status == INIT || status == LOST) {
	 
		tracked_point = _p;
		lost_fr_count = 0;
		accum_score.x = accum_score.y = 0;
		prev_status = status;
		status = TRACK;
	}
	else {

		int curr_dist = sqrt(pow(_p.x - tracked_point.x, 2) + pow(_p.y - tracked_point.y, 2));

		if (curr_dist < radius && curr_dist < min_dist) {
			min_dist = curr_dist;
			prev_tracked_point = _p;
			updated_on_curr_frame = true;
			
			prev_status = status;
			status = TRACK;
		}

	}

	return updated_on_curr_frame;
}

void PrimPointTracker::nextFrame() {
	if (!updated_on_curr_frame) {
		lost_fr_count++;

		if (lost_fr_count >= max_lost_frames) {
			prev_status = status;
			status = LOST;
		}
	}
	else {
		//swap it
		Point2f p = tracked_point;
		tracked_point = prev_tracked_point;
		prev_tracked_point = p;
		accum_score.x = fabs(prev_tracked_point.x - tracked_point.x);
		accum_score.y = fabs(prev_tracked_point.y - tracked_point.y);
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