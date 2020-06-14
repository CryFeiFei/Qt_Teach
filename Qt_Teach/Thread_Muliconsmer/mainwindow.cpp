#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStringList>
#include <QWaitCondition>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <QDebug>
#include <deque>

#ifdef Q_OS_WIN
#include<windows.h>
#else
#include <unistd.h>
#endif


static const int kItemsToProduce = 10000;//定义生产者能够生产的最大产品个数

struct ItemRepository
{
	std::deque<int> itemQueue; //缓冲区
	const int MaxSize = 10; // 仓库所容纳的产品最大个数
	int itemCounterUse = 0; //消费者用了多少
	std::mutex mutex;// 互斥量,保护产品缓冲区
	std::mutex mutexConsumer; //消费者用来同步用的
	std::condition_variable bufferNotFull;     // 条件变量, 指产品仓库缓冲区不为满
	std::condition_variable bufferNotEmpty;    // 条件变量, 指产品仓库缓冲区不为空
}gItemRepository;   // 产品库全局变量，生产者和消费者操作该变量.

class Producer : public QThread
{
	Q_OBJECT
public:
	Producer(QObject *parent = NULL) : QThread(parent)
	{
	}
	// 生产 产品
	void ProduceItem(ItemRepository &itemRepo, int item)
	{
		std::unique_lock<std::mutex> lock(itemRepo.mutex);
		if (itemRepo.itemQueue.size() == itemRepo.MaxSize)
		{
			itemRepo.bufferNotFull.wait(lock);
			qDebug()<<"product has full"<<endl;
		}

//#ifdef Q_OS_WIN
//		Sleep(1000);
//#endif

		itemRepo.itemQueue.push_back(item);         // 仓库放入产品
		itemRepo.bufferNotEmpty.notify_all();  // 通知消费者仓库不为空
	}

	void run() override
	{
		for (int i = 1; i <= kItemsToProduce; i++)
		{
			ProduceItem(gItemRepository, i);
			{
				qDebug()<<"procut------------"<<i<<endl;
			}
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
	// 消费 产品
	int ConsumeItem(ItemRepository &itemRepo)
	{
		int data;
		std::unique_lock<std::mutex> lock(itemRepo.mutex);
		if (itemRepo.itemQueue.empty())
		{
			itemRepo.bufferNotEmpty.wait(lock);
		}
		data = itemRepo.itemQueue.front();
		itemRepo.itemQueue.pop_front();
		itemRepo.bufferNotFull.notify_all();
		return data;
	}

	void run() override
	{
		while (true)
		{
			int item = 0;
			std::unique_lock<std::mutex> lock(gItemRepository.mutexConsumer);  // 仓库产品消费计数器保持多线程互斥
			if (gItemRepository.itemCounterUse < kItemsToProduce)
			{
				item = ConsumeItem(gItemRepository);    // 消费产品
				gItemRepository.itemCounterUse++;  // 每消费一次进行计数器+1
			}

			lock.unlock();

			if (this->objectName() == "thread0")
			{
#ifdef Q_OS_WIN
				Sleep(2000);
#endif
			}
			else if (this->objectName() == "thread1")
			{
#ifdef Q_OS_WIN
				Sleep(3000);
#endif
			}
			else if (this->objectName() == "thread2")
			{
#ifdef Q_OS_WIN
				Sleep(2000);
#endif
			}
			qDebug()<<"Consumer------------"<<this->objectName()<<"-----"<<item<<endl;
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
	consumer.setObjectName("thread0");
	Consumer consumer1;
	consumer1.setObjectName("thread1");
	Consumer consumer2;
	consumer2.setObjectName("thread2");


	producer.start();
	consumer.start();
	consumer1.start();
	consumer2.start();

	producer.wait();
	consumer.wait();
	consumer1.wait();
	consumer2.wait();
}

MainWindow::~MainWindow()
{
	delete ui;
}

#include "mainwindow.moc"

