#include "widget.h"


void FlagBlob(Mat &input, int row0, int row1, int col0, int col1);

Widget::Widget(QWidget *parent)
	: QWidget(parent)
{
	setWindowTitle("BloackPointCheck");

	/*
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(&label1);
	layout->addWidget(&label2);

	setLayout(layout);
	*/
	
	connect(ui->pb, SIGNAL(clicked()), SLOT(showDialog()));
	
	ui->label->setText(" ");
	ui->label->setAlignment(Qt::AlignCenter);
	
}

Widget::~Widget()
{
	delete ui;
}

//显示文件对话框
void Widget::showDialog()
{
	QString dir;
	if(0 == path.length())
	{
		dir = "D:\\";
	}
	else
	{
		QFileInfo fi(path);
		dir = dir.absoluteDir().absolutePath();
	}
	
	path = QFileDialog::getOpenFileName(this, "Please Open Image", dir, "Image File(*.png *.bmp *.jpg)");
	
	if (path == NULL)
		return;

	showSrc();

	ImageProcess();
}

void Widget::showSrc()
{
	QImage img;
	img.load(path); //输入图片
	QImage newImg = img.scaled( (this->width() - 40)/2, (this->height() - 120) );
	
	width_ = newImg.width();
	height_ = newImg.height();
	
	QPixmap pix = QPixmap::fromImage(newImg);
	ui->labelSrc->setPixmap(pix);
}

void Widget::showEffect()
{
	QImage img;
	img.load("result.bmp");

	QImage newImg = img.scaled( (this->width() - 40)/2, (this->height() - 120) );
	
	QPixmap pix = QPixmap::fromImage(newImg);
	ui->labelSrc->setPixmap(pix);
}

void Widget::ImageProcess()
{
	QString filePath = path.toStdString();
	
	src_ = imread(filePath); //读入源图片
	
	Mat srcBck;
	cvtColor(src_, srcBck, CV_RGB2GRAY);
	
	cvtColor(src_, mask_, CV_RGB2GRAY);
	for(int i = 0; i < mask_.rows; i++)
	{
		for(int j = 0; j < mask_.cols; j++)
		{
			mask_.at<uchar>(i, j) = 255;
		}
	}
	
	resetCount();
	
	/*
	Mat srcImage, dstImage;
	string name = path.toStdString();
	srcImage = imread(name);
	*/
	
//	imshow("src", srcImage);

	//【1、标红色】
	Mat src;
	srcBck.copyTo(src);
	
//	cvtColor(srcImage, src, CV_RGB2GRAY);
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
			if (pixSrc < (pixFilter - 20))
			{
				src.at<uchar>(i, j) = 0; //灰点置黑
			}
			else
			{
				src.at<uchar>(i, j) = 255; //置白
			}
		}
	}

	imshow("red", src);

	vector<KeyPoint> keys;
	SimpleBlobDetector::Params params;

	params.minDisBetweenBlobs = 2;
	params.blobColor = 0;
	
//	params.minArea = 100;
//	params.maxArea = 400;

//	params.filterByInertia = true;
	params.minConvexity = 0.25f;
//	params.maxConvexity = 1000.00f;

	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
	detector->detect(src, keys);
//	drawKeypoints(src, keys, src, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

//	cout << "blob size = " << keys.size() << endl;

	vector<Point> points;
	for (auto pot : keys)
	{
		points.push_back(pot.pt);
		
		int sz = (int)pot.size;
		
		/*
		cout << "x: " << pot.pt.x;
		cout << ", y: " << pot.pt.y;
		cout << ", count: " << pot.size << endl;
		*/
		
		int sz = (int)pot.size;
		if (sz < 15)
			continue;

		int x = pot.pt.x;
		int y = pot.pt.y;
		
		FlagBlob(srcImage, x - 10, x + 10, y - 10, y + 10, RLevel);
	}

	//【2、标黄色】
	srcBck.copyTo(src);

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
			if (pixSrc <= (pixFilter - 11) && pixSrc >= (pixFilter - 20))
			{
				src.at<uchar>(i, j) = 0; //灰点置黑
			}
			else
			{
				src.at<uchar>(i, j) = 255; //置白
			}
		}
	}

	imshow("yellow", src);

	detector->detect(src, keys);
//	drawKeypoints(src, keys, src, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

//	cout << "blob size = " << keys.size() << endl;

	points.clear();
	for (auto pot : keys)
	{
		points.push_back(pot.pt);
		
		int sz = (int)pot.size;
		
		int sz = (int)pot.size;
		if (sz < 15)
			continue;

		int x = pot.pt.x;
		int y = pot.pt.y;
		
		FlagBlob(srcImage, x - 10, x + 10, y - 10, y + 10, YLevel);
	}

	//【3、标绿色】
	srcBck.copyTo(src);

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
			if (pixSrc <= (pixFilter - 5) && pixSrc >= (pixFilter - 12))
			{
				src.at<uchar>(i, j) = 0; //灰点置黑
			}
			else
			{
				src.at<uchar>(i, j) = 255; //置白
			}
		}
	}

	imshow("green", src);

	detector->detect(src, keys);
//	drawKeypoints(src, keys, src, Scalar(255, 0, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

//	cout << "blob size = " << keys.size() << endl;

	points.clear();
	for (auto pot : keys)
	{
		points.push_back(pot.pt);
		
		int sz = (int)pot.size;
		
		int sz = (int)pot.size;
		if (sz < 15)
			continue;

		int x = pot.pt.x;
		int y = pot.pt.y;
		
		FlagBlob(srcImage, x - 10, x + 10, y - 10, y + 10, GLevel);
	}
	
	
	
	
//	cout << "effect blob count = " << points.size();

//	imshow("result", srcImage);
	imwrite("result.bmp", src_);
	
	if(isChecked())
	{
		ui->label->setStyleSheet("\n"
			"font: 75 16pt \"微软雅黑\";\n"
			"color: red;\n"
			//"}\n"
		));
		
		QFileInfo fi(path);
		QString name = fi.fileName();
		ui->label->setText(name + " Fail!");
	}
	else
	{
		ui->label->setStyleSheet("\n"
			"font: 75 16pt \"微软雅黑\";\n"
			"color: green;\n"
			//"}\n"
		));
		
		QFileInfo fi(path);
		QString name = fi.fileName();
		ui->label->setText(name + " Success!");

	}
	
	//【5、显示效果图】
	showEffect();

	waitKey(0);
}

#if 0
void Widget::WindowResize(int width, int height)
{

}
#endif

void Widget::FlagBlob(int row0, int row1, int col0, int col1, BlobLevel level)
{
#ifdef DELDUP
	uchar pixMask;
	for (int i = row0; i < row1; i++)
	{
		for (int j = col0; j < col1; j++)
		{
			pixMask = mask_.at<uchar>(i,j);
			if(0 == pixMask)
				return;
		}
	}
#endif

	if(BlobLevel::GreenLevel == level) {
		rectangle(src_, cvPoint(row0, col0), cvPoint(row1, col1), Scalar(0, 255, 0), 1, 1, 0); //
		blobCountG++;
	}
	else if(BlobLevel::YLevel == level) {
		rectangle(src_, cvPoint(row0, col0), cvPoint(row1, col1), Scalar(0, 255, 255), 1, 1, 0); //
		blobCountY++;
	}
	else if(BlobLevel::RLevel == level) {
		rectangle(src_, cvPoint(row0, col0), cvPoint(row1, col1), Scalar(0, 0, 255), 1, 1, 0); //
		blobCountR++;
	}

#ifdef DELDUP
	uchar pixMask;
	for (int i = row0; i < row1; i++)
	{
		for (int j = col0; j < col1; j++)
		{
			mask_.at<uchar>(i,j) = 0;
		}
	}
#endif
	
}


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


