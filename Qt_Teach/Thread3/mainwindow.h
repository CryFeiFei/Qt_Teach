#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include <QThread>
#include <QObject>

class WorkThread : public QObject
{
	Q_OBJECT
public:
	WorkThread(QObject* parent = nullptr);
	~WorkThread();
public slots:
	void Start();
	void Stop();
	void Pause();
	void Continue();
	void Destory();

private:
	void doWork();

signals:
	void workStart();
	void workStop();
	void workPause();
	void workContinue();
	void workDestory();

private:
	bool m_bPause;
	bool m_bStop;
	bool m_bDestory;
	int m_i;
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

signals:
	void threadStart();

public slots:
	void ThreadStart();
	void ThreadPause();
	void ThreadContinue();
	void ThreadStop();
	void ThreadDestory();

private:
	Ui::MainWindow *ui;
	QThread* m_workerThread;
	WorkThread* m_worker;
};
#endif // MAINWINDOW_H
