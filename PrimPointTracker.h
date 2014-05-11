#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class PrimPointTracker {

public:
	enum TrStatus{
		INIT = 0,
		TRACK,
		LOST
	};

	PrimPointTracker() {
		reset();
	}

	void update(Point2f _p);
	void nextFrame();
	void reset();

	inline Point2f getPoint() { return tracked_point; }
	inline bool isTracked() { return status == TRACK; }
	
	

private:

	TrStatus status;
	bool updated_on_curr_frame;
	int lost_fr_count;

	int min_dist;

	Point2f tracked_point;

	const int max_lost_frames = 10;
	const int radius = 20;
};