#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat img_color;

	img_color = imread("house.jpg", IMREAD_COLOR);
	if (img_color.empty())
	{
		cout << "Not Open" << endl;
		return -1;
	}

	Mat img_gray;


	cvtColor(img_color, img_gray, COLOR_BGR2GRAY);
	imwrite("gray house.jpg", img_gray);

	Mat img_result;
	hconcat(img_color, img_gray, img_result);
	
	namedWindow("result", WINDOW_AUTOSIZE);
	imshow("result", img_result);


	while (waitKey(0) != 27);

	return 0;
}