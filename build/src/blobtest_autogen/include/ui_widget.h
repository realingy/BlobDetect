/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *pb;
    QLabel *label;
    QLabel *labelSrc;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(942, 649);
        pb = new QPushButton(Widget);
        pb->setObjectName(QString::fromUtf8("pb"));
        pb->setGeometry(QRect(380, 560, 93, 51));
        pb->setStyleSheet(QString::fromUtf8("background-color: rgb(71, 255, 123);"));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(190, 310, 451, 241));
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 217, 101);"));
        labelSrc = new QLabel(Widget);
        labelSrc->setObjectName(QString::fromUtf8("labelSrc"));
        labelSrc->setGeometry(QRect(200, 50, 431, 211));
        labelSrc->setStyleSheet(QString::fromUtf8("background-color: rgb(117, 255, 172);"));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        pb->setText(QCoreApplication::translate("Widget", "PushButton", nullptr));
        label->setText(QString());
        labelSrc->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
