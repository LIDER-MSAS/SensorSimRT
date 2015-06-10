#ifndef __qglWidget__
#define __qglWidget__

#include <QGLWidget>
#include <QTimer>
#include <string>
#include <vector>
#include <QtGui>


#include "sensorsimrt.h"
using namespace std;



class qglWidget:public QGLWidget
{
	Q_OBJECT

public slots:
	void slotPointSize(int);
	void slotPosX(double);
	void slotPosY(double);
	void slotPosZ(double);
	void slotNoise(double);
	void slotSaveCloudASCII();
	void slotLoadObj();
	void slotZup(bool);
	void slotScale(double);
	void slotUpdateScene();
	void slotSensor(QString);
	void slotPoints(bool);
	void slotTriangles(bool);

signals:

private:

	bool showPoints;
	bool showTriangles;

	float translate_z;
    float rotate_x;
    float rotate_y;

	VirtualSensor *sensor;

	QPoint lastPos;
	int pointsize;
	
    void setupViewport(int width, int height);

	SensorSimRT s;

	float posx,posy,posz;

protected:
	void initializeGL();
	void paintGL();
    void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	
public:
	qglWidget(QWidget *parent);
	~qglWidget();
};


#endif
