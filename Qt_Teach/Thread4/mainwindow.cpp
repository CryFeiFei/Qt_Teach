#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>

#ifdef Q_OS_WIN
#include<windows.h>
#else
#include <unistd.h>
#endif

namespace
{
	QMutex mutex;
	static int index = 0;
}

class WorkerThread : public QThread
{
	Q_OBJECT
public:
	WorkerThread(QObject *parent = 0)
		: QThread(parent)
	{
		setObjectName("WorkerThread");
	}
protected:
	void run()
	{
		while(true)
		{
			QMutexLocker locker(&mutex);
			index ++;
			Sleep(2000);
			qDebug()<<this->objectName()<<"---"<<index<<endl;
		}
	}
};

class WorkerThread1 : public QThread
{
	Q_OBJECT
public:
	WorkerThread1(QObject *parent = 0)
		: QThread(parent)
	{
		setObjectName("WorkerThread1");
	}
protected:
	void run()
	{
		while(true)
		{
			QMutexLocker locker(&mutex);
			index ++;
			Sleep(2000);
			qDebug()<<this->objectName()<<"---"<<index<<endl;
		}
	}
};

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	WorkerThread thread;
	WorkerThread1 thread1;
	thread.start();
	thread1.start();

	thread.wait();
	thread1.wait();
}

MainWindow::~MainWindow()
{
	delete ui;
}

#include "mainwindow.moc"
