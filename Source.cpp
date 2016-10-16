#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

//1280 x 1024 image size

//Load main image
Mat img = imread("C:\\Users\\IASA-FRI\\Desktop\\Project1\\Project1\\test1.jpg");

//Load image from which to take template (template source)
Mat templateSource = imread("C:\\Users\\IASA-FRI\\Desktop\\Project1\\Project1\\test2.jpg");

//Select region of interest for template
Mat roi(templateSource, Rect(462, 590, 100, 100));
//Declare template image variable
Mat templ;


//Why use char* instead of string*? A string = a char pointer in C++. String* wasn't around whe openCV was written, but String* can be used.
//You can't use an int* because ints and chars are not in the same family.
//Why use pointers at all? They're 'faster' than using variables.
char* image_window = "Source Image";
char* result_window = "Result window";

//Declare variable to be associated with template matching method
int match_method = 1;
//Initialize trackbar variable to specify maximal position of trackbar on window
int max_Trackbar = 5;

//Define a method that is called every time the trackbar is moved
//Since MatchingMethod() and main() are in one file and main() comes before MatchingMethod(), 
//matchingMethod() must be declared otherwise main() will not know what it is!
void MatchingMethod(int, void*);

int main() {
	//Copy selected region of interest to template img matrix
	roi.copyTo(templ);
	//Create windows for displaying images
	//First parameter in namedWindow is a pointing to the name of the window
	//Second parameter can only be CV_WINDOW_AUTOSIZE (for our purposes)
	namedWindow(image_window, CV_WINDOW_AUTOSIZE);
	namedWindow(result_window, CV_WINDOW_AUTOSIZE);

	//Create trackbar

	//Why doesn't this display? Names too long, hard to edit this.
	char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
	//1st parameter: Trackbar name
	//2nd parameter: Window name
	//3rd parameter: Pointer to an integer variable whose value reflects the position of the slider. Upon creation, the slider position is defined by this variable
	//4th parameter: Maximal position of slider
	//5th parameter: Pointer to the function to be called every time the slider changes position
	createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);

	//Why is this here? Pass in arbitrary 0 and 0
	MatchingMethod(0, 0);

	waitKey(0);
	return 0;
}


void MatchingMethod(int, void*) {
	//Source image to display
	Mat img_display;
	img.copyTo(img_display);

	//Create the result matrix
	//Why are the number of columns and rows the way they are?
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;

	Mat result;
	result.create(result_rows, result_cols, CV_32FC1);

	//Do the matching
	//How does it know which match_method to use?
	matchTemplate(img, templ, result, match_method);
	//Review this normalize function
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	//Localizing the best match with minMaxLoc
	//minMaxLoc() finds the global minimum and maximum in an array
	//Declare minVal to store minimum value in array
	//Declare maxVal to store maximum value in array
	//Declare minLoc to store 2D Point (location of minimum value)
	//Declare maxLoc to store 2D Point (location of maximum value)
	//Declare matchLoc to be able to select between minLoc and maxLoc depending on match_method
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	//Call minMaxLoc() passing in array to be operated on, pointers to all the aforementioned variables
	//and a mask (which does nothing except select the whole array?)
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	//For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}

	//Draw rectangles surrounding selected points on both original image and resultant image
	//1st parameter: image
	//2nd parameter: top left vertex of rectangle
	//3rd parameter: bottom right vertex of rectangle
	//4th parameter: Alternative specification of the drawn rectangle (?)
	//5th parameter: line thickness
	//6th parameter: line type
	//7th parameter: Number of fractional bits in the point coordinates (??)
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

	//Display main image and resultant image (displaying comparison results)
	imshow(image_window, img_display);
	imshow(result_window, result);

	return;
}