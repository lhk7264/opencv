#include "Filtering_contour.h"

Rect Filtering_contour(Mat Canny_img, Mat Ori_img)
{
	vector<vector<Point> > mContours;
	vector<Vec4i> mhierarchy;

	findContours(Canny_img, mContours, mhierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
	vector<vector<Point>> mContours_poly(mContours.size());
	vector<Rect> mBoundRect(mContours.size());
	vector<Rect> mBoundRect2(mContours.size());

	for (unsigned int i = 0; i < mContours.size(); i++)
	{
		approxPolyDP(Mat(mContours[i]), mContours_poly[i], 1, true);
		mBoundRect[i] = boundingRect(Mat(mContours_poly[i]));
	}
	Mat mDrawing_findContour = Mat::zeros(Canny_img.size(), CV_8UC3);
	for (unsigned int i = 0; i < mContours.size(); i++) {
		drawContours(mDrawing_findContour, mContours, i, Scalar(0, 255, 255), 1, 8, mhierarchy, 0, Point());
		rectangle(mDrawing_findContour, mBoundRect[i].tl(), mBoundRect[i].br(), Scalar(255, 0, 0), 1, 8, 0);
	}
	imshow("4. contour image", mDrawing_findContour);

	Mat mDrawing = Mat::zeros(Canny_img.size(), CV_8UC3);
	int refind_count = 0;
	for (unsigned int i = 0; i < mContours.size(); i++) {
		double mRatio = (double)mBoundRect[i].height / mBoundRect[i].width;

		if (((mRatio <= 3.0) && (mRatio >= 0.5))
			&& ((mBoundRect[i].area() <= 160) && (mBoundRect[i].area() >= 40)))
		{
			drawContours(mDrawing, mContours, i, Scalar(0, 255, 255), 1, 8, mhierarchy, 0, Point());
			rectangle(mDrawing, mBoundRect[i].tl(), mBoundRect[i].br(), Scalar(255, 0, 0), 1, 8, 0);

			mBoundRect2[refind_count] = mBoundRect[i];

			cout << "contour :" << refind_count << " X:" << mBoundRect2[refind_count].x << " Y:" << mBoundRect2[refind_count].y
				<< " heigh:" << mBoundRect2[refind_count].height << " width:" << mBoundRect2[refind_count].width << " area:" << mBoundRect2[refind_count].area() << endl;

			refind_count += 1;
		}
	}

	mBoundRect2.resize(refind_count);
	imshow("5. Filtering_contour", mDrawing);

	//bubble sorting to X-coordinate
	for (unsigned int i = 1; i < mBoundRect2.size(); i++)
	{
		for (unsigned int j = 0; j < (mBoundRect2.size() - i - 1); j++)
		{
			if (mBoundRect2[j].tl().x > mBoundRect2[j + 1].tl().x) {
				Rect temp_rect;
				temp_rect = mBoundRect2[j];
				mBoundRect2[j] = mBoundRect2[j + 1];
				mBoundRect2[j + 1] = temp_rect;
			}
		}
	}

	//Log......
	for (unsigned int i = 0; i < mBoundRect2.size(); i++) {
		cout << "after bubble contour :" << i << " X:" << mBoundRect2[i].x << " Y:" << mBoundRect2[i].y
			<< " heigh:" << mBoundRect2[i].height << " width:" << mBoundRect2[i].width << " area:" << mBoundRect2[i].area() << endl;
	}

	//Find Number...
	vector<vector<Rect> > FindGroup(mBoundRect2.size());
	int FindGroup_count = 0;
	
	Mat mDrawing_findContour2;
	Ori_img.copyTo(mDrawing_findContour2);
	for (unsigned int i = 0; i < mBoundRect2.size() - 1; i++)
	{
		int m_Horiz_count = 0;
		unsigned int delta_x, delta_y;
		for (unsigned int j = i + 1; j < mBoundRect2.size(); j++)
		{
			delta_x = abs(mBoundRect2[j].x - mBoundRect2[i].x);
			delta_y = abs(mBoundRect2[j].y - mBoundRect2[i].y);

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
						FindGroup[FindGroup_count].push_back(mBoundRect2[i]);
					}
					FindGroup[FindGroup_count].push_back(mBoundRect2[j]);
					m_Horiz_count++;
				}
			}
		}

		//숫자 잡기
		if (m_Horiz_count > 8) {
			char mtemp[7];
			sprintf_s(mtemp, "%d", i);
			putText(mDrawing_findContour2, mtemp, mBoundRect2[i].tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
			rectangle(mDrawing_findContour2, mBoundRect2[i].tl(), mBoundRect2[i].br(), Scalar(255, 0, 0), 1, 8, 0);
			//line(image3, mBoundRect2[i].tl(), Point(mBoundRect2[i].tl().x + delta_x, mBoundRect2[i].tl().y), Scalar(0, 0, 255), 1, 8, 0);

			FindGroup_count++;
		}
		else {
			FindGroup[FindGroup_count].clear();
		}

	}
	FindGroup.resize(FindGroup_count);

	imshow("6.Filtering_contour2", mDrawing_findContour2);
	return FindGroup_count;
}