#include "CarNumberDetection.h"

int CarNumberDetection()
{

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
	
	//3 FindContours
	vector<vector<Point> > Contours;
	vector<Vec4i> mhierarchy;
	
	findContours(Canny_img, Contours, mhierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	vector<vector<Point>> Contours_poly(Contours.size());
	vector<Rect> BoundRect(Contours.size());
	vector<Rect> BoundRect2(Contours.size());

	for (unsigned int i = 0; i < Contours.size(); i++)
	{
		approxPolyDP(Mat(Contours[i]), Contours_poly[i], 1, true);
		BoundRect[i] = boundingRect(Mat(Contours_poly[i]));
	}
	Mat mDrawing_findContour = Mat::zeros(Canny_img.size(), CV_8UC3);
	for (unsigned int i = 0; i < Contours.size(); i++) {
		drawContours(mDrawing_findContour, Contours, i, Scalar(0, 255, 255), 1, 8, mhierarchy, 0, Point());
		rectangle(mDrawing_findContour, BoundRect[i].tl(), BoundRect[i].br(), Scalar(255, 0, 0), 1, 8, 0);
	}
	imshow("4. contour image", mDrawing_findContour);

	Mat mDrawing = Mat::zeros(Canny_img.size(), CV_8UC3);
	int refind_count = 0;
	for (unsigned int i = 0; i < Contours.size(); i++) {
		double mRatio = (double)BoundRect[i].height / BoundRect[i].width;

		if (((mRatio <= 3.0) && (mRatio >= 0.5))
			&& ((BoundRect[i].area() <= 160) && (BoundRect[i].area() >= 40)))
		{
			drawContours(mDrawing, Contours, i, Scalar(0, 255, 255), 1, 8, mhierarchy, 0, Point());
			rectangle(mDrawing, BoundRect[i].tl(), BoundRect[i].br(), Scalar(255, 0, 0), 1, 8, 0);

			BoundRect2[refind_count] = BoundRect[i];

			cout << "contour :" << refind_count << " X:" << BoundRect2[refind_count].x << " Y:" << BoundRect2[refind_count].y
				<< " heigh:" << BoundRect2[refind_count].height << " width:" << BoundRect2[refind_count].width << " area:" << BoundRect2[refind_count].area() << endl;

			refind_count += 1;
		}
	}

	BoundRect2.resize(refind_count);
	imshow("5. Filtering_contour", mDrawing);

	//bubble sorting to X-coordinate
	for (unsigned int i = 1; i < BoundRect2.size(); i++)
	{
		for (unsigned int j = 0; j < (BoundRect2.size() - i-1); j++)
		{
			if (BoundRect2[j].tl().x > BoundRect2[j + 1].tl().x) {
				Rect temp_rect;
				temp_rect = BoundRect2[j];
				BoundRect2[j] = BoundRect2[j + 1];
				BoundRect2[j + 1] = temp_rect;
			}
		}
	}

	//Log......
	for (unsigned int i = 0; i < BoundRect2.size(); i++) {
		cout << "after bubble contour :" << i << " X:" << BoundRect2[i].x << " Y:" << BoundRect2[i].y
			<< " heigh:" << BoundRect2[i].height << " width:" << BoundRect2[i].width << " area:" << BoundRect2[i].area() << endl;
	}

	//Find Number...
	vector<vector<Rect>> FindGroup(BoundRect2.size());
	int FindGroup_count = 0;
	Mat image3;
	ori_image.copyTo(image3);
	for (unsigned int i = 0; i < BoundRect2.size() - 1; i++)
	{
		int m_Horiz_count = 0;
		unsigned int delta_x, delta_y;
		for (unsigned int j = i + 1; j < BoundRect2.size(); j++)
		{
			delta_x = abs(BoundRect2[j].x - BoundRect2[i].x);
			delta_y = abs(BoundRect2[j].y - BoundRect2[i].y);

			if ((delta_x > 100) || ((delta_x == 0) && (delta_y == 0)))
			{
				cout << " m_Horiz_count(i-j) : " << i << "-" << j << "= " << m_Horiz_count << endl;
				break;
			}


			if ((delta_x > 5) && (delta_y < 10)) {
				double mGradient = (double)(delta_y / delta_x);

				if (mGradient < 1.5)
				{
					if (m_Horiz_count == 0)
					{
						FindGroup[FindGroup_count].push_back(BoundRect2[i]);
					}
					FindGroup[FindGroup_count].push_back(BoundRect2[j]);
					m_Horiz_count++;
				}
			}
		}

		//숫자 잡기
		if (m_Horiz_count > 8) {
			char temp[7];
			sprintf_s(temp, "%d", i);
			putText(image3, temp, BoundRect2[i].tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
			rectangle(image3, BoundRect2[i].tl(), BoundRect2[i].br(), Scalar(255, 0, 0), 1, 8, 0);
			//line(image3, BoundRect2[i].tl(), Point(BoundRect2[i].tl().x + delta_x, BoundRect2[i].tl().y), Scalar(0, 0, 255), 1, 8, 0);

			FindGroup_count++;
		}
		else {
			FindGroup[FindGroup_count].clear();
		}

	}
	FindGroup.resize(FindGroup_count);

	imshow("6.Filtering_contour2", image3);



	Mat Group;
	ori_image.copyTo(Group);
	vector<Rect> Group_merge;

	for (int i = 0; i < FindGroup.size(); i++)
	{
		vector<Rect> tempGroup = FindGroup[i];
		char temp[7];
		sprintf_s(temp, "%d", i);
		putText(Group, temp, tempGroup[0].tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));

		int MaxHight = 0;
		for (int j = 0; j < tempGroup.size(); j++)
		{
			if (tempGroup[j].height > MaxHight) MaxHight = tempGroup[j].height;
		}

		Point WiHi;
		WiHi.x = tempGroup[tempGroup.size() - 1].br().x;
		WiHi.y = tempGroup[0].y + MaxHight;
		rectangle(Group, tempGroup[0].tl(), WiHi, Scalar(255, 0, 0), 1, 8, 0);


		cout << " 5.Group2 : " << i << "== x: " << tempGroup[0].x << "  y:" << tempGroup[0].y
			<< "  w:" << WiHi.x << "  h:" << WiHi.y << endl;

		Rect temp_merge;
		temp_merge.x = tempGroup[0].x;
		temp_merge.y = tempGroup[0].y;
		temp_merge.width = WiHi.x - tempGroup[0].x;
		temp_merge.height = WiHi.y - tempGroup[0].y;

		Group_merge.push_back(temp_merge);
	}

	imshow("7.Merge_1", Group);


	//bubble sorting to X-coordinate
	for (unsigned int i = 1; i < Group_merge.size(); i++)
	{
		for (unsigned int j = 0; j < (Group_merge.size() - i); j++)
		{
			if (Group_merge[j].tl().y > Group_merge[j + 1].tl().y) {
				Rect temp_rect;
				temp_rect = Group_merge[j];
				Group_merge[j] = Group_merge[j + 1];
				Group_merge[j + 1] = temp_rect;
			}
		}
	}

	for (unsigned int i = 0; i < Group_merge.size(); i++)
	{
		cout << "after bubble Group :" << i << "= X:" << Group_merge[i].x << " Y:" << Group_merge[i].y
			<< " h:" << Group_merge[i].height << " w:" << Group_merge[i].width << " A:" << Group_merge[i].area() << endl;
	}

	int test_flag = 1;

	do
	{
		for (int i = 0; i < Group_merge.size(); i++)
		{
			for (int j = i; j < (Group_merge.size() - 1); j++)
			{
				if (
					(abs(Group_merge[j + 1].y - Group_merge[j].y) < 8)
					&& (abs(Group_merge[j + 1].x - Group_merge[j].x) < 23)
					)
				{
					if (Group_merge[j].x > Group_merge[j + 1].x)
					{
						Group_merge[j].x = Group_merge[j + 1].x;
					}

					if (Group_merge[j].y > Group_merge[j + 1].y)
					{
						Group_merge[j].y = Group_merge[j + 1].y;
					}

					if (Group_merge[j].br().x < Group_merge[j + 1].br().x)
					{
						Group_merge[j].width = Group_merge[j + 1].br().x - Group_merge[j].x;
					}
					Group_merge.erase(Group_merge.begin() + j + 1);
					i = -1;
					break;
				}
			}
		}
		test_flag = 0;
	} while (test_flag);

	Mat Group_final;
	ori_image.copyTo(Group_final);
	for (unsigned int i = 0; i < Group_merge.size(); i++)
	{
		char temp[7];
		sprintf_s(temp, "%d", i);
		putText(Group_final, temp, Group_merge[i].tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
		rectangle(Group_final, Group_merge[i].tl(), Group_merge[i].br(), Scalar(255, 0, 0), 1, 8, 0);

		cout << "after Group final:" << i << "= X:" << Group_merge[i].x << " Y:" << Group_merge[i].y
			<< " h:" << Group_merge[i].height << " w:" << Group_merge[i].width << " A:" << Group_merge[i].area() << endl;

	}
	

	imshow("8.Merge_2", Group_final);

	waitKey(0);


	return 0;

}










