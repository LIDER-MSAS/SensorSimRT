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

	translate_z=-66;
	rotate_x=-64;
	rotate_y=212;

	showPoints=0;
	showTriangles=1;

	posx=posy=posz=0;

	sensor=0;
}
qglWidget::~qglWidget()
{

}
void qglWidget::initializeGL()
{
	glEnable(GL_DEPTH_TEST);


	glPushMatrix();
	glLoadIdentity();


	glPointSize(pointsize);

	glEnable(GL_DEPTH_TEST);






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

	glMatrixMode(GL_MODELVIEW);
}
void qglWidget::paintGL()
{
	unsigned int num_rays=sensor->num_rays;
	float *data=new float[num_rays*3];

	float *color_data=new float[num_rays];

	int *hits=new int[s.scene->num_tris];

	for(int i=0;i<s.scene->num_tris;++i)
	{
		hits[i]=0;
	}


	for(int i=0;i<num_rays;++i)
	{
		color_data[i]=sensor->intensity[i];
	}


	s.Simulate();

	//sensor->GetPointCloud(data);


	for(int i=0;i<sensor->num_rays;++i)
	{
		if(s.results[i].prim_id>0)
		{
			int ida=s.scene->indices[3*s.results[i].prim_id+0];
			int idb=s.scene->indices[3*s.results[i].prim_id+1];
			int idc=s.scene->indices[3*s.results[i].prim_id+2];

			double u[3]={s.scene->verts[3*ida+0]-s.scene->verts[3*idc+0],s.scene->verts[3*ida+1]-s.scene->verts[3*idc+1],s.scene->verts[3*ida+2]-s.scene->verts[3*idc+2]};
			double v[3]={s.scene->verts[3*idb+0]-s.scene->verts[3*idc+0],s.scene->verts[3*idb+1]-s.scene->verts[3*idc+1],s.scene->verts[3*idb+2]-s.scene->verts[3*idc+2]};

			data[3*i+0]=s.scene->verts[3*idc+0] + s.barycentric[i].x*u[0] + s.barycentric[i].y*v[0];
			data[3*i+1]=s.scene->verts[3*idc+1] + s.barycentric[i].x*u[1] + s.barycentric[i].y*v[1];
			data[3*i+2]=s.scene->verts[3*idc+2] + s.barycentric[i].x*u[2] + s.barycentric[i].y*v[2];

			++hits[s.results[i].prim_id];
		}
		else
		{
			data[3*i+0]=data[3*i+1]=data[3*i+2]=0;
		}

	}


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	makeCurrent();


	glPointSize(pointsize);


	glClearColor(1.0, 1.0, 1, 1.0);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glTranslatef(0.0, 0.0, translate_z);
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 0.0, 1.0);



	glDisable(GL_TEXTURE_2D); 

	if(showPoints)
	{
		glBegin(GL_POINTS);
		for(int i=0;i<num_rays;++i)
		{
			glColor3f(color_data[i],color_data[i],color_data[i]);
			glVertex3fv(&data[3*i+0]);
		}
		glEnd();
	}

	glBegin(GL_LINES);
	glColor3f(1,0,0);
	glVertex3f(0,0,0);
	glVertex3f(1,0,0);
	glColor3f(0,1,0);
	glVertex3f(0,0,0);
	glVertex3f(0,1,0);
	glColor3f(0,0,1);
	glVertex3f(0,0,0);
	glVertex3f(0,0,1);
	glEnd();

	if(showTriangles)
	{
		for(int i=0;i<s.scene->num_tris;++i)
		{
			if(hits[i])
			{
				glColor3f(.9,.9,.9);

				glBegin(GL_TRIANGLES);
				glVertex3fv(&s.scene->verts[3*s.scene->indices[3*i+0]]);
				glVertex3fv(&s.scene->verts[3*s.scene->indices[3*i+1]]);
				glVertex3fv(&s.scene->verts[3*s.scene->indices[3*i+2]]);
				glEnd();
			}
			else
			{
				glColor3f(0.5,0,0);

				glBegin(GL_LINES);
				glVertex3fv(&s.scene->verts[3*s.scene->indices[3*i+0]]);
				glVertex3fv(&s.scene->verts[3*s.scene->indices[3*i+1]]);

				glVertex3fv(&s.scene->verts[3*s.scene->indices[3*i+1]]);
				glVertex3fv(&s.scene->verts[3*s.scene->indices[3*i+2]]);

				glVertex3fv(&s.scene->verts[3*s.scene->indices[3*i+2]]);
				glVertex3fv(&s.scene->verts[3*s.scene->indices[3*i+0]]);
				glEnd();
			}
		}
	}


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
void qglWidget::slotPoints(bool a)
{
	showPoints=a;
}
void qglWidget::slotTriangles(bool a)
{
	showTriangles=a;
}
