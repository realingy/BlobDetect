#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include <QObject>
#include <QPushButton>
#include <QLayout>

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
//	void WindowResize(int width, int height);

private:
	QLabel label1;
	QLabel label2;
	QPixmap pix1;
	QPixmap pix2;
	QString path;
};

#endif //__WIDGET_H__
