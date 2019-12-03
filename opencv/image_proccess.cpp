#include<opencv2/opencv.hpp>
#include<iostream>
#include "test_crop_image_each_funtion.h"

using namespace cv;

class ImageProccess {

	const int f_x = 112;
	const int l_y = 204;
	const int w = 116;
	const int h = 40;


	Mat getcarnumberimage()
	{
		return imread("carnumber_1.jpg");;
	}


	Mat imageProcess(Mat carnumberImage) {

		Mat grayImage, adaptiveImage;

		cvtColor(carnumberImage, grayImage, COLOR_BGR2GRAY);
		adaptiveThreshold(grayImage, adaptiveImage, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 10);
		//Canny(adaptiveImage, adaptiveImage, 100, 300, 3);

		return adaptiveImage;
	}

	Mat getProcceedImage()
	{

		Mat carnumberImage = getcarnumberimage();

		return imageProcess(carnumberImage);
	}

};

