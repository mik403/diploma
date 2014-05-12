#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;


class ShiftTracker {

public:
	ShiftTracker() :inited(false){};

	void init(Mat &image, Point2f start);
	void update(Mat &frame);

private:
	TermCriteria term_crit;
	Mat mask, roi_hist, hsv_roi, roi;
	Mat hsv, dst;
	Rect track_window;
	bool inited;
};