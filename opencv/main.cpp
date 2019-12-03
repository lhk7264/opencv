#include<opencv2/opencv.hpp>
#include<iostream>
#include <cstdlib>

using namespace cv;
using namespace std;

int main()
{
	Mat image, image2, image3, drawing, imagethr, imagethr2;  //  Make images.
	Rect rect, temp_rect;  //  Make temporary rectangles.
	vector<vector<Point> > contours;  //  Vectors for 'findContours' function.
	vector<Vec4i> hierarchy;

	double ratio, delta_x, delta_y, gradient;  //  Variables for 'Snake' algorithm.
	int select, plate_width, count, friend_count = 0, refinery_count = 0;
	

	image = imread("carnumber_1.JPG");

	imshow("원본", image);
	waitKey(0);



	image.copyTo(image2);
	image.copyTo(image3);

	cvtColor(image2, image2, COLOR_BGR2GRAY);  //  그레이 변환 단순화
	imshow("Original->Gray", image2);
	waitKey(0);
	
	//GaussianBlur(image2,Blurimage,Size(5, 5), 1.0);
	//Mat imageRoi = image2(Rect(112, 242, image2.cols, image2.rows));
	//Rect rect1(112, 242, image2.cols, image2.rows);
	//Mat imageRoi = image2(rect1);
	//Rect rect1(112, 242, image2.cols, image2.rows);
	//Mat subImage = image2(rect1);
	//imshow("roi", subImage);

	adaptiveThreshold(image2, imagethr2, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 10);
	

	Canny(imagethr2, imagethr2, 100, 300, 5);
	imshow("원본->그레이 변환->엣지 검출", imagethr2);
	waitKey(0);


	//  Finding contours.
	findContours(imagethr2, contours, hierarchy, RETR_TREE,CHAIN_APPROX_SIMPLE, Point(0,0)); // 동일한 색상 강도 부분의 엣지 경계를 연결
	vector<vector<Point> > contours_poly(contours.size()); // 주변 윤곽선을 연결
	/*
	if (contours.size > 0)
	{
		for (int idx = 0; idx < contours.size; idx++) {
			Rect rect = boundingRect(contours.size);
			if (rect.height > 10 && rect.width > 19 && !(rect.width >= 351 - 5 && rect.height >= 283 - 5)) {
				rectangle(image3, Point(rect.br().x - rect.width, rect.br().y - rect.height),rect.br(),Scalar(0, 255, 0), 1,8,0);
		}
	
	}
	*/
	

	/*
		vector<Rect> boundRect(contours.size()); // 번호판마다 사각형 씌어주기
	    vector<Rect> boundRect2(contours.size()); // 번호판 크기에 맞는 사격형을 추려서 다시 담는 정제된 사각형
	//  Bind rectangle to every rectangle.
	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 1, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

	drawing = Mat::zeros(imagethr2.size(), CV_8UC3);

	for (int i = 0; i < contours.size(); i++) {

		ratio = (double)boundRect[i].height / boundRect[i].width;

		if ((ratio <= 10.0) && (ratio >= 0.5) && (boundRect[i].area() <= 700) && (boundRect[i].area() >= 20)) {

			drawContours(drawing, contours, i, Scalar(0, 255, 255), 1, 8, hierarchy, 0, Point());
			//rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 1, 8, 0);

		
			boundRect2[refinery_count] = boundRect[i];
			refinery_count += 1;
		}
	}

	boundRect2.resize(refinery_count); 

	imshow("Original->Gray->Canny->Contours&Rectangles", drawing);
	waitKey(0);


	
	for (int i = 0; i < boundRect2.size(); i++) {
		for (int j = 0; j < (boundRect2.size() - i-1); j++) {
			if (boundRect2[j].tl().x > boundRect2[j + 1].tl().x) {
				temp_rect = boundRect2[j];
				boundRect2[j] = boundRect2[j + 1];
				boundRect2[j + 1] = temp_rect;
			}
		}
	}


	for (int i = 0; i < boundRect2.size(); i++) {

		rectangle(image3, boundRect2[i].tl(), boundRect2[i].br(), Scalar(0, 255, 0), 1, 8, 0);

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
			rectangle(image3, boundRect2[select].tl(), boundRect2[select].br(), Scalar(255, 0, 0), 1, 8, 0);
			plate_width = delta_x;  
		} 
		
	}
	*/

	//rectangle(image3, boundRect2[select].tl(), boundRect2[select].br(), Scalar(0, 0, 255), 2, 8, 0);
	//line(image3, boundRect2[select].tl(), Point(boundRect2[select].tl().x + plate_width, boundRect2[select].tl().y), Scalar(0, 0, 255), 1, 8, 0);

	imshow("Rectangles on original", image3);
	waitKey(0);



	

//	imwrite("carnumber-1.JPG",
//		image(Rect(boundRect2[select].tl().x - 20, boundRect2[select].tl().y - 20, plate_width + 40, plate_width * 0.3)));
	
	exit(0);
}
