#include <opencv2/highgui/highgui.hpp>
using namespace cv;

int main()
{
	Mat image = imread("sample.png", IMREAD_COLOR);

	imshow("Load Image", image);
	waitKey(0);

	return 0;
}
