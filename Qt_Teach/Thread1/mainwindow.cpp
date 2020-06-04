#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QDebug>
#include <QObject>

class WorkerThread : public QThread
{
	Q_OBJECT
public:
	WorkerThread(QObject *parent = 0)
		: QThread(parent)
	{
	}
protected:
	void run()
	{
		for (int i = 0; i < 1000; i++)
		{
			qDebug()<<i<<endl;
		}
		emit workFinished();
	}
signals:
	void workFinished();
};

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	WorkerThread* workThread = new WorkerThread(NULL);
	connect(ui->pushButton, &QPushButton::clicked, [=]()
	{
		workThread->start();
	});

	connect(workThread, &WorkerThread::workFinished, [&]()
	{
		qDebug()<<"workfinished"<<endl;
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}


#include "mainwindow.moc"
