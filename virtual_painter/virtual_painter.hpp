#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp> // for the gui
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

vector<vector<int>> detectColour{ {96,40,40,179,255,255}, // purple
									{0,61,0,179,255,255}}; // green

vector<Scalar> detectColourValues{ {255,0,255}, // purple
									{0,255,0} }; // green

int findColor(Mat img) {
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	// hue, sat and value mins and maxes
	Scalar lower(hmin, smin, vmin);
	Scalar upper(hmax, smax, vmax);
	inRange(imgHSV, lower, upper, mask); // num colours = num masks

	return 0;
}
