#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include <QObject>
#include <QPushButton>
#include <QLayout>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <QFileDialog>
#include <QImage>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

#define CTG 4
#define CTY 4
#define CTR 4

enum BlobLevel { GLevel, YLevel, RLevel };

class Widget : public QWidget
{
	Q_OBJECT
public:
	explicit Widget(QWidget *parent = 0);
	~Widget();

	void showSrc();
	void showEffect();

public slots:
	void showDialog(); //显示文件对话框

private:
	void ImageProcess();
	void FlagBlob(int r0, int r1, int c0, int c1, BlobLevel level);
	void resetCount() {
		blobCountG_ = 0;
		blobCountY_ = 0;
		blobCountR_ = 0;
	}
	bool isChecked() {
		if(blobCountR_ > CTR || blobCountY_ > CTY || blobCountG_ > CTG)
			return true;
		return false;
	}


private:
	Mat src_;
	Mat mask_;
	int width_;
	int height_;
	QString path;
	
	int blobCountG_;
	int blobCountY_;
	int blobCountR_;
	
};

#endif //__WIDGET_H__
