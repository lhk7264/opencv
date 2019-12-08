#include "NumBer_Find.h"

int main()
{
	Mat ori_image, Gray_img, Canny_img;
	ori_image = imread("carnumber_ori.jpg");
	imshow("1.Original image", ori_image);

	cvtColor(ori_image, Gray_img, COLOR_BGR2GRAY);
	imshow("2. gray image", Gray_img);

	Canny(Gray_img, Canny_img, 100, 300, 3);
	imshow("3. Canny image", Canny_img);

	NumBer_Find(Canny_img, ori_image);
	waitKey(0);

}