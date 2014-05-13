#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;


class ShiftTracker {
public:
	
	ShiftTracker() :inited(false){};
	
	void setId(int _id) { id = _id; }
	void deactivate() { inited = false; lost = true; }
	void init(Mat &image, Point2f start);
	void update(Mat &frame);
	
	inline bool isLost() { return lost; }
	inline Point2f getScore() { return score; }
	inline Point2f getDetCenter() { return Point2f(track_window.x + track_window.width / 2, 
												   track_window.y + track_window.height / 2); }
private:
	
	int id;

	TermCriteria term_crit;
	Mat mask, roi_hist, hsv_roi, roi;
	Mat hsv, dst;
	
	Rect track_window;
	
	bool inited;
	bool lost;

	Point2f score;
	Point2f prev_wnd_center;
};