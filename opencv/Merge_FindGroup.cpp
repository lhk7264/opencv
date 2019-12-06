#include "Merge_FindGroup.h"

void Merge_FindGroup(Mat Ori_img)
{
	Mat mGroup;
	Ori_img.copyTo(mGroup);
	vector<Rect> mGroup_merge;

	for (int i = 0; i < FindGrop.size(); i++)
	{
		vector<Rect> mTempGroup = FindGrop[i];
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

	imshow("7.Merge_1", mGroup);


	//bubble sorting to X-coordinate
	for (unsigned int i = 1; i < mGroup_merge.size(); i++)
	{
		for (unsigned int j = 0; j < (mGroup_merge.size() - i); j++)
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
					(abs(mGroup_merge[j + 1].y - mGroup_merge[j].y) < 8)
					&& (abs(mGroup_merge[j + 1].x - mGroup_merge[j].x) < 23)
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
					mGroup_merge.erase(mGroup_merge.begin() + j + 1);
					i = -1;
					break;
				}
			}
		}
		test_flag = 0;
	} while (test_flag);

	Mat mGroup_final;
	ori_image.copyTo(mGroup_final);
	for (unsigned int i = 0; i < mGroup_merge.size(); i++)
	{
		char mtemp[7];
		sprintf_s(mtemp, "%d", i);
		putText(mGroup_final, mtemp, mGroup_merge[i].tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
		rectangle(mGroup_final, mGroup_merge[i].tl(), mGroup_merge[i].br(), Scalar(255, 0, 0), 1, 8, 0);

		cout << "after Group final:" << i << "= X:" << mGroup_merge[i].x << " Y:" << mGroup_merge[i].y
			<< " h:" << mGroup_merge[i].height << " w:" << mGroup_merge[i].width << " A:" << mGroup_merge[i].area() << endl;

	}


	imshow("8.Merge_2", mGroup_final);
}