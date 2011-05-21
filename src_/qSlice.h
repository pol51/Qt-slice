#ifndef __QSLICE_H
#define __QSLICE_H

#include <QGLWidget>
#include <QTimer>

class QMouseEvent ;

class QSlice : public QGLWidget
{
	Q_OBJECT
	
	private:
		double		_currentMin;	// valeur minimale courante
		double		_currentMax;	// valeur maximale courante
		double		_valueMin;		// valeur minimale limite
		double		_valueMax;		// valeur maximale limite
		double		_cursorSize;	// taille du curseur
		QTimer		_refreshTimer;	// timer de maj
		double 		_mouseX;		// derni�re position de la souris
		int 		_moved;			// indice de l'�l�ment d�plac�
		bool 		_onMin;			// sur le curseur min
		bool 		_onMax;			// sur le curseur max
	
	public:
		QSlice(QWidget *parent = NULL);	// constructeur
		~QSlice();						// destructeur
		
		QSize sizeHint() const;			// taille par defaut
		
		//getters
		int currentMin();
		int currentMax();
		int valueMin();
		int valueMax();
		
		//setters
		void setValueMin(int valueMin);
		void setValueMax(int valueMax);
	
	public slots:
		void setCurrentMin(int currentMin);
		void setCurrentMax(int currentMax);
	
	protected:
		void initializeGL();
		void paintGL();
		void resizeGL(int width, int height);
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);

	protected slots:
		void printVals(int = 0);
		
	signals:
		void minChanged(int);	// minimum chang�
		void maxChanged(int);	// maximum chang�
};

#endif
