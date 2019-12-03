#include <opencv2/opencv.hpp>
#include <iostream>
#include "Image_Proccee.h"

using namespace cv;


Mat getProcceedImage(Mat carNumberImage) {

	ImageProccess imageProccess = ImageProccess();
	return imageProccess.getProcceedImage(carNumberImage);
}

int main() {

	Mat carNumberImage = imread("carnumber_1.jpg");
	Mat procceedImage = getProcceedImage(carNumberImage);

	imshow("d", procceedImage);

	waitKey(5000);

}
