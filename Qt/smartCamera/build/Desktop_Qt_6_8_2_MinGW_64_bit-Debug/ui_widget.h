/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
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
    QLabel *label;
    QPushButton *upButton;
    QPushButton *rightButton;
    QPushButton *leftButton;
    QPushButton *downButton;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(419, 342);
        label = new QLabel(Widget);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 30, 251, 261));
        upButton = new QPushButton(Widget);
        upButton->setObjectName("upButton");
        upButton->setGeometry(QRect(330, 90, 41, 41));
        rightButton = new QPushButton(Widget);
        rightButton->setObjectName("rightButton");
        rightButton->setGeometry(QRect(370, 130, 41, 41));
        leftButton = new QPushButton(Widget);
        leftButton->setObjectName("leftButton");
        leftButton->setGeometry(QRect(290, 130, 41, 41));
        downButton = new QPushButton(Widget);
        downButton->setObjectName("downButton");
        downButton->setGeometry(QRect(330, 170, 41, 41));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QString());
        upButton->setText(QCoreApplication::translate("Widget", "\344\270\212", nullptr));
        rightButton->setText(QCoreApplication::translate("Widget", "\345\217\263", nullptr));
        leftButton->setText(QCoreApplication::translate("Widget", "\345\267\246", nullptr));
        downButton->setText(QCoreApplication::translate("Widget", "\344\270\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
