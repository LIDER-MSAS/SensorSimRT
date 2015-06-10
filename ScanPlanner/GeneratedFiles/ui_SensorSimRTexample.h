/********************************************************************************
** Form generated from reading UI file 'SensorSimRTexample.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENSORSIMRTEXAMPLE_H
#define UI_SENSORSIMRTEXAMPLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_SensorSimRTexampleClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    qglWidget *widget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton_2;
    QCheckBox *checkBox;
    QLabel *label_6;
    QDoubleSpinBox *doubleSpinBox_5;
    QPushButton *pushButton_3;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QComboBox *comboBox;
    QGridLayout *gridLayout_2;
    QDoubleSpinBox *doubleSpinBox_2;
    QLabel *label_2;
    QLabel *label;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_3;
    QDoubleSpinBox *doubleSpinBox_4;
    QLabel *label_5;
    QDoubleSpinBox *doubleSpinBox;
    QPushButton *pushButton;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QSpinBox *spinBox_4;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QSpacerItem *verticalSpacer;

    void setupUi(QMainWindow *SensorSimRTexampleClass)
    {
        if (SensorSimRTexampleClass->objectName().isEmpty())
            SensorSimRTexampleClass->setObjectName(QStringLiteral("SensorSimRTexampleClass"));
        SensorSimRTexampleClass->resize(1072, 747);
        centralWidget = new QWidget(SensorSimRTexampleClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget = new qglWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);

        gridLayout->addWidget(widget, 0, 1, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        verticalLayout_2->addWidget(pushButton_2);

        checkBox = new QCheckBox(groupBox);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setChecked(true);

        verticalLayout_2->addWidget(checkBox);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout_2->addWidget(label_6);

        doubleSpinBox_5 = new QDoubleSpinBox(groupBox);
        doubleSpinBox_5->setObjectName(QStringLiteral("doubleSpinBox_5"));
        doubleSpinBox_5->setDecimals(6);
        doubleSpinBox_5->setMinimum(1e-06);
        doubleSpinBox_5->setMaximum(1e+09);
        doubleSpinBox_5->setValue(0.001);

        verticalLayout_2->addWidget(doubleSpinBox_5);

        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        verticalLayout_2->addWidget(pushButton_3);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        comboBox = new QComboBox(groupBox_2);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        verticalLayout_3->addWidget(comboBox);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        doubleSpinBox_2 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_2->setObjectName(QStringLiteral("doubleSpinBox_2"));
        doubleSpinBox_2->setDecimals(3);
        doubleSpinBox_2->setMinimum(-1e+09);
        doubleSpinBox_2->setMaximum(1e+09);
        doubleSpinBox_2->setSingleStep(0.1);

        gridLayout_2->addWidget(doubleSpinBox_2, 2, 2, 1, 1);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 2, 1, 1, 1);

        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 1, 1, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 3, 1, 1, 1);

        doubleSpinBox_3 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_3->setObjectName(QStringLiteral("doubleSpinBox_3"));
        doubleSpinBox_3->setDecimals(3);
        doubleSpinBox_3->setMinimum(-1e+09);
        doubleSpinBox_3->setMaximum(1e+09);
        doubleSpinBox_3->setSingleStep(0.1);

        gridLayout_2->addWidget(doubleSpinBox_3, 3, 2, 1, 1);

        doubleSpinBox_4 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_4->setObjectName(QStringLiteral("doubleSpinBox_4"));
        doubleSpinBox_4->setDecimals(3);
        doubleSpinBox_4->setMaximum(1);
        doubleSpinBox_4->setSingleStep(0.01);
        doubleSpinBox_4->setValue(0.012);

        gridLayout_2->addWidget(doubleSpinBox_4, 4, 2, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 4, 1, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setDecimals(3);
        doubleSpinBox->setMinimum(-1e+09);
        doubleSpinBox->setMaximum(1e+09);
        doubleSpinBox->setSingleStep(0.1);

        gridLayout_2->addWidget(doubleSpinBox, 1, 2, 1, 1);


        verticalLayout_3->addLayout(gridLayout_2);

        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout_3->addWidget(pushButton);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_4 = new QVBoxLayout(groupBox_3);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_4->addWidget(label_4);

        spinBox_4 = new QSpinBox(groupBox_3);
        spinBox_4->setObjectName(QStringLiteral("spinBox_4"));
        spinBox_4->setMinimum(1);
        spinBox_4->setMaximum(100);
        spinBox_4->setValue(5);

        verticalLayout_4->addWidget(spinBox_4);


        verticalLayout->addWidget(groupBox_3);

        checkBox_2 = new QCheckBox(centralWidget);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        verticalLayout->addWidget(checkBox_2);

        checkBox_3 = new QCheckBox(centralWidget);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setChecked(true);

        verticalLayout->addWidget(checkBox_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        SensorSimRTexampleClass->setCentralWidget(centralWidget);

        retranslateUi(SensorSimRTexampleClass);
        QObject::connect(spinBox_4, SIGNAL(valueChanged(int)), widget, SLOT(slotPointSize(int)));
        QObject::connect(doubleSpinBox, SIGNAL(valueChanged(double)), widget, SLOT(slotPosX(double)));
        QObject::connect(doubleSpinBox_2, SIGNAL(valueChanged(double)), widget, SLOT(slotPosY(double)));
        QObject::connect(doubleSpinBox_3, SIGNAL(valueChanged(double)), widget, SLOT(slotPosZ(double)));
        QObject::connect(doubleSpinBox_4, SIGNAL(valueChanged(double)), widget, SLOT(slotNoise(double)));
        QObject::connect(pushButton, SIGNAL(clicked()), widget, SLOT(slotSaveCloudASCII()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), widget, SLOT(slotLoadObj()));
        QObject::connect(checkBox, SIGNAL(clicked(bool)), widget, SLOT(slotZup(bool)));
        QObject::connect(doubleSpinBox_5, SIGNAL(valueChanged(double)), widget, SLOT(slotScale(double)));
        QObject::connect(pushButton_3, SIGNAL(clicked()), widget, SLOT(slotUpdateScene()));
        QObject::connect(comboBox, SIGNAL(currentIndexChanged(QString)), widget, SLOT(slotSensor(QString)));
        QObject::connect(checkBox_3, SIGNAL(clicked(bool)), widget, SLOT(slotTriangles(bool)));
        QObject::connect(checkBox_2, SIGNAL(clicked(bool)), widget, SLOT(slotPoints(bool)));

        QMetaObject::connectSlotsByName(SensorSimRTexampleClass);
    } // setupUi

    void retranslateUi(QMainWindow *SensorSimRTexampleClass)
    {
        SensorSimRTexampleClass->setWindowTitle(QApplication::translate("SensorSimRTexampleClass", "SensorSimRT GUI example", 0));
        groupBox->setTitle(QApplication::translate("SensorSimRTexampleClass", "Scene", 0));
        pushButton_2->setText(QApplication::translate("SensorSimRTexampleClass", "Load scene from .obj", 0));
        checkBox->setText(QApplication::translate("SensorSimRTexampleClass", "Z - up", 0));
        label_6->setText(QApplication::translate("SensorSimRTexampleClass", "Scale:", 0));
        pushButton_3->setText(QApplication::translate("SensorSimRTexampleClass", "Update scene", 0));
        groupBox_2->setTitle(QApplication::translate("SensorSimRTexampleClass", "Sensor", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("SensorSimRTexampleClass", "LMS100", 0)
         << QApplication::translate("SensorSimRTexampleClass", "Kinect 1", 0)
         << QApplication::translate("SensorSimRTexampleClass", "Kinect 2", 0)
         << QApplication::translate("SensorSimRTexampleClass", "Velodyne HDL-64E", 0)
         << QApplication::translate("SensorSimRTexampleClass", "Velodyne HDL-32E", 0)
         << QApplication::translate("SensorSimRTexampleClass", "Velodyne VPL-16", 0)
         << QApplication::translate("SensorSimRTexampleClass", "Mandala 3D Unit LMS100", 0)
         << QApplication::translate("SensorSimRTexampleClass", "ZF 5010", 0)
        );
        label_2->setText(QApplication::translate("SensorSimRTexampleClass", "Y pos", 0));
        label->setText(QApplication::translate("SensorSimRTexampleClass", "X pos", 0));
        label_3->setText(QApplication::translate("SensorSimRTexampleClass", "Z pos", 0));
        label_5->setText(QApplication::translate("SensorSimRTexampleClass", "noise", 0));
        pushButton->setText(QApplication::translate("SensorSimRTexampleClass", "Save cloud ASCII", 0));
        groupBox_3->setTitle(QApplication::translate("SensorSimRTexampleClass", "Viewer", 0));
        label_4->setText(QApplication::translate("SensorSimRTexampleClass", "PointSize:", 0));
        checkBox_2->setText(QApplication::translate("SensorSimRTexampleClass", "points", 0));
        checkBox_3->setText(QApplication::translate("SensorSimRTexampleClass", "triangles", 0));
    } // retranslateUi

};

namespace Ui {
    class SensorSimRTexampleClass: public Ui_SensorSimRTexampleClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENSORSIMRTEXAMPLE_H
