#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

WorkThread::WorkThread(QObject* parent) : QObject (parent)
{
}
WorkThread::~WorkThread()
{
	qDebug()<<"WorkThread Destory"<<endl;
}
void WorkThread::start1()
{
	emit workStart();
	doWork();
}
void WorkThread::doWork()
{
	for (int i = 0; i < 1000; i++)
	{
		qDebug()<<i<<endl;
	}
	emit workFinished();
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);


	m_workerThread = new QThread();
	WorkThread* worker = new WorkThread();
	worker->moveToThread(m_workerThread);

	//开始线程
	connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::ThreadStart);
	connect(m_workerThread, &QThread::started, worker, &WorkThread::start1);

	//销毁线程
	connect(worker, &WorkThread::workFinished, worker, &WorkThread::deleteLater);
	connect(worker, &WorkThread::destroyed, m_workerThread, &QThread::quit);
	connect(m_workerThread, &QThread::finished, m_workerThread, &QThread::deleteLater);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::ThreadStart()
{
	m_workerThread->start();
}

