#include<opencv2/opencv.hpp>
#include<iostream>
#include "Edge_process.h"

using namespace cv;

class ImageProccess {

	const int f_x = 112;
	const int l_y = 204;
	const int w = 116;
	const int h = 40;


	Mat imageProcess(Mat carnumberImage) {

		Mat grayImage, adaptiveImage;
		Rect rect(f_x, l_y, w, h);
		Mat cropImage = carnumberImage(rect);
		cvtColor(cropImage, grayImage, COLOR_BGR2GRAY);
		adaptiveThreshold(grayImage, adaptiveImage, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 10);
		Canny(adaptiveImage, adaptiveImage, 100, 300, 3);

		return adaptiveImage;
	}

public:
	Mat getProcceedImage(Mat carnumberImage)
	{

		return imageProcess(carnumberImage);
	}

};

