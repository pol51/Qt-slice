#include "qSlice.h"

#include <QGridLayout>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

// contructeur
QSlice::QSlice(QWidget *parent)
	: QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DoubleBuffer), parent)
{
	// valeurs par defaut
	_valueMin = 12.;
	_valueMax = 120.;
	this->setCurrentMin(16);
	this->setCurrentMax(25);
	_cursorSize = 8.;
	_mouseX	= 0.;
	_moved = 0;
	_onMin = false;
	_onMax = false;
	
	// suivi des déplacement de la souris
	this->setMouseTracking(true);
}

// destructeur
QSlice::~QSlice()
{
}

// taille par defaut
QSize QSlice::sizeHint() const
{
    return QSize(480, 40);
}

// getters
int QSlice::currentMin()
{
	return qRound(_currentMin + _valueMin);
}

int QSlice::currentMax()
{
	return qRound(_currentMax + _valueMin);
}

int QSlice::valueMin()
{
	return (int)_valueMax;
}

int QSlice::valueMax()
{
	return (int)_valueMax;
}

//setters
void QSlice::setCurrentMin(int currentMin)
{
	double newCurrentMin = (double)currentMin - _valueMin;
	
	if (newCurrentMin >= 0)
		_currentMin = newCurrentMin;
		
	if (newCurrentMin > _currentMax)
		_currentMax = newCurrentMin;
	
	this->update();
}

void QSlice::setCurrentMax(int currentMax)
{
	double newCurrentMax = (double)currentMax - _valueMin;
	
	if (newCurrentMax  <= _valueMax)
		_currentMax = newCurrentMax ;
		
	if (newCurrentMax  < _currentMin)
		_currentMin = newCurrentMax ;
	
	this->update();
}
	
void QSlice::setValueMin(int valueMin)
{
	if (valueMin < _valueMax)
		_valueMin = valueMin;

	if (_currentMin < _valueMin)
		_currentMin = _valueMin;
	
	this->update();
}

void QSlice::setValueMax(int valueMax)
{
	if (valueMax > _valueMin)
		_valueMax = valueMax;

	if (_currentMax > _valueMax)
		_currentMax = _valueMax;
	
	this->update();
}

void QSlice::initializeGL()
{
}

void QSlice::paintGL()
{
	if (_onMax || _onMin)
		setCursor(Qt::SizeHorCursor);
	else
		setCursor(Qt::ArrowCursor);
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	
	glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	
	resizeGL(width(), height());
	
	
	double minX = (_currentMin / (_valueMax - _valueMin) * 100.);
	double maxX = (_currentMax / (_valueMax - _valueMin) * 100.);
	
	// fond
	QColor color = this->palette().color(QPalette::Window);
	QColor colorBG = color.dark(110);
	glClearColor(color.redF(), color.greenF(), color.blueF(), 0.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	
	glColor3f(colorBG.redF(), colorBG.greenF(), colorBG.blueF());
	glBegin(GL_QUADS);
		glVertex3f(100,  0.5, -0.5);
		glVertex3f(100, -0.5, -0.5);
		glVertex3f(0  , -0.5, -0.5);
		glVertex3f(0  ,  0.5, -0.5);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(99.995,  0.495, -0.25);
		glVertex3f(99.995, -0.495, -0.25);
		glVertex3f( 0.005, -0.495, -0.25);
		glVertex3f( 0.005,  0.495, -0.25);
	glEnd();
	
	// dessin des zones non-selectionnables (bordures) avec la couleur de fond
	/*glColor3f(color.redF(), color.greenF(), color.blueF());
	glBegin(GL_QUADS);
		glVertex3f(- _cursorSize, 0.5, -0.3);
		glVertex3f(0, 0.5, -0.3);
		glVertex3f(0, -0.5, -0.3);
		glVertex3f(- _cursorSize, -0.5, -0.3);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f(100, 0.5, -0.3);
		glVertex3f(100 + _cursorSize, 0.5, -0.3);
		glVertex3f(100 + _cursorSize, -0.5, -0.3);
		glVertex3f(100, -0.5, -0.3);
	glEnd();
	*/
	// dessin de la selection active
	QColor colorFG = color.dark(95);
	glColor3f(colorFG.redF(), colorFG.greenF(), colorFG.blueF());
	glBegin(GL_QUADS);
		glVertex3f(maxX, 0.5, -0.3);
		glVertex3f(maxX, -0.5, -0.3);
		glVertex3f(minX, -0.5, -0.3);
		glVertex3f(minX, 0.5, -0.3);
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(maxX -0.005,  0.495, -0.2);
		glVertex3f(maxX -0.005, -0.495, -0.2);
		glVertex3f(minX -0.005, -0.495, -0.2);
		glVertex3f(minX -0.005,  0.495, -0.2);
	glEnd();
	
	// dessin des curseurs
	this->drawCursor(minX, 1);
	this->drawCursor(maxX, 2);
	
	glFlush();
	
	glPopAttrib();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void QSlice::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(
		-(_cursorSize / 2.0), 100.0 + (_cursorSize / 2.0),	// X
		0.5, -0.5,	// Y
		-1.0, 1.0);	// Z
	glMatrixMode(GL_MODELVIEW);
}

void QSlice::paintEvent(QPaintEvent */*event*/)
{
	QPainter painter;
	painter.begin(this);
	
	this->paintGL();
	painter.setRenderHint(QPainter::Antialiasing);
	
	// overpainting: c'est possible ici
	// (? mettre des HUD sur les curseurs lorsqu'on les déplacent ?)
	
	painter.end();
}

void QSlice::mousePressEvent(QMouseEvent *event)
{
 	if (event->buttons() & Qt::LeftButton)
		_mouseX = (float)(event->x()) /
					(float)this->width() *
					(100.0 + _cursorSize);
	_moved = 0;
	this->update();
}

void QSlice::mouseReleaseEvent(QMouseEvent *event)
{
	_moved = 0;
	this->mouseMoveEvent(event);
	this->update();
}

void QSlice::mouseMoveEvent(QMouseEvent *event)
{
	float mouseX =	(float)(event->x()) /
					(float)this->width() *
					(100.0 + _cursorSize);
	float minX = (_currentMin / (_valueMax - _valueMin) * 100.);
	float maxX = (_currentMax / (_valueMax - _valueMin) * 100.);
	_onMin = (((mouseX > (minX - (_cursorSize * 0.0))) &&
				(mouseX < (minX + (_cursorSize / 1.0)))) ||
				((_mouseX > (minX - (_cursorSize * 0.0))) &&
				(_mouseX < (minX + (_cursorSize / 1.0)))))	// curseur au dessus
				&& (_moved != 2)	// pas en train de bouger l'autre
				&& !_onMax;			// pas au-dessus de l'autre
	_onMax = (((mouseX > (maxX - (_cursorSize * 0.0))) &&
				(mouseX < (maxX + (_cursorSize / 1.0)))) ||
				((mouseX > (maxX - (_cursorSize * 0.0))) &&
				(mouseX < (maxX + (_cursorSize / 1.0)))))	// curseur au dessus
				&& (_moved != 1)	// pas en train de bouger l'autre
				&& !_onMin;			// pas au dessus de l'autre
	
	if (event->buttons() & Qt::LeftButton)
	{
		if ((_moved != 2) && _onMin)
		{
			minX += mouseX - _mouseX;
			if (minX < 0)	minX = 0;
			if (minX > 100) minX = 100;
			_currentMin = (minX * (_valueMax - _valueMin) / 100.);
			if (_currentMax < _currentMin)
				_currentMax = _currentMin;
			_moved = 1;
			emit minChanged(this->currentMin());
		}
		if ((_moved != 1) && _onMax)
		{	
			maxX += mouseX - _mouseX;
			if (maxX < 0)	maxX = 0;
			if (maxX > 100) maxX = 100;
			_currentMax = (maxX * (_valueMax - _valueMin) / 100.);
			if (_currentMin > _currentMax)
				_currentMin = _currentMax;
			_moved = 2;
			emit maxChanged(this->currentMax());
		}
		this->update();
		//this->printVals();
	}
	else
		_moved = 0;
	
	_mouseX = mouseX;
}

// dessin des curseurs
void QSlice::drawCursor(double pos, int id)
{
	if ((id != 1) && (id != 2))
		return;
	
	// profondeur (z-index)
	double z;
	if (_onMax != (id == 1))	z = 0.2;
	else						z = 0.0;
	
	// curseur
	QColor color = this->palette().color(QPalette::Window).dark(135);
	glColor3f(color.redF(), color.greenF(), color.blueF());
	glBegin(GL_QUADS);
		glVertex3f(pos - (_cursorSize / 2.0),	 0.4, -0.1 + z);
		glVertex3f(pos + (_cursorSize / 2.0),	 0.4, -0.1 + z);
		glVertex3f(pos + (_cursorSize / 2.0),	 0.2, -0.1 + z);
		glVertex3f(pos - (_cursorSize / 2.0),	 0.2, -0.1 + z);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f(pos - (_cursorSize / 2.0),	-0.2, -0.1 + z);
		glVertex3f(pos + (_cursorSize / 2.0),	-0.2, -0.1 + z);
		glVertex3f(pos + (_cursorSize / 2.0),	-0.4, -0.1 + z);
		glVertex3f(pos - (_cursorSize / 2.0),	-0.4, -0.1 + z);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f(pos - (_cursorSize / 2.0),	 0.2, -0.1 + z);
		glVertex3f(pos - (_cursorSize / 4.0),	 0.2, -0.1 + z);
		glVertex3f(pos - (_cursorSize / 4.0),	-0.2, -0.1 + z);
		glVertex3f(pos - (_cursorSize / 2.0),	-0.2, -0.1 + z);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3f(pos + (_cursorSize / 4.0),	 0.2, -0.1 + z);
		glVertex3f(pos + (_cursorSize / 2.0),	 0.2, -0.1 + z);
		glVertex3f(pos + (_cursorSize / 2.0),	-0.2, -0.1 + z);
		glVertex3f(pos + (_cursorSize / 4.0),	-0.2, -0.1 + z);
	glEnd();
	//~ if ((_onMax && (id == 2)) || (_onMin && (id == 1)))
	//~ {
		//~ if (_moved == id)
			//~ glColor3f(0.25, 0.25, 1.0);
		//~ else
			//~ glColor3f(0.25, 0.25, 0.75);
		//~ glBegin(GL_QUADS);
			//~ glVertex3f(pos - (_cursorSize / 4.0),	 0.2, -0.05 + z);
			//~ glVertex3f(pos + (_cursorSize / 4.0),	 0.2, -0.05 + z);
			//~ glVertex3f(pos + (_cursorSize / 4.0),	-0.2, -0.05 + z);
			//~ glVertex3f(pos - (_cursorSize / 4.0),	-0.2, -0.05 + z);
		//~ glEnd();
	//~ }
	if ((_onMax || _onMin) && (_moved == id))
	{
		glColor3f(0.25, 0.25, 0.75);
		glBegin(GL_QUADS);
			glVertex3f(pos - (_cursorSize / 4.0),	 0.2, -0.05 + z);
			glVertex3f(pos + (_cursorSize / 4.0),	 0.2, -0.05 + z);
			glVertex3f(pos + (_cursorSize / 4.0),	-0.2, -0.05 + z);
			glVertex3f(pos - (_cursorSize / 4.0),	-0.2, -0.05 + z);
		glEnd();
	}
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(pos - (_cursorSize / 2.0) +0.05,  0.4, 0.0 + z);
		glVertex3f(pos + (_cursorSize / 2.0) -0.05,  0.4, 0.0 + z);
		glVertex3f(pos + (_cursorSize / 2.0) -0.05, -0.4, 0.0 + z);
		glVertex3f(pos - (_cursorSize / 2.0) +0.05, -0.4, 0.0 + z);
	glEnd();
	glBegin(GL_LINE_LOOP);
		glVertex3f(pos - (_cursorSize / 4.0) +0.05,  0.2, 0.0 + z);
		glVertex3f(pos + (_cursorSize / 4.0) -0.05,  0.2, 0.0 + z);
		glVertex3f(pos + (_cursorSize / 4.0) -0.05, -0.2, 0.0 + z);
		glVertex3f(pos - (_cursorSize / 4.0) +0.05, -0.2, 0.0 + z);
	glEnd();
	color = this->palette().color(QPalette::Window);
	glColor3f(color.redF(), color.greenF(), color.blueF());
	glBegin(GL_LINES);
		glVertex3f(pos - (_cursorSize / 4.0), -0.23, 0 + z);
		glVertex3f(pos - (_cursorSize / 4.0), -0.37, 0 + z);
		glVertex3f(pos - (_cursorSize / 4.0), +0.23, 0 + z);
		glVertex3f(pos - (_cursorSize / 4.0), +0.37, 0 + z);
		glVertex3f(pos - (_cursorSize / 8.0), -0.23, 0 + z);
		glVertex3f(pos - (_cursorSize / 8.0), -0.37, 0 + z);
		glVertex3f(pos - (_cursorSize / 8.0), +0.23, 0 + z);
		glVertex3f(pos - (_cursorSize / 8.0), +0.37, 0 + z);
		glVertex3f(pos						 , -0.23, 0 + z);
		glVertex3f(pos						 , -0.37, 0 + z);
		glVertex3f(pos						 , +0.23, 0 + z);
		glVertex3f(pos						 , +0.37, 0 + z);
		glVertex3f(pos + (_cursorSize / 8.0), -0.23, 0 + z);
		glVertex3f(pos + (_cursorSize / 8.0), -0.37, 0 + z);
		glVertex3f(pos + (_cursorSize / 8.0), +0.23, 0 + z);
		glVertex3f(pos + (_cursorSize / 8.0), +0.37, 0 + z);
		glVertex3f(pos + (_cursorSize / 4.0), -0.23, 0 + z);
		glVertex3f(pos + (_cursorSize / 4.0), -0.37, 0 + z);
		glVertex3f(pos + (_cursorSize / 4.0), +0.23, 0 + z);
		glVertex3f(pos + (_cursorSize / 4.0), +0.37, 0 + z);
	glEnd();
}

// affichage des changements
void QSlice::printVals(int /*val*/)
{
	qDebug() << "Slice [" << qRound(_currentMin + _valueMin)
			 << "-" << qRound(_currentMax + _valueMin) <<"] \t"
			 << "Total [" << qRound(_valueMin)
			 << "-" << qRound(_valueMax) << "]";
}
