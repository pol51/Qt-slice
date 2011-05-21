#include "qSlice.h"

#include <QGridLayout>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

// contructeur
QSlice::QSlice(QWidget *parent)
	: QWidget(parent)
{
	_painter = new QPainter();
	
	// valeurs par defaut
	_valueMin = 12.;
	_valueMax = 120.;
	this->setCurrentMin(16);
	this->setCurrentMax(25);
	_cursorSize = 16.;
	_paddingSize = 4.;
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
	delete _painter;
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

void QSlice::paintEvent(QPaintEvent */*event*/)
{
	_paddingSize = this->height() / 10;
	_cursorSize = this->height() / 2 - _paddingSize;
	
	// init
	_painter->begin(this);
	_painter->setRenderHint(QPainter::TextAntialiasing);
	_painter->setBrush(Qt::SolidPattern);
	_painter->setPen(Qt::SolidLine);
	
	// couleurs
	QColor color = this->palette().color(QPalette::Window);
	QColor colorBG = color.dark(110);
	QColor colorFG = color.dark(95);
	
	// init variables
	QRectF rectangle;
	double minX = _currentMin / (_valueMax - _valueMin) * 
		((this->width() - 1)  - 2 * _cursorSize) + _cursorSize;
	double maxX = _currentMax / (_valueMax - _valueMin) * 
		((this->width() - 1) - 2 * _cursorSize) + _cursorSize;
	
	// dessin du fond
	_painter->setBrush(QBrush(colorBG));
	rectangle.setCoords(
		_cursorSize, 0,
		this->width() - 1 - _cursorSize, this->height() - 1);
	_painter->drawRect(rectangle);
	
	// dessin de la selection
	_painter->setBrush(QBrush(colorFG));
	rectangle.setCoords(minX, 0, maxX, this->height() - 1);
	_painter->drawRect(rectangle);
	
	// dessin des curseurs
	if (_moved != 2)
	{
		this->drawCursor(maxX, 2);
		this->drawCursor(minX, 1);
	}
	else
	{
		this->drawCursor(minX, 1);
		this->drawCursor(maxX, 2);
	}
	
	_painter->end();
}

void QSlice::mousePressEvent(QMouseEvent *event)
{
 	if (event->buttons() & Qt::LeftButton)
		_mouseX = (float)(event->x());
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
	// maj du curseur
	if (_onMax || _onMin)
		setCursor(Qt::SizeHorCursor);
	else
		setCursor(Qt::ArrowCursor);

	float mouseX =	(float)(event->x());
	float minX = _currentMin / (_valueMax - _valueMin) * 
		((this->width() - 1)  - 2 * _cursorSize) + _cursorSize;
	float maxX = _currentMax / (_valueMax - _valueMin) * 
		((this->width() - 1) - 2 * _cursorSize) + _cursorSize;
	
	_onMin = ((mouseX > (minX - _cursorSize * 1.1)) &&
				(mouseX < (minX + _cursorSize * 1.1)))	// curseur au dessus
				&& !_onMax			// pas au-dessus de l'autre
				|| (_moved == 1);	// si déjà en déplacement
	_onMax = ((mouseX > (maxX - _cursorSize * 1.1)) &&
				(mouseX < (maxX + _cursorSize * 1.1)))	// curseur au dessus
				&& !_onMin			// pas au dessus de l'autre
				|| (_moved == 2);	// si déjà en déplacement
	
	if (event->buttons() & Qt::LeftButton)
	{
		if ((_moved != 2) && _onMin)
		{
			minX = mouseX * width() /
				((this->width() - 1)  - 2 * _cursorSize) - _cursorSize;
			if (minX < 0)	minX = 0;
			if (minX > width()) minX = width();
			_currentMin = (minX * (_valueMax - _valueMin) / (width() - 1));
			if (_currentMax < _currentMin)
				_currentMax = _currentMin;
			_moved = 1;
			emit minChanged(this->currentMin());
		}
		else if ((_moved != 1) && _onMax)
		{	
			maxX = mouseX * width() / 
				((this->width() - 1)  - 2 * _cursorSize) - _cursorSize;
			if (maxX < 0)	maxX = 0;
			if (maxX > width()) maxX = width();
			_currentMax = (maxX * (_valueMax - _valueMin) / (width() - 1));
			if (_currentMin > _currentMax)
				_currentMin = _currentMax;
			_moved = 2;
			emit maxChanged(this->currentMax());
		}
		this->update();
		//~ this->printVals();
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
	
	QColor color = this->palette().color(QPalette::Window).dark(135);
	
	QRectF rectangle;
	
	// curseur
	_painter->setBrush(Qt::SolidPattern);
	_painter->setPen(Qt::NoPen);
	
	_painter->setBrush(QBrush(color));
	rectangle.setCoords(
		pos - _cursorSize,
		_paddingSize,
		pos + _cursorSize,
		_cursorSize / 2 + _paddingSize);
	_painter->drawRect(rectangle);
	
	rectangle.setCoords(
		pos - _cursorSize,
		this->height() - _paddingSize - 1,
		pos + _cursorSize,
		this->height() - _cursorSize / 2 - _paddingSize - 1);
	_painter->drawRect(rectangle);
	
	rectangle.setCoords(
		pos - _cursorSize,
		_cursorSize / 2 + _paddingSize,
		pos - _cursorSize / 2,
		this->height() - _cursorSize / 2 - _paddingSize - 1);
	_painter->drawRect(rectangle);
	
	rectangle.setCoords(
		pos + _cursorSize,
		_cursorSize / 2 + _paddingSize,
		pos + _cursorSize / 2,
		this->height() - _cursorSize / 2 - _paddingSize - 1);
	_painter->drawRect(rectangle);
	
	//contour
	_painter->setBrush(Qt::NoBrush);
	_painter->setPen(Qt::SolidLine);
	
	rectangle.setCoords(
		pos - _cursorSize, _paddingSize,
		pos + _cursorSize, this->height() - _paddingSize - 1);
	_painter->drawRect(rectangle);
	
	rectangle.setCoords(
		pos - _cursorSize / 2,
		_cursorSize / 2 + _paddingSize,
		pos + _cursorSize / 2,
		this->height() - _cursorSize / 2 - _paddingSize - 1);
	_painter->drawRect(rectangle);
}

// affichage des changements
void QSlice::printVals(int /*val*/)
{
	qDebug() << "Slice [" << qRound(_currentMin + _valueMin)
			 << "-" << qRound(_currentMax + _valueMin) <<"] \t"
			 << "Total [" << qRound(_valueMin)
			 << "-" << qRound(_valueMax) << "]";
}
