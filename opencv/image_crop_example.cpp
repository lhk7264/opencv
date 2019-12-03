
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
// using namespace std;

const int init_x = 112;
const int init_y = 204;
const int cropWidth = 116;
const int cropHeight = 40;

Mat cropCarNumber(Mat carNumberImage) {
	Rect rect = Rect(init_x, init_y, cropWidth, cropHeight);
	return carNumberImage(rect);
}

Mat getCarNumberImage() {
	return imread("carnumber_1.JPG");
}

Mat initCarNumberImage() {
	Mat carNumberImage = getCarNumberImage();
	return cropCarNumber(carNumberImage);
}

int main() {

	Mat carNumberImage = initCarNumberImage();

	imshow("cropedImage", carNumberImage);

	waitKey(5000);

	return 0;
}



