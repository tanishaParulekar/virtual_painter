#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp> // for the gui
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// store all values from which to draw in a vector
// where each inside vector stores {x, y, indexOfDetectColourVector}
vector<vector<int>> newPoints;

vector<vector<int>> detectColour{ {108,47,0,150,255,255}, // purple
									{18,94,0,179,255,255} }; // green
									//{0,132,0,179,255,255}}; // orange

vector<Scalar> detectColourValues{ {255,0,255}, // purple
									{0,255,0} }; // green
									//{51,153,255}}; // orange

// returns the point of the object from which to draw
// here we aim for top centre of object (marker)
Point getContours(Mat imgDil, Mat img) {
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
	// to find tip of marker
	Point myPoint(0, 0);

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
			// get bounding box at top. Use this to find tip of marker. 
			// (instead of using contour to make easier)
			// want to draw rectangle from the middle, not the side
			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			// for y coordinate don't want centre 
			myPoint.y = boundRect[i].y;

			// bound with green box (rectangle)
			// tl() is top left, br() is bottom right
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
					
		}
	}

	return myPoint;
}

// store point of colour from which to draw
vector<vector<int>> findColor(Mat img) {
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	// for each colour, find a mask
	for (int i = 0; i < detectColour.size(); i++) {
		// hue, sat and value mins and maxes
		Scalar lower(detectColour[i][0], detectColour[i][1], detectColour[i][2]);
		Scalar upper(detectColour[i][3], detectColour[i][4], detectColour[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);

		/*
		use first element of each vector row as identifier
		imshow(to_string(i), mask); 
		*/
		// get top centre point of marker to draw from
		Point myPoint = getContours(mask, img);
		if (myPoint.x != 0 && myPoint.y != 0) {
			// store the points
			newPoints.push_back({ myPoint.x, myPoint.y, i });
		}
			
	}
	
	return newPoints;
}

// accept newPoints and colour values 
int drawOnCanvas(vector<vector<int>> newPoints, Mat img) {
	for (int i = 0; i < newPoints.size(); i++) {
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, detectColourValues[newPoints[i][2]], FILLED);
	}
	
	return 0;
}

