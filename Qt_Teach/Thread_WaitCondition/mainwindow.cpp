#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStringList>
#include <QWaitCondition>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <QDebug>

#ifdef Q_OS_WIN
#include<windows.h>
#else
#include <unistd.h>
#endif

const int DataSize = 1000;
QStringList listBuffer;
const int bufferSize = 10;

QWaitCondition bufferNotEmpty;
QWaitCondition bufferNotFull;
QMutex mutex;

class Producer : public QThread
{
public:
	Producer(QObject *parent = NULL) : QThread(parent)
	{
	}
	void run() override
	{
		for (int i = 0; i < DataSize; ++i)
		{
			QMutexLocker locker(&mutex);
			if (listBuffer.size() == bufferSize)
				bufferNotFull.wait(&mutex);

			QString strNumer = QString::number(i);

			//假设生产者比较慢,一秒一个
#ifdef Q_OS_WIN
			Sleep(1000);
#endif
			qDebug()<<"producer----------"<<strNumer<<endl;
			listBuffer.push_back(strNumer);
			bufferNotEmpty.wakeAll();
		}
	}
};

class Consumer : public QThread
{
	Q_OBJECT
public:
	Consumer(QObject *parent = NULL) : QThread(parent)
	{
	}

	void run() override
	{
		while(true)
		{
			QMutexLocker locker(&mutex);
			if (listBuffer.isEmpty())
				bufferNotEmpty.wait(&mutex);

			QString strText = listBuffer.front();
			listBuffer.pop_front();
			bufferNotFull.wakeAll();

			//在这里手动unlock.
			locker.unlock();
#ifdef Q_OS_WIN
			Sleep(2000);
#endif
			//最好在这里来消费，如果多个消费者，就不会卡主其它线程消费了。
			qDebug()<<"consmer------"<<strText<<endl;

		}
	}
};

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);


	Producer producer;
	Consumer consumer;
	producer.start();
	consumer.start();
	producer.wait();
	consumer.wait();
}

MainWindow::~MainWindow()
{
	delete ui;
}

#include "mainwindow.moc"
