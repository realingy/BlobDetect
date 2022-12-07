#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QLayout>
#include <QObject>
#include <QPixmap>
#include <QPushButton>
#include <QWidget>
#include <iostream>
#include <vector>

#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ui_widget.h"

using namespace std;

#define CTG 4
#define CTY 4
#define CTR 4

enum BlobLevel { GLevel, YLevel, RLevel };

class Widget : public QWidget {
  Q_OBJECT
 public:
  explicit Widget(QWidget *parent = 0);
  ~Widget();

  void showSrc();
  void showEffect();

 public slots:
  void showDialog();  //��ʾ�ļ��Ի���

 private:
  void ImageProcess();
  void FlagBlob(cv::Mat src, int r0, int r1, int c0, int c1, BlobLevel level);
  void resetCount() {
    blobCountG_ = 0;
    blobCountY_ = 0;
    blobCountR_ = 0;
  }
  bool isChecked() {
    if (blobCountR_ > CTR || blobCountY_ > CTY || blobCountG_ > CTG)
      return true;
    return false;
  }

 private:
  cv::Mat src_;
  cv::Mat mask_;
  int width_;
  int height_;
  QString path_;

  int blobCountG_;
  int blobCountY_;
  int blobCountR_;

 private:
  Ui::Widget *ui;
};

#endif  //__WIDGET_H__
