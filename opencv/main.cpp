#include <opencv2/opencv.hpp>  //  Include 'OpenCV' library.

#include <iostream>
#include <cstdlib>
#include <stdio.h> 
#include <io.h> 
#include <conio.h> 

using namespace cv;
using namespace std;

int main()
{
	Mat image, image2, image3, drawing, image2_f, image2_canny;  //  Make images.
	Rect rect, temp_rect;  //  Make temporary rectangles.
	vector<vector<Point> > contours;  //  Vectors for 'findContours' function.
	vector<Vec4i> hierarchy;

	double ratio, delta_x, delta_y, gradient;  //  Variables for 'Snake' algorithm.
	int select, plate_width, count, friend_count = 0, refinery_count = 0;



	image = imread("carnumber_ori - 1.JPG");  //  Load an image file.

	imshow("Original", image);
	waitKey(0);



	image.copyTo(image2);  //  Copy to temporary images.
	image.copyTo(image3);  //  'image2' - to preprocessing, 'image3' - to 'Snake' algorithm.

	cvtColor(image2, image2, COLOR_BGR2GRAY);  //  Convert to gray image.
	imshow("Original->Gray", image2);
	waitKey(0);
	//adaptiveThreshold(image2, image2_f, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 10);
	Canny(image2, image2_canny, 100, 300, 5);  //  Getting edges by Canny algorithm.
	imshow("Original->Gray->Canny", image2_canny);
	
	

vector<vector<Point> > mContours;
vector<Vec4i> mhierarchy;
findContours(image2_canny, mContours, mhierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

vector<vector<Point>> mContours_poly(mContours.size());
vector<Rect> mBoundRect(mContours.size());
vector<Rect> mBoundRect2(mContours.size());

for (unsigned int i = 0; i < mContours.size(); i++)
{
	approxPolyDP(Mat(mContours[i]), mContours_poly[i], 1, true);
	mBoundRect[i] = boundingRect(Mat(mContours_poly[i]));
}
/*
#if 1 //test
Mat mDrawing1 = Mat::zeros(image2_canny.size(), CV_8UC3);
for (unsigned int i = 0; i < mContours.size(); i++) {
	drawContours(mDrawing1, mContours, i, Scalar(0, 255, 255), 1, 8, mhierarchy, 0, Point());
	rectangle(mDrawing1, mBoundRect[i].tl(), mBoundRect[i].br(), Scalar(255, 0, 0), 1, 8, 0);
}
imshow("4-0. contour", mDrawing1);
#endif
*/



Mat mDrawing = Mat::zeros(image2_canny.size(), CV_8UC3);
int refind_count = 0;
for (unsigned int i = 0; i < mContours.size(); i++) {
	double mRatio = (double)mBoundRect[i].height / mBoundRect[i].width;

	if (((mRatio <= 3.0) && (mRatio >= 0.5))
		&& ((mBoundRect[i].area() <= 160) && (mBoundRect[i].area() >= 90)))
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
imshow("4. contour", mDrawing);

//bubble sorting to X-coordinate
for (unsigned int i = 1; i < mBoundRect2.size(); i++)
{
	for (unsigned int j = 0; j < (mBoundRect2.size() - i-1); j++)
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
vector<vector<Rect>> mFindGroup(mBoundRect2.size());
int mFindGroup_count = 0;
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


		if ((delta_x >=0) && (delta_y < 15)) {
			double mGradient = (double)(delta_y / delta_x);

			if (mGradient < 0.15)
			{
				if (m_Horiz_count == 0)
				{
					mFindGroup[mFindGroup_count].push_back(mBoundRect2[i]);
				}
				mFindGroup[mFindGroup_count].push_back(mBoundRect2[j]);
				m_Horiz_count++;
			}
		}
	}
	
	//Result detect number1
	if (m_Horiz_count > 8) {
		char mtemp[7];
		sprintf_s(mtemp, "%d", i);
		putText(image3, mtemp, mBoundRect2[i].tl(), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
		rectangle(image3, mBoundRect2[i].tl(), mBoundRect2[i].br(), Scalar(255, 0, 0), 1, 8, 0);
		line(image3, mBoundRect2[i].tl(), Point(mBoundRect2[i].tl().x + delta_x, mBoundRect2[i].tl().y), Scalar(0, 0, 255), 1, 8, 0);

		mFindGroup_count++;
	}
	else {
		mFindGroup[mFindGroup_count].clear();
	}
	
}
mFindGroup.resize(mFindGroup_count);

imshow("4.Group", image3);



Mat mGroup;
vector<Rect> mGroup_merge;

for (int i = 0; i < mFindGroup.size(); i++)
{
	vector<Rect> mTempGroup = mFindGroup[i];
	char mtemp[7];
	sprintf_s(mtemp, "%d", i);
	putText(mGroup, mtemp, mTempGroup[0].tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));

	int mMaxHight = 0;
	for (int j = 0; j < mTempGroup.size(); j++)
	{
		if (mTempGroup[j].height > mMaxHight) mMaxHight = mTempGroup[j].height;
	}

	Point mWiHi;
	mWiHi.x = mTempGroup[mTempGroup.size() - 1].br().x;
	mWiHi.y = mTempGroup[0].y + mMaxHight;
	rectangle(mGroup, mTempGroup[0].tl(), mWiHi, Scalar(255, 0, 0), 1, 8, 0);


	cout << " 5.Group2 : " << i << "== x: " << mTempGroup[0].x << "  y:" << mTempGroup[0].y
		<< "  w:" << mWiHi.x << "  h:" << mWiHi.y << endl;

	Rect mTemp_merge;
	mTemp_merge.x = mTempGroup[0].x;
	mTemp_merge.y = mTempGroup[0].y;
	mTemp_merge.width = mWiHi.x - mTempGroup[0].x;
	mTemp_merge.height = mWiHi.y - mTempGroup[0].y;

	mGroup_merge.push_back(mTemp_merge);
}

//imshow("5.Group2", mGroup);


//sorting ¸ÕÀú.....
//bubble sorting to X-coordinate
for (unsigned int i = 1; i < mGroup_merge.size(); i++)
{
	for (unsigned int j = 0; j < (mGroup_merge.size() - i-1); j++)
	{
		if (mGroup_merge[j].tl().y > mGroup_merge[j + 1].tl().y) {
			Rect temp_rect;
			temp_rect = mGroup_merge[j];
			mGroup_merge[j] = mGroup_merge[j + 1];
			mGroup_merge[j + 1] = temp_rect;
		}
	}
}

for (unsigned int i = 0; i < mGroup_merge.size(); i++)
{
	cout << "after bubble Group :" << i << "= X:" << mGroup_merge[i].x << " Y:" << mGroup_merge[i].y
		<< " h:" << mGroup_merge[i].height << " w:" << mGroup_merge[i].width << " A:" << mGroup_merge[i].area() << endl;
}



int test_flag = 1;

do
{
	for (int i = 0; i < mGroup_merge.size(); i++)
	{
		for (int j = i; j < (mGroup_merge.size() - 1); j++)
		{
			if (
				(abs(mGroup_merge[j + 1].y - mGroup_merge[j].y) < 5)
				&& (abs(mGroup_merge[j + 1].x - mGroup_merge[j].x) < 30)
				//					&& ((abs(mGroup_merge[j + 1].x - mGroup_merge[j].x) < 20)
				//					|| ((mGroup_merge[j + 1].x > mGroup_merge[j].x) && (mGroup_merge[j + 1].x < mGroup_merge[j].br().x)))
				)
			{
				if (mGroup_merge[j].x > mGroup_merge[j + 1].x)
				{
					mGroup_merge[j].x = mGroup_merge[j + 1].x;
				}

				if (mGroup_merge[j].y > mGroup_merge[j + 1].y)
				{
					mGroup_merge[j].y = mGroup_merge[j + 1].y;
				}

				if (mGroup_merge[j].br().x < mGroup_merge[j + 1].br().x)
				{
					mGroup_merge[j].width = mGroup_merge[j + 1].br().x - mGroup_merge[j].x;
				}

				//			if (mGroup_merge[j].br().y < mGroup_merge[j + 1].br().y)
				//			{
				//				mGroup_merge[j].height = mGroup_merge[j + 1].br().y - mGroup_merge[j].y;
				//			}

				mGroup_merge.erase(mGroup_merge.begin() + j + 1);
				i = -1;
				break;
			}
		}
	}
	test_flag = 0;
} while (test_flag);
waitKey(0);


return 0;
}



