#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class PrimPointTracker {

	enum TrStatus{
		INIT = 0,
		TRACK,
		LOST
	};

	PrimPointTracker() {
		status = INIT;
		lost_fr_count = 0;
	}

	void update(Point2f _p) { 
	
		if (status == INIT) {
			tracked_point = _p;
			lost_fr_count = 0;
		}
		else {

			int curr_dist = sqrt(_p.x* tracked_point.x + _p.y* tracked_point.y);

			if (curr_dist > radius) {
				
				lost_fr_count++;
				if (lost_fr_count >= max_lost_frames) {
					status = LOST;
				} 

			}
			else {
				tracked_point = _p;
			}

		}

	}

private:

	TrStatus status;
	int lost_fr_count;

	Point2f tracked_point;

	const int max_lost_frames = 5;
	const int radius = 30;
};