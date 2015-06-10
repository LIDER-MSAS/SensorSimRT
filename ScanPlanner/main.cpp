#include "SensorSimRTexample.h"
#include <ctime>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	srand(time(0));

	QApplication a(argc, argv);
	SensorSimRTexample w;
	w.show();
	return a.exec();
}
