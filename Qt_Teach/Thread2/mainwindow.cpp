#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

WorkThread::WorkThread(QObject* parent) : QObject (parent)
{
}
WorkThread::~WorkThread()
{
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


	QThread* m_workerThread = new QThread();
	WorkThread* worker = new WorkThread();
	worker->moveToThread(m_workerThread);

	connect(ui->pushButton, &QPushButton::clicked, [&](){
		m_workerThread->start();
	});

	connect(m_workerThread, &QThread::started, worker, &WorkThread::start1);
	connect(worker, &WorkThread::workFinished, worker, &WorkThread::deleteLater);
	connect(worker, &WorkThread::destroyed, m_workerThread, &QThread::deleteLater);
}

MainWindow::~MainWindow()
{
	delete ui;
}

