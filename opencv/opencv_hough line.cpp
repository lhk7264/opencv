#include <opencv2/core/mat.hpp>  
#include <opencv2/imgcodecs.hpp>  
#include <opencv2/imgproc.hpp>  
#include <opencv2/highgui.hpp>  

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	Mat img_original = imread("line1.jpg", IMREAD_COLOR);
	Mat img_edge, img_gray;


	//�׷��� ������ �������� ��ȯ ����.  
	cvtColor(img_original, img_gray, COLOR_BGR2GRAY);
	//ĳ�Ͽ����� �̿��Ͽ� ���� ������ �����մϴ�.  
	Canny(img_gray, img_edge, 30, 90, 3);


	//���� ������ �����մϴ�.  
	vector<Vec4i> lines;
	HoughLinesP(img_edge, lines, 1, CV_PI / 180, 30, 30, 3);

	//������ ������ ���� �׷��ݴϴ�.  
	for (int i = 0; i < lines.size(); i++)
	{
		Vec4i L = lines[i];
		line(img_original, Point(L[0], L[1]), Point(L[2], L[3]),
			Scalar(0, 0, 255), 1, LINE_AA);
	}

	namedWindow("edge", WINDOW_AUTOSIZE);
	imshow("edge", img_edge);

	namedWindow("result", WINDOW_AUTOSIZE);
	imshow("result", img_original);

	waitKey(0);

	return 0;
}