#include "hough.hpp"

void max_object(Mat img, Rect &rect ) 
{
	vector<vector<Point>> contours;
	findContours(img.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);	// �ܰ��� ����
	
	int max_area = 0 ;
	for (int i = 0; i < (int)contours.size(); i++)	// ���� ū ���� ��������
	{
		Rect r = boundingRect(contours[i]);			// �ܰ��� ���� ���� �簢��
		if (max_area < r.area()) {
			max_area = r.area();
			rect = r;
		}
	}
	rect = rect - Point(10, 10) + Size(20, 20);
}

int main()
{
	Rect  rect;
	Mat		gray, canny, morph, th_gray, canny_line, dst;
	double rho = 1, theta = CV_PI / 180;				// ������ȯ �Ÿ�����, ��������
	vector<Vec2f> lines;								// ���� ���� ���ε�

	Mat  image = imread("../5.tif" , 1);
	//Mat  image = imread("./1.bmp" , 1);
	CV_Assert(image.data);

	cvtColor(image, gray, COLOR_BGR2GRAY);						// ���ϵ� ���� ��ȯ
	threshold(gray, th_gray, 240, 255, THRESH_BINARY);		//���� ���� ��ȯ
	erode(th_gray, morph, Mat(), Point(-1, -1), 2);			// ħ�� ����

	max_object(morph, rect);								// ���� ū ��ü �˻�
	rectangle(morph, rect, Scalar(100), 2);					// �˻� ��ü ǥ��

	Canny(th_gray(rect), canny, 40, 100);
	houghLines(canny, lines, rho, theta, 50);
	draw_houghLines(canny, canny_line, lines, 1);

	double angle = (CV_PI - lines[0][1]) * 180 / CV_PI  ;
	Point  center = image.size() / 2;
	Mat rot_map = getRotationMatrix2D(center, -angle, 1);
	
	warpAffine(image, dst, rot_map, image.size(), INTER_LINEAR);
	imshow("morph", morph);
	imshow("image", image);
	imshow("line", canny_line);
	imshow("dst", dst);

	resizeWindow("line", 150, 150);
	waitKey();
}

//resize(gray, resize_gray, Size(), scale, scale);			// ���� ���
//Canny(resize_gray, canny, 40, 100);						// �ɴ� ���� ����
//dilate(canny, morph, Mat(), Point(-1, -1), 2);			// ��â ���� 2�� ����

//max_object(morph, rect);									// ���� ū ��ü ����

//// ���� �ϳ׽� �簢�� ���� ũ�� ���� 
//Point pt1 = rect.tl() / scale;
//Point pt2 = rect.br() / scale;
//Rect	harness_rect(pt1, pt2);					// �ϳ׽� �簢�� 
//Mat	harness_object = gray(harness_rect);	// ���� ������
