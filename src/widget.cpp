#include "widget.h"
#include <QFileDialog>
#include <QImage>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

void FlagBlob(Mat &input, int row0, int row1, int col0, int col1);

Widget::Widget(QWidget *parent)
	: QWidget(parent)
{
	setWindowTitle("BloackPointCheck");

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(&label1);
	layout->addWidget(&label2);

	setLayout(layout);
}

Widget::~Widget()
{

}

//显示文件对话框
void Widget::showDialog()
{
	path = QFileDialog::getOpenFileName(this, "Please Open Image", "D:\\image", "Image File(*.png *.bmp *.jpg)");
	if (path == NULL)
		return;

	showSrc();

	ImageProcess();
}

void Widget::showSrc()
{
	QImage img;
	img.load(path);
	int w = img.width();
	int h = img.height();

	int w2 = this->width();
	int h2 = this->height();

//	cout << "11111111111111111 == " << w2 << endl;
//	cout << "22222222222222222 == " << h2 << endl;

	QPixmap pix = QPixmap::fromImage(img); //加载灰度图
	label1.setPixmap(pix);

//	WindowResize(w, h); //缩放窗面
}

void Widget::showEffect()
{
	QImage img;
	img.load("D:\\result.bmp");

	QPixmap pix = QPixmap::fromImage(img); //加载灰度图
	label2.setPixmap(pix);
}

void Widget::ImageProcess()
{
	Mat srcImage, dstImage;
	string name = path.toStdString();
	srcImage = imread(name);

//	imshow("src", srcImage);

	Mat src;
	cvtColor(srcImage, src, CV_RGB2GRAY);
//	srcImage.copyTo(src); //复制

	Mat filter;
	boxFilter(src, filter, -1, Size(120, 120));

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.rows; j++)
		{
			uchar pixSrc = src.at<uchar>(i, j);
			uchar pixFilter = filter.at<uchar>(i, j);
			if (pixSrc > (pixFilter + 10))
			{
				src.at<uchar>(i, j) = pixFilter; //去除白点
			}
		}
	}

	boxFilter(src, filter, -1, Size(60, 60));

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.rows; j++)
		{
			uchar pixSrc = src.at<uchar>(i, j);
			uchar pixFilter = filter.at<uchar>(i, j);
			if (pixSrc < (pixFilter - 5))
			{
				src.at<uchar>(i, j) = 0; //灰点置黑
			}
			else
			{
				src.at<uchar>(i, j) = 255; //置白
			}
		}
	}

	imshow("bin", src);

	vector<KeyPoint> keys;
	SimpleBlobDetector::Params params;

	params.minArea = 100;
	params.maxArea = 400;

	params.filterByInertia = true;
	params.minConvexity = 0.15f;
	params.maxConvexity = 1000.00f;

	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
	detector->detect(src, keys);
	drawKeypoints(src, keys, src, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	cout << "blob size = " << keys.size() << endl;

	vector<Point> points;
	for (auto pot : keys)
	{
		points.push_back(pot.pt);
		cout << "x: " << pot.pt.x;
		cout << ", y: " << pot.pt.y;
		cout << ", count: " << pot.size << endl;

		int sz = (int)pot.size;
		if (sz < 15)
			continue;

		int x = pot.pt.x;
		int y = pot.pt.y;
		FlagBlob(srcImage, x - 10, x + 10, y - 10, y + 10);
	}

	cout << "effect blob count = " << points.size();

//	imshow("result", srcImage);
	imwrite("D:\\result.bmp", srcImage);

	showEffect();

	waitKey(0);
}

#if 0
void Widget::WindowResize(int width, int height)
{

}
#endif

void FlagBlob(Mat &input, int row0, int row1, int col0, int col1)
{
	rectangle(input, cvPoint(row0, col0), cvPoint(row1, col1), Scalar(0, 0, 255), 1, 1, 0);

#if 0
	for (int i = row0; i < row1; i++)
	{
		for (int j = col0; j < col1; j++)
		{
			if (i >= row0 + 1 && i < row1 - 1 && j >= col0 + 1 && j < col1 - 1)
				continue;
			else
				input.at<uchar>(i, j) = 0;
		}
	}
#endif
}


