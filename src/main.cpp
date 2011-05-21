#include <QApplication>

#include "qSlice.h"

#include <QSpinBox>
#include <QWidget>
#include <QVBoxLayout>

int main (int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	QSlice slice;
	QSlice slice2;
	QWidget mainWidget;
	QVBoxLayout mainLayout(&mainWidget);
	
	slice.setWindowTitle("Slice[Master]");
	slice2.setWindowTitle("Slice[Slave]");
	
	mainLayout.addWidget(&slice);
	mainLayout.addWidget(&slice2);
	
	mainWidget.show();	
	
	QObject::connect(
		&slice, SIGNAL(minChanged(int)),
		&slice2, SLOT(setCurrentMin(int))
	);
	QObject::connect(
		&slice, SIGNAL(maxChanged(int)),
		&slice2, SLOT(setCurrentMax(int))
	);
	
	return app.exec();
}
