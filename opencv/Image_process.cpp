#include<opencv2/opencv.hpp>
#include<iostream>
#include "Image_process.h"

using namespace cv;

Mat	Image_process::getcarnumberimage()
{
	return imread("carnumber_1.jpg");;
}

Mat	Image_process::imageProcess(Mat carnumberImage)
{
	Mat grayImage, adaptiveImage;

	cvtColor(carnumberImage, grayImage, COLOR_BGR2GRAY);
	adaptiveThreshold(grayImage, adaptiveImage, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 10);
	Canny(adaptiveImage, adaptiveImage, 100, 300, 3);

	return adaptiveImage;
}

Mat	Image_process::getProcceedImage()
{
	Mat carnumberImage = getcarnumberimage();

	return imageProcess(carnumberImage);
}
