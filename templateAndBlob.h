#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;
using namespace cv;

//1280 x 1024 image size in pixels
//Loop through all parts of an image to take template from
//Loop through all images to compare against

//How can we return nothing if main returns type int?
int main() {
	//BLOB DETECTION

	//number of images to iterate through
	int NUM_CHARLIE_BROWN_LOW = 21;
	int NUM_CHARLIE_BROWN_MED = 11;
	int NUM_CHARLIE_BROWN_HIGH = 21;

	for (int i = 0; i < NUM_CHARLIE_BROWN_LOW; i++) {
		std::string toRead = "C:\\Users\\IASA - FRI\\Desktop\\Project1\\Project1\\L" + std::to_string(i) + ".jpg";

		Mat im = imread(toRead);

		// Setup SimpleBlobDetector parameters.
		SimpleBlobDetector::Params params;

		// Change thresholds
		params.minThreshold = 9;
		//6-10
		params.maxThreshold = 50;

		// Filter by Area.
		params.filterByArea = true;
		params.minArea = 50;

		// Filter by Circularity
		params.filterByCircularity = false;
		params.minCircularity = 0.1;

		// Filter by Convexity
		params.filterByConvexity = false;
		params.minConvexity = 0.87;

		// Filter by Inertia
		params.filterByInertia = false;
		params.minInertiaRatio = 0.01;

		vector<KeyPoint> keypoints;

		//#if is a preprocessor directive; it controls what compiles
#if CV_MAJOR_VERSION < 3   // If you are using OpenCV 2

	// Set up detector with params
		SimpleBlobDetector detector(params);

		// You can use the detector this way
		// detector.detect( im, keypoints);

#else

	// Set up detector with params
		Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

		// SimpleBlobDetector::create creates a smart pointer. 
		// So you need to use arrow ( ->) instead of dot ( . )
		// detector->detect( im, keypoints);
		detector->detect(im, keypoints);

#endif
		// Draw detected blobs as red circles.
		// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
		// the size of the circle corresponds to the size of blob

		Mat im_with_keypoints;
		drawKeypoints(im, keypoints, im_with_keypoints, Scalar(255, 255, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

		// Show blobs
		imshow("keypoints", im_with_keypoints);
		//imwrite("L" + std::to_string(i), im_with_keypoints);

		//int numBlobs = keypoints.size();
	}
	waitKey(0);
	return 0;
}