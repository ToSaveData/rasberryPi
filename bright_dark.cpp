#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("./lena.bmp", IMREAD_GRAYSCALE);
	CV_Assert(!image.empty());

	Mat dst1 = image + 100;
	Mat dst2 = image - 100;
	Mat dst3 = 255 -image;

	Mat dst4(image.size(), image.type());
	Mat dst5(image.size(), image.type());

	for(int i = 0; i < image.rows; i++){
		for(int j = 0; j < image.cols;j++){
			dst4.at<uchar>(i,j) = image.at<uchar>(i, j) + 100;
			dst5.at<uchar>(i,j) = 255- image.at<uchar>(i, j);
			}
		}
	imshow("circle", image), imshow("dst1 - birghten", dst1);
	imshow("dst2 - darken", dst2), imshow("dst3 - contrast", dst3);
	imshow("dst4 - brighten", dst4), imshow("dst5 - contrast", dst5);
	waitKey();

	return 0;
}

