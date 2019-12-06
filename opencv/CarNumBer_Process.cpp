#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
Mat Draw_Boundrect(Mat adaptiveThresholdimage, Mat drawing, Mat carnumber_copy)
{
	Rect rect, temp_rect;  //  Make temporary rectangles.
	vector<vector<Point> > contours_1;  //  Vectors for 'findContours' function.
	vector<Vec4i> hierarchy_1;
	double ratio, delta_x, delta_y, gradient;  //  Variables for 'Snake' algorithm.
	int select, plate_width, count, friend_count = 0, refinery_count = 0;
	findContours(adaptiveThresholdimage, contours_1, hierarchy_1, RETR_TREE, CHAIN_APPROX_SIMPLE, Point()); // 동일한 색상 강도 부분의 엣지 경계를 연결

	
	vector<vector<Point> > contours_poly(contours_1.size()); // 주변 윤곽선을 연결
	vector<Rect> boundRect(contours_1.size()); // 번호판마다 사각형 씌어주기
	vector<Rect> boundRect2(contours_1.size()); // 번호판 크기에 맞는 사격형을 추려서 다시 담는 정제된 사각형
	
	for (int i = 0; i < contours_1.size(); i++) {
		approxPolyDP(Mat(contours_1[i]), contours_poly[i], 1, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}
	drawing = Mat::zeros(adaptiveThresholdimage.size(), CV_8UC3);

	for (int i = 0; i < contours_1.size(); i++) {

		ratio = (double)boundRect[i].height / boundRect[i].width;

		//  Filtering rectangles height/width ratio, and size.
		if ((ratio <= 2.5) && (ratio >= 0.5) && (boundRect[i].area() <= 700) && (boundRect[i].area() >= 15)) {

			drawContours(drawing, contours_1, i, Scalar(0, 255, 255), 1, 8, hierarchy_1, 0, Point());
			rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 1, 8, 0);

			//  Include only suitable rectangles.
			boundRect2[refinery_count] = boundRect[i];
			refinery_count += 1;
		}
	}
	
	boundRect2.resize(refinery_count);  //  Resize refinery rectangle array.

	imshow("Original->Gray->Canny->Contours&Rectangles", drawing);
	waitKey(0);
	
	//  Bubble Sort accordance with X-coordinate.
	for (int i = 0; i < boundRect2.size(); i++) {
		for (int j = 0; j < (boundRect2.size() - i - 1); j++) {
			if (boundRect2[j].tl().x > boundRect2[j + 1].tl().x) {
				temp_rect = boundRect2[j];
				boundRect2[j] = boundRect2[j + 1];
				boundRect2[j + 1] = temp_rect;
			}
		}
	}
	/*

	for (int i = 0; i < boundRect2.size(); i++) {

		rectangle(carnumber_copy, boundRect2[i].tl(), boundRect2[i].br(), Scalar(0, 255, 0), 1, 8, 0);

		count = 0;


		for (int j = i + 1; j < boundRect2.size(); j++) {

			delta_x = abs(boundRect2[j].tl().x - boundRect2[i].tl().x);

			if (delta_x > 150)
				break;

			delta_y = abs(boundRect2[j].tl().y - boundRect2[i].tl().y);



			if (delta_x == 0) {
				delta_x = 1;
			}

			if (delta_y == 0) {
				delta_y = 1;
			}


			gradient = delta_y / delta_x;
			cout << gradient << endl;

			if (gradient < 0.25) {
				count += 1;
			}
		}


		if (count > friend_count) {
			select = i;
			friend_count = count;
			rectangle(carnumber_copy, boundRect2[select].tl(), boundRect2[select].br(), Scalar(255, 0, 0), 1, 8, 0);
			plate_width = delta_x;
		}

	rectangle(carnumber_copy, boundRect2[select].tl(), boundRect2[select].br(), Scalar(0, 0, 255), 2, 8, 0);
	line(carnumber_copy, boundRect2[select].tl(), Point(boundRect2[select].tl().x + plate_width, boundRect2[select].tl().y), Scalar(0, 0, 255), 1, 8, 0);
	*/
	/*
	vector<vector<Rect>> FindGroup(boundRect2.size());
	int FindGroup_count = 0;
	for (unsigned int i = 0; i < boundRect2.size() - 1; i++)
	{
		int Horiz_count = 0;
		unsigned int delta_x, delta_y;
		for (unsigned int j = i + 1; j < boundRect2.size(); j++)
		{
			delta_x = abs(boundRect2[j].x - boundRect2[i].x);
			delta_y = abs(boundRect2[j].y - boundRect2[i].y);

			if ((delta_x > 100) || ((delta_x == 0) && (delta_y == 0)))
			{
				cout << " m_Horiz_count(i-j) : " << i << "-" << j << "= " << Horiz_count << endl;
				break;
			}


			if ((delta_x > 5) && (delta_y < 10)) {
				double mGradient = (double)(delta_y / delta_x);

				if (mGradient < 0.15)
				{
					if (Horiz_count == 0)
					{
						FindGroup[FindGroup_count].push_back(boundRect2[i]);
					}
					FindGroup[FindGroup_count].push_back(boundRect2[j]);
					Horiz_count++;
				}
			}
		}
		if (Horiz_count > 8) {
			char mtemp[7];
			sprintf_s(mtemp, "%d", i);
			putText(carnumber_copy, mtemp, boundRect2[i].tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
			rectangle(carnumber_copy, boundRect2[i].tl(), boundRect2[i].br(), Scalar(255, 0, 0), 1, 8, 0);
			line(carnumber_copy, boundRect2[i].tl(), Point(boundRect2[i].tl().x + delta_x, boundRect2[i].tl().y), Scalar(0, 0, 255), 1, 8, 0);

			FindGroup_count++;
		}
		else {
			FindGroup[FindGroup_count].clear();
		}
		FindGroup.resize(FindGroup_count);
	}
	*/
	return carnumber_copy;
}

int main()
{
	Mat adaptiveThresholdimage,cannyimage, carnumberimage ,drawing, carnumberimage_copy;

	carnumberimage = imread("carnumber_ori.JPG");
	carnumberimage.copyTo(carnumberimage_copy);
	cvtColor(carnumberimage, carnumberimage, COLOR_BGR2GRAY);
	adaptiveThreshold(carnumberimage, adaptiveThresholdimage, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 10);
	Canny(adaptiveThresholdimage, adaptiveThresholdimage, 100, 300, 5);
	
	Draw_Boundrect(adaptiveThresholdimage, drawing, carnumberimage_copy);
	imshow("원본", carnumberimage_copy);
	waitKey(0);
	exit(0);
}