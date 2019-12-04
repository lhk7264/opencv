#include <opencv2/opencv.hpp>
#include <iostream>
#include "Image_Proccee.h"
using namespace cv;

Mat getProcceedImage(Mat carNumberImage) {

	ImageProccess imageProccess = ImageProccess();
	return imageProccess.getProcceedImage(carNumberImage);
}
/*
Mat getEdgeProccesedImage(Mat ProccessImage)
{
	ImageEdgeProccess edgeProccess  = ImageEdgeProccess();
	return edgeProccess.getEdgeProccesedImage(ProccessImage);
}
*/

int main() {

	Mat carNumberImage = imread("carnumber_1.jpg");
	Mat procceedImage = getProcceedImage(carNumberImage);
	//Mat edgeImage = getEdgeProccesedImage(procceedImage);

	imshow("¿ÃπÃ¡ˆ", procceedImage);

	waitKey(0);

}