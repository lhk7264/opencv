#include<opencv2/opencv.hpp>
#include<iostream>
#include <cstdlib>

using namespace cv;
using namespace std;

class ImageEdgeProccess
{
	Mat imageEdgeProccess(Mat ProccessImage)
	{
		Mat ProccessImage_Canny, drawing;

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		Canny(ProccessImage, ProccessImage_Canny, 100, 300, 5);
		findContours(ProccessImage_Canny, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
		//vector<vector<Point> > contours_poly(contours.size);

	}
public:
	Mat getEdgeProccesedImage(Mat ProccessImage) {
		return imageEdgeProccess(ProccessImage);
	}

		
};