#include <QtWidgets/QFileDialog>
#include <QtGUI/QMouseEvent>
#include <QtWidgets/QApplication>
#include <QtCore/QTimer>
#define NOMINMAX
#include <Windows.h>
//#include <QtCore/QObject>
//#include <QtGUI/Qimage>
//#include <QtCore/QString>
#include <string>
#include <fstream>
#include <deque>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

#include "QglWidget.h"

#include <gl\GL.h>
#include <gl\glu.h>


#include "M3DLMS100.h"
#include "ZF5010.h"


using namespace std;



qglWidget::qglWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	QTimer *timer=new QTimer(this);
	
    timer->setSingleShot(false);

	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(10);
	
    setAutoFillBackground(false);
	
	pointsize=5;

	glReady=new QOpenGLFunctions_3_3_Core();

	glsl=new GLSLManager(glReady);
	
    translate_z=-66;
    rotate_x=-64;
    rotate_y=212;

	VAOid_points=0;
	VBOid_points=0;

	posx=posy=posz=0;

	sensor=0;
}
qglWidget::~qglWidget()
{

}
void qglWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	glReady->initializeOpenGLFunctions();

	glsl->LoadFromConstChar(vertexSource,fragmentSource);

	glPushMatrix();
	glLoadIdentity();
	
	GLfloat modelViewMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
	
	glsl->UniformMatrix4fv(modelViewMatrix,"modelViewMatrix");
	
	glPopMatrix();

	glPointSize(pointsize);

    glEnable(GL_DEPTH_TEST);


	//Unit Axes
	float axes[18]={0,0,0,
				1,0,0,
				0,0,0,
				0,1,0,
				0,0,0,
				0,0,1};
	float color[6]={0,0.25,0,0.5,0,0.75};
	
	glReady->glGenVertexArrays(1, &VAOid_x);
	glReady->glBindVertexArray(VAOid_x);
	glReady->glGenBuffers(1, &VBOid_x);
	glReady->glGenBuffers(1, &VBOid_c);
	
	glReady->glEnableVertexAttribArray(0);
	glReady->glBindBuffer(GL_ARRAY_BUFFER, VBOid_x);
	glReady->glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), axes, GL_STATIC_DRAW);
	glReady->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glReady->glEnableVertexAttribArray(1);
	glReady->glBindBuffer(GL_ARRAY_BUFFER, VBOid_c);
	glReady->glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), color, GL_STATIC_DRAW);
	glReady->glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);

	glReady->glBindVertexArray(0);


	


	s.LoadNewSceneObj("scenes\\imm.obj");
	sensor=new M3DLMS100;
	s.SetSensor(sensor);
	s.InitSim();
}
void qglWidget::setupViewport(int width, int height)
{
    int side = qMin(width, height);
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0,(GLdouble)width/(GLdouble)height,0.1,10000.0);
	
	GLfloat projectionMatrix[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
	
	glsl->UniformMatrix4fv(projectionMatrix,"projectionMatrix");

    glMatrixMode(GL_MODELVIEW);
}
void qglWidget::paintGL()
{
	unsigned int num_rays=sensor->num_rays;
	float *data=new float[num_rays*3];
	
	float *color_data=new float[num_rays];
	
	for(int i=0;i<num_rays;++i)
	{
		color_data[i]=sensor->intensity[i];
	}


	s.Simulate();

	sensor->GetPointCloud(data);




	
	if(!VAOid_points)
		glReady->glGenVertexArrays(1, &VAOid_points);
	
	glReady->glBindVertexArray(VAOid_points);
	
	if(!VBOid_points)
		glReady->glGenBuffers(1, &VBOid_points);
	
	glReady->glBindBuffer(GL_ARRAY_BUFFER, VBOid_points);
	glReady->glBufferData(GL_ARRAY_BUFFER, num_rays * 3 * sizeof(float), data, GL_STATIC_DRAW);
	
	if(!VBOid_color)
		glReady->glGenBuffers(1, &VBOid_color);
	
	glReady->glBindBuffer(GL_ARRAY_BUFFER, VBOid_color);
	glReady->glBufferData(GL_ARRAY_BUFFER, num_rays * sizeof(float), color_data, GL_STATIC_DRAW);

	glReady->glEnableVertexAttribArray(0);
	glReady->glBindBuffer(GL_ARRAY_BUFFER, VBOid_points);
	glReady->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,(void*)0);
	
	// 2nd attribute buffer : colors
	glReady->glEnableVertexAttribArray(1);
	glReady->glBindBuffer(GL_ARRAY_BUFFER, VBOid_color);
	glReady->glVertexAttribPointer(1,1,GL_FLOAT,GL_FALSE,0,(void*)0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
    makeCurrent();
	
	
	glPointSize(pointsize);
	
	
	glClearColor(1.0, 1.0, 1, 1.0);
	
	
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	
	glTranslatef(0.0, 0.0, translate_z);
    glRotatef(rotate_x, 1.0, 0.0, 0.0);
    glRotatef(rotate_y, 0.0, 0.0, 1.0);
	
	
	GLfloat modelViewMatrix[16];
	
	
	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
	glsl->UniformMatrix4fv(modelViewMatrix,"modelViewMatrix");
	
	
	glDisable(GL_TEXTURE_2D); 
	
	
	glsl->UseProgram1();
	
	
	
	
	
	glReady->glBindVertexArray(VAOid_points);
	glDrawArrays(GL_POINTS,0,num_rays);
	
	
	


	glReady->glBindVertexArray(VAOid_x);
	glDrawArrays(GL_LINES,0,6);
	
	
	glReady->glBindVertexArray(0); 

	delete[] data;
	delete[] color_data;
	
}
void qglWidget::resizeGL(int width, int height)
{
    setupViewport(width, height);
}

void qglWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos=event->pos();

}
void qglWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx=event->x()-lastPos.x();
    int dy=event->y()-lastPos.y();
 
    if (event->buttons() & Qt::LeftButton)
    {
		rotate_x+=dy*0.2f;
		rotate_y+=dx*0.2f;
	}
    else
    if(event->buttons() & Qt::RightButton)
    {
        if(fabs(translate_z)>1)
			translate_z+=dy*0.01f*fabs(translate_z);
        else
			translate_z+=dy*0.01f;
    }
 
    lastPos=event->pos();
}
void qglWidget::slotPointSize(int a)
{
	pointsize=a;
	glPointSize(a);
}

void qglWidget::slotPosX(double a)
{
	posx=a;
	sensor->SetPosition(posx,posy,posz);
}

void qglWidget::slotPosY(double a)
{
	posy=a;
	sensor->SetPosition(posx,posy,posz);
}

void qglWidget::slotPosZ(double a)
{
	posz=a;
	sensor->SetPosition(posx,posy,posz);
}
void  qglWidget::slotNoise(double a)
{
	sensor->SetNoise(a);
}
void  qglWidget::slotSaveCloudASCII()
{
	QString filename=QFileDialog::getSaveFileName(this,"Save ASCII point cloud");
	if(filename.size())
	{
		FILE *f=fopen(filename.toStdString().c_str(),"w");
		if(f)
		{
			float *data=new float [sensor->num_rays*3];
			sensor->GetPointCloud(data);
			int counter=0;
			for(int i=0;i<sensor->num_rays;++i)
			{
				float x=data[counter++];
				float y=data[counter++];
				float z=data[counter++];
				if(x!=0 && y!=0 && z!=0)
					fprintf(f,"%f %f %f\n",x,y,z);
			}
			fclose(f);
			delete[] data;
		}

	}

}

void  qglWidget::slotLoadObj()
{
	QString filename=QFileDialog::getOpenFileName(this,"Open obj scene","","*.obj(*.obj)");
	if(filename.size())
	{
		s.LoadNewSceneObj(filename.toStdString().c_str());
	}

}

void qglWidget::slotZup(bool a)
{
	s.SetSceneUpAxisToZ(a);
}

void qglWidget::slotScale(double a)
{
	s.SetSceneScale(a);
}

void qglWidget::slotUpdateScene()
{
	s.UpdateOptixScene();
}

void qglWidget::slotSensor(QString s)
{
	if(s=="Mandala 3D Unit LMS100")
	{
		sensor=new M3DLMS100;
		sensor->SetPosition(posx,posy,posz);
		this->s.SetSensor(sensor);
	}
	if(s=="ZF 5010")
	{
		sensor=new ZF5010;
		sensor->SetPosition(posx,posy,posz);
		this->s.SetSensor(sensor);
	}

}
