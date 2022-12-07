#include "widget.h"

// void FlagBlob(cv::Mat &input, int row0, int row1, int col0, int col1,
//   BlobLevel level);

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
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

Widget::~Widget() { delete ui; }

void Widget::showDialog() {
  QString dir;
  if (0 == path_.length()) {
    dir = "D:\\";
  } else {
    QFileInfo file(path_);
    dir = file.absoluteDir().absolutePath();
  }

  path_ = QFileDialog::getOpenFileName(this, "Please Open Image", dir,
                                       "Image File(*.png *.bmp *.jpg)");

  if (path_ == NULL) return;

  showSrc();

  ImageProcess();
}

void Widget::showSrc() {
  QImage img;
  img.load(path_);
  QImage newImg = img.scaled((this->width() - 40) / 2, (this->height() - 120));

  width_ = newImg.width();
  height_ = newImg.height();

  QPixmap pix = QPixmap::fromImage(newImg);
  ui->labelSrc->setPixmap(pix);
}

void Widget::showEffect() {
  QImage img;
  img.load("result.bmp");

  QImage newImg = img.scaled((this->width() - 40) / 2, (this->height() - 120));

  QPixmap pix = QPixmap::fromImage(newImg);
  ui->labelSrc->setPixmap(pix);
}

void Widget::ImageProcess() {
  std::string filePath = path_.toStdString();

  src_ = cv::imread(filePath);

  cv::Mat srcBck;
  cvtColor(src_, srcBck, cv::COLOR_RGB2GRAY);

  cvtColor(src_, mask_, cv::COLOR_RGB2GRAY);

  for (int i = 0; i < mask_.rows; i++) {
    for (int j = 0; j < mask_.cols; j++) {
      mask_.at<uchar>(i, j) = 255;
    }
  }

  resetCount();

  /*
  Mat srcImage, dstImage;
  string name = path_.toStdString();
  srcImage = imread(name);
  */

  // imshow("src", srcImage);

  cv::Mat src;
  srcBck.copyTo(src);

  //	cvtColor(srcImage, src, CV_RGB2GRAY);
  //	srcImage.copyTo(src); //����

  cv::Mat filter;
  boxFilter(src, filter, -1, cv::Size(120, 120));

  for (int i = 0; i < src.rows; i++) {
    for (int j = 0; j < src.rows; j++) {
      uchar pixSrc = src.at<uchar>(i, j);
      uchar pixFilter = filter.at<uchar>(i, j);
      if (pixSrc > (pixFilter + 10)) {
        src.at<uchar>(i, j) = pixFilter;  //ȥ���׵�
      }
    }
  }

  boxFilter(src, filter, -1, cv::Size(60, 60));

  for (int i = 0; i < src.rows; i++) {
    for (int j = 0; j < src.rows; j++) {
      uchar pixSrc = src.at<uchar>(i, j);
      uchar pixFilter = filter.at<uchar>(i, j);
      if (pixSrc < (pixFilter - 20)) {
        src.at<uchar>(i, j) = 0;  //�ҵ��ú�
      } else {
        src.at<uchar>(i, j) = 255;  //�ð�
      }
    }
  }

  cv::imshow("red", src);

  std::vector<cv::KeyPoint> keys;
  cv::SimpleBlobDetector::Params params;

  params.minDistBetweenBlobs = 2;
  params.blobColor = 0;

  //	params.minArea = 100;
  //	params.maxArea = 400;

  //	params.filterByInertia = true;
  params.minConvexity = 0.25f;
  //	params.maxConvexity = 1000.00f;

  cv::Ptr<cv::SimpleBlobDetector> detector =
      cv::SimpleBlobDetector::create(params);
  detector->detect(src, keys);
  //	drawKeypoints(src, keys, src, Scalar(255, 0, 0),
  // DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

  //	cout << "blob size = " << keys.size() << endl;

  cv::Mat srcImage;

  std::vector<cv::Point> points;
  for (auto pot : keys) {
    points.push_back(pot.pt);

    int sz = (int)pot.size;

    /*
    cout << "x: " << pot.pt.x;
    cout << ", y: " << pot.pt.y;
    cout << ", count: " << pot.size << endl;
    */

    // int sz = (int)pot.size;
    if (sz < 15) continue;

    int x = pot.pt.x;
    int y = pot.pt.y;

    FlagBlob(srcImage, x - 10, x + 10, y - 10, y + 10, RLevel);
  }

  srcBck.copyTo(src);

  // cv::Mat filter;
  boxFilter(src, filter, -1, cv::Size(120, 120));

  for (int i = 0; i < src.rows; i++) {
    for (int j = 0; j < src.rows; j++) {
      uchar pixSrc = src.at<uchar>(i, j);
      uchar pixFilter = filter.at<uchar>(i, j);
      if (pixSrc > (pixFilter + 10)) {
        src.at<uchar>(i, j) = pixFilter;
      }
    }
  }

  boxFilter(src, filter, -1, cv::Size(60, 60));

  for (int i = 0; i < src.rows; i++) {
    for (int j = 0; j < src.rows; j++) {
      uchar pixSrc = src.at<uchar>(i, j);
      uchar pixFilter = filter.at<uchar>(i, j);
      if (pixSrc <= (pixFilter - 11) && pixSrc >= (pixFilter - 20)) {
        src.at<uchar>(i, j) = 0;  //�ҵ��ú�
      } else {
        src.at<uchar>(i, j) = 255;  //�ð�
      }
    }
  }

  imshow("yellow", src);

  detector->detect(src, keys);
  //	drawKeypoints(src, keys, src, Scalar(255, 0, 0),
  // DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

  //	cout << "blob size = " << keys.size() << endl;

  points.clear();
  for (auto pot : keys) {
    points.push_back(pot.pt);

    int sz = (int)pot.size;

    // int sz = (int)pot.size;
    if (sz < 15) continue;

    int x = pot.pt.x;
    int y = pot.pt.y;

    FlagBlob(srcImage, x - 10, x + 10, y - 10, y + 10, YLevel);
  }

  srcBck.copyTo(src);

  //   cv::Mat filter;
  boxFilter(src, filter, -1, cv::Size(120, 120));

  for (int i = 0; i < src.rows; i++) {
    for (int j = 0; j < src.rows; j++) {
      uchar pixSrc = src.at<uchar>(i, j);
      uchar pixFilter = filter.at<uchar>(i, j);
      if (pixSrc > (pixFilter + 10)) {
        src.at<uchar>(i, j) = pixFilter;
      }
    }
  }

  boxFilter(src, filter, -1, cv::Size(60, 60));

  for (int i = 0; i < src.rows; i++) {
    for (int j = 0; j < src.rows; j++) {
      uchar pixSrc = src.at<uchar>(i, j);
      uchar pixFilter = filter.at<uchar>(i, j);
      if (pixSrc <= (pixFilter - 5) && pixSrc >= (pixFilter - 12)) {
        src.at<uchar>(i, j) = 0;
      } else {
        src.at<uchar>(i, j) = 255;
      }
    }
  }

  imshow("green", src);

  detector->detect(src, keys);
  //	drawKeypoints(src, keys, src, Scalar(255, 0, 0),
  // DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

  //	cout << "blob size = " << keys.size() << endl;

  points.clear();
  for (auto pot : keys) {
    points.push_back(pot.pt);

    int sz = (int)pot.size;

    // int sz = (int)pot.size;
    if (sz < 15) continue;

    int x = pot.pt.x;
    int y = pot.pt.y;

    FlagBlob(srcImage, x - 10, x + 10, y - 10, y + 10, GLevel);
  }

  //	cout << "effect blob count = " << points.size();

  //	imshow("result", srcImage);
  imwrite("result.bmp", src_);

  if (isChecked()) {
#if 0
                ui->label->setStyleSheet("\n"
			"font: 75 16pt \"\";\n"
			"color: red;\n"
			//"}\n"
		));
#endif

    QFileInfo fi(path_);
    QString name = fi.fileName();
    ui->label->setText(name + " Fail!");
  } else {
#if 0
                ui->label->setStyleSheet("\n"
			"font: 75 16pt \"\";\n"
			"color: green;\n"
			//"}\n"
		));
