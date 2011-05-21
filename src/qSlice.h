#ifndef __QSLICE_H
#define __QSLICE_H

#include <QWidget>

class QMouseEvent ;
class QPainter;

class QSlice : public QWidget
{
	Q_OBJECT
	
	private:
		double		_currentMin;	// valeur minimale courante
		double		_currentMax;	// valeur maximale courante
		double		_valueMin;		// valeur minimale limite
		double		_valueMax;		// valeur maximale limite
		double		_cursorSize;	// taille du curseur
		double		_paddingSize;	// taille de la "marge-curseur"
		double 		_mouseX;		// dernière position de la souris
		int 		_moved;			// indice de l'élément déplacé
		bool 		_onMin;			// sur le curseur min
		bool 		_onMax;			// sur le curseur max
		QPainter	*_painter;		// qpainter pour redessiner le widget
	
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
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
	
		void drawCursor(double pos, int id);// id = 1 -> min
											// id = 2 -> max

	protected slots:
		void printVals(int = 0);
		
	signals:
		void minChanged(int);	// minimum changé
		void maxChanged(int);	// maximum changé
};

#endif
