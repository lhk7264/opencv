class Image_process
{
	const int f_x = 112;
	const int l_y = 204;
	const int w = 116;
	const int h = 40;

public:
	Mat getcarnumberimage();
	Mat imageProcess(Mat carnumberImage);
	Mat getProcceedImage();
};
