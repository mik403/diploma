#include "Segmentator.h"

void Segmentator::makeSegmentation(vector<Point2f> &points_to_segment) {
 

	if (points_to_segment.empty() || points_to_segment.size() < CLUSTER_COUNT) {
		return ;
	}


	Mat points(points_to_segment.size(), 2, CV_32F, Scalar(100));
	 
	res_centers.reset(new Mat(CLUSTER_COUNT, 1, points.type()));
	res_labels.reset(new Mat());

	for (int i = 0; i < points.rows; i++)
	{
		points.at<float>(i, 0) = points_to_segment.at(i).x;
		points.at<float>(i, 1) = points_to_segment.at(i).y;
	}


	kmeans(points, CLUSTER_COUNT, *res_labels, TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0), 30, KMEANS_PP_CENTERS, *res_centers);
	
	//fill point count for each cluster
	points_count.clear();
	for (int i = 0; i < res_labels->rows; ++i) {
		points_count[res_labels->at<int>(i, 0)]++;
	}
}