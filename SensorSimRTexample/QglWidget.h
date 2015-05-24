#ifndef __qglWidget__
#define __qglWidget__

#include <QGLWidget>
#include <QTimer>
#include <QOpenGLFunctions_3_3_Core>
#include <string>
#include <vector>
#include <QtGui>

#include "GLSLManager.h"
#include "shaders.h"

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

signals:

private:
	GLSLManager *glsl;

	QOpenGLFunctions_3_3_Core *glReady;
	GLuint VAOid_M;
	GLuint VAOid_x;
	
	GLuint VBOid_x;
	GLuint VBOid_M;

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


	//bool w;
};


#endif
