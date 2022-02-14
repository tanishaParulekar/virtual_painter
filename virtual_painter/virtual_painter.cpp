#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp> // for the gui
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "virtual_painter.hpp"

using namespace cv;
using namespace std;

int main() {
	// replace path with an id # of camera - usually related to main cameras and number of cameras
	// id 0 is laptop cam, 1 is webcam
	VideoCapture cap(0);
	Mat img;
	while (true) {
		cap.read(img);
		newPoints = findColor(img);
		drawOnCanvas(newPoints, img);
		imshow("Image", img);
		
		waitKey(1);// so you want a super small delay (in ms)
	}

	return 0;
}