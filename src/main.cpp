#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Widget *widget = new Widget;

	QPushButton button("Open Image", widget);
	button.move(20, 5);
	QObject::connect(&button, SIGNAL(clicked()), widget, SLOT(showDialog()));
	
	widget->resize(1000, 600);
	widget->show();

	return app.exec();
}