#endif

    QFileInfo fi(path_);
    QString name = fi.fileName();
    ui->label->setText(name + " Success!");
  }

  showEffect();

  cv::waitKey();
}

void Widget::FlagBlob(cv::Mat src, int row0, int row1, int col0, int col1,
                      BlobLevel level) {
#ifdef DELDUP
  uchar pixMask;
  for (int i = row0; i < row1; i++) {
    for (int j = col0; j < col1; j++) {
      pixMask = mask_.at<uchar>(i, j);
      if (0 == pixMask) return;
    }
  }
#endif

  if (BlobLevel::GLevel == level) {
    rectangle(src_, cv::Point(row0, col0), cv::Point(row1, col1),
              cv::Scalar(0, 255, 0), 1, 1, 0);  //
    blobCountG_++;
  } else if (BlobLevel::YLevel == level) {
    rectangle(src_, cv::Point(row0, col0), cv::Point(row1, col1),
              cv::Scalar(0, 255, 255), 1, 1, 0);  //
    blobCountY_++;
  } else if (BlobLevel::RLevel == level) {
    rectangle(src_, cv::Point(row0, col0), cv::Point(row1, col1),
              cv::Scalar(0, 0, 255), 1, 1, 0);  //
    blobCountR_++;
  }

#ifdef DELDUP
  uchar pixMask;
  for (int i = row0; i < row1; i++) {
    for (int j = col0; j < col1; j++) {
      mask_.at<uchar>(i, j) = 0;
    }
  }
#endif
}

// void FlagBlob(cv::Mat &input, int row0, int row1, int col0, int col1) {
void FlagBlob(cv::Mat &input, int row0, int row1, int col0, int col1,
              BlobLevel level) {
  rectangle(input, cv::Point(row0, col0), cv::Point(row1, col1),
            cv::Scalar(0, 0, 255), 1, 1, 0);

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
