#include "CarNumberDetection.h"

int main()
{
	Mat ori_img;
	//1. original
	Mat ori_image = imread("carnumber_ori.jpg");
	imshow("1.Original image", ori_image);
	//2. change gray
	Mat Gray_img;
	cvtColor(ori_image, Gray_img, COLOR_BGR2GRAY);
	imshow("2. gray image", Gray_img);
	//3 Edge
	Mat Canny_img;
	Canny(Gray_img, Canny_img, 100, 300, 3);
	imshow("3. Canny image", Canny_img);

	CarNumberDetection();
	
	
	waitKey(0);

}