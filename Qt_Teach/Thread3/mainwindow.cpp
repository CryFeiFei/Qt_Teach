#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#ifdef Q_OS_WIN
#include<windows.h>
#else
#include <unistd.h>
#endif

WorkThread::WorkThread(QObject* parent) : QObject (parent)
  , m_bPause(false)
  , m_bStop(false)
  , m_bDestory(false)
  , m_i(0)
{
}
WorkThread::~WorkThread()
{
	qDebug()<<"WorkThread Destory"<<endl;
}
void WorkThread::Start()
{
	m_bStop = false;
	emit workStart();
	doWork();
	emit workStop();
}
void WorkThread::doWork()
{
	while(true)
	{
		if (m_bPause)
		{
			emit workPause();
			continue;
		}

		if (m_bStop)
		{
			emit workStop();
			break;
		}

		if (m_bDestory)
		{
			emit workDestory();
			break;
		}

#ifdef Q_OS_WIN
		Sleep(1000);
#else
		sleep(1);
#endif
		qDebug()<<"--"<<m_i<<endl;
		m_i++;
	}
}

void WorkThread::Stop()
{
	m_bStop = true;
}

void WorkThread::Pause()
{
	m_bPause = true;
}

void WorkThread::Continue()
{
	m_bPause = false;
}

void WorkThread::Destory()
{
	m_bDestory = true;
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_workerThread = new QThread();
	m_worker = new WorkThread();
	m_worker->moveToThread(m_workerThread);

	connect(ui->btnPause, &QPushButton::clicked, this, &MainWindow::ThreadPause, Qt::DirectConnection);
	connect(ui->btnContinue, &QPushButton::clicked, this, &MainWindow::ThreadContinue ,Qt::DirectConnection);
	connect(ui->btnStop, &QPushButton::clicked, this, &MainWindow::ThreadStop, Qt::DirectConnection);
	connect(ui->btnDestory, &QPushButton::clicked, this, &MainWindow::ThreadDestory, Qt::DirectConnection);

	//启动线程
	connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::ThreadStart);
	connect(m_workerThread, &QThread::started, m_worker, &WorkThread::Start);
	//stop之后继续走
	connect(this, &MainWindow::threadStart, m_worker, &WorkThread::Start);

	connect(m_worker, &WorkThread::workDestory, m_workerThread, [this]()
	{
		m_workerThread->quit();
		m_workerThread->wait();
	});

	connect(m_workerThread, &QThread::finished, m_worker, &WorkThread::deleteLater);
	connect(m_worker, &WorkThread::destroyed, m_workerThread, &QThread::deleteLater);
	//销毁线程
//	connect(m_worker, &WorkThread::workDestory, m_worker, &WorkThread::deleteLater);
//	connect(m_worker, &WorkThread::destroyed, m_workerThread, &QThread::quit);
//	connect(m_workerThread, &QThread::finished, m_workerThread, &QThread::deleteLater);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::ThreadStart()
{
	if (m_workerThread->isRunning())
	{
		emit threadStart();
	}
	else
	{
		m_workerThread->start();
	}
}

void MainWindow::ThreadPause()
{
	m_worker->Pause();
}

void MainWindow::ThreadContinue()
{
	m_worker->Continue();
}

void MainWindow::ThreadStop()
{
	m_worker->Stop();
}

void MainWindow::ThreadDestory()
{
	m_worker->Destory();
}

