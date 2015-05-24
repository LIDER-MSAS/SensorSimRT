#ifndef SensorSimRTexample_H
#define SensorSimRTexample_H

#include <QtWidgets/QMainWindow>
#include "ui_SensorSimRTexample.h"

class SensorSimRTexample : public QMainWindow
{
	Q_OBJECT

public:
	SensorSimRTexample(QWidget *parent = 0);
	~SensorSimRTexample();

private:
	Ui::SensorSimRTexampleClass ui;
};

#endif // SensorSimRTexample_H
