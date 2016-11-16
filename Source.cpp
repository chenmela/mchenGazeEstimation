#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;
using namespace cv;

class BlobObject {
public:
	int blobs;
	std::string riskLevel;
};
//1280 x 1024 image size in pixels
//Loop through all parts of an image to take template from
//Loop through all images to compare against

//How can we return nothing if main returns type int?
void WriteArffFile(std::string outFileName, std::vector<BlobObject> featureVector);
void BlobDetection(int numImages, std::string letter, std::vector<BlobObject> &featureVector);
int main()
{

	//number of images to iterate through
	int NUM_CHARLIE_BROWN_LOW = 21;
	int NUM_CHARLIE_BROWN_MED = 11;
	int NUM_CHARLIE_BROWN_HIGH = 21;
	std::vector<BlobObject> featureVector;

	BlobDetection(NUM_CHARLIE_BROWN_LOW, "L", featureVector);
	BlobDetection(NUM_CHARLIE_BROWN_MED, "M", featureVector);
	BlobDetection(NUM_CHARLIE_BROWN_HIGH, "H", featureVector);
	WriteArffFile("BlobDetection.arff", featureVector);

	waitKey(0);
	return 0;
}

void BlobDetection(int numImages, std::string letter, std::vector<BlobObject> &featureVector) {

	for (int i = 0; i < numImages; i++) {
		BlobObject feature;
		std::string toRead = "C:\\Users\\IASA-FRI\\Desktop\\Project1\\Project1\\" + letter + std::to_string(i) + ".jpg";

		// Read image
		Mat im = imread(toRead);
		if (!im.empty()) {

			// Setup SimpleBlobDetector parameters.
			SimpleBlobDetector::Params params;

			// Change thresholds
			params.minThreshold = 6;
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


			// Storage for blobs
			vector<KeyPoint> keypoints;


#if CV_MAJOR_VERSION < 3   // If you are using OpenCV 2

			// Set up detector with params
			SimpleBlobDetector detector(params);

			// Detect blobs
			detector.detect(im, keypoints);
#else 

			// Set up detector with params
			Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

			// Detect blobs
			detector->detect(im, keypoints);
#endif 

			// Draw detected blobs as red circles.
			// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
			// the size of the circle corresponds to the size of blob

			Mat im_with_keypoints;
			drawKeypoints(im, keypoints, im_with_keypoints, Scalar(255, 255, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

			// Show blobs
			imshow("keypoints", im_with_keypoints);
			int numBlobs = keypoints.size();
			feature.blobs = numBlobs;
			feature.riskLevel = letter;

			featureVector.push_back(feature);
		}

	}
}

void WriteArffFile(std::string outFileName, std::vector<BlobObject> featureVector)
{
	std::ofstream out;
	out.open(outFileName);
	out << "% 1. Title: ASD Classification\n"
		<< "%\n"
		<< "% 2. Sources :\n"
		<< "% (a)Creator : NIH and Binghamton FRI 2016\n"
		<< "% (b)Date : Fall 2016\n"
		<< "%\n"
		<< "@RELATION asd\n"
		<< "\n"
		<< "@ATTRIBUTE numBlobs NUMERIC\n"
		<< "@ATTRIBUTE class {L, M, H}\n"
		<< "\n@DATA\n";

	//you need to have a vector that stores the number of blobs for each image
	//you also need to know what the diagnosis is for each image
	for (int i = 0; i < featureVector.size(); ++i)
	{
		out << featureVector[i].blobs << ", " << featureVector[i].riskLevel << std::endl;
		//imageDiagnosis.at(i)
	}
	out.close();
}