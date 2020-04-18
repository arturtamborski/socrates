#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setOrganizationName("arturtamborski");
	a.setOrganizationDomain("pl");
	a.setApplicationName("Socrates");

	MainWindow w;
	w.show();

	return a.exec();
}
