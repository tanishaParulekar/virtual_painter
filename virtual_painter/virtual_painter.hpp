#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp> // for the gui
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

vector<vector<int>> detectColour{ {108,47,0,150,255,255}, // purple
									{18,94,0,179,255,255} }; // green
									//{0,132,0,179,255,255}}; // orange

vector<Scalar> detectColourValues{ {255,0,255}, // purple
									{0,255,0} }; // green
									//{51,153,255}}; // orange

int getContours(Mat imgDil, Mat img) {
	// Part 1: get shape contours
	// Each array in the array will have a contour
	vector<vector<Point>> contours;
	// openCV has a datatype of a vector of 4 integers - Vec4i
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	/*

	// print (draw) the contours - id -1 means all contours
	drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	*/

	// declare output contours vector to use in approxPolyDP method inside for loop
	vector<vector<Point>> contourPoly(contours.size());
	// vector to store boundingRect values
	vector<Rect> boundRect(contours.size());
	// shape identifier
	string objType;

	// Part 2: filter out any noise/variation by calculating area of each contour - in this image it is the tiny black circle
	for (int i = 0; i < contours.size(); i++) {
		// calculate and print out each contour area
		int area = contourArea(contours[i]);
		cout << area << endl;

		// use contour area values calculated to choose a threshold (in this case 1000)
		if (area > 1000) {
			// Part 3: find approximation of type of shape
			float perimeter = arcLength(contours[i], true);
			// find approximation of curves - i.e. the corner points
			approxPolyDP(contours[i], contourPoly[i], 0.02 * perimeter, true);

			// print (draw) the approximated contours (i.e. points) the algorithm has found - id -1 means all contours
			drawContours(img, contourPoly, i, Scalar(255, 0, 255), 2);

			// print #points found for each shape
			cout << contourPoly[i].size() << endl;

			// Part 4: draw a bounding rectanlge around each shape
			boundRect[i] = boundingRect(contourPoly[i]);
		}
	}

	return 0;
}

int findColor(Mat img) {
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	// for each colour, find a mask
	for (int i = 0; i < detectColour.size(); i++) {
		// hue, sat and value mins and maxes
		Scalar lower(detectColour[i][0], detectColour[i][1], detectColour[i][2]);
		Scalar upper(detectColour[i][3], detectColour[i][4], detectColour[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);

		//imshow(to_string(i), mask); // use first element of each vector row as identifier
		getContours(mask, img);
	}
	
	return 0;
}

