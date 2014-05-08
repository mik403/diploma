#include "Segmentator.h"

auto_ptr<Mat> Segmentator::makeSegmentation(vector<Point2f> &points_to_segment) {
 

	auto_ptr<Mat> res;

	if (points_to_segment.empty() || points_to_segment.size() < clusterCount) {
		return res;
	}


	Mat points(points_to_segment.size(), 2, CV_32F, Scalar(100));
	Mat labels;
	 
	res.reset(new Mat(clusterCount, 1, points.type()));
	

	for (int i = 0; i < points.rows; i++)
	{
		points.at<float>(i, 0) = points_to_segment.at(i).x;
		points.at<float>(i, 1) = points_to_segment.at(i).y;
	}


	kmeans(points, clusterCount, labels, TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0), 30, KMEANS_PP_CENTERS, *res);

	return res;

}