#include <opencv2/opencv.hpp>
#include <iostream>  

using namespace cv;
using namespace std;


int main()
{
	Mat img_input = imread("house.jpg", IMREAD_GRAYSCALE);
	if (img_input.empty()) {
		cout << "이미지를 읽을 수 없습니다." << endl;
	}

	Mat img_canny;
	Canny(img_input, img_canny, 50, 150);

	Mat img_result;
	hconcat(img_input, img_canny, img_result);

	namedWindow("result", WINDOW_AUTOSIZE);
	imshow("result", img_result);

	waitKey(0);

	return 0;
}