#include <QApplication>
#include <QPushButton>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	
	QPushButton *button = new QPushButton("quit", 0);
	button->resize(120, 35);
	button->move(300,300);
	button->show();

	return app.exec();
}
