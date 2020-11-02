#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setAttribute(Qt::WA_TranslucentBackground, true);
	setWindowFlags(Qt::FramelessWindowHint);


	m_imgList.push_back(QImage(":/res/1.png"));
	m_imgList.push_back(QImage(":/res/2.png"));
	m_imgList.push_back(QImage(":/res/3.png"));

	m_pAnimation = new QPropertyAnimation(this, "index");
	m_pAnimation->setDuration(1000);
	m_pAnimation->setLoopCount(-1);
	m_pAnimation->setStartValue(0);
	m_pAnimation->setEndValue(3);

	m_pAnimation->start();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setIndex(int index)
{
	m_pixIndex = index;
	update();
}

int MainWindow::getIndex()
{
	return m_pixIndex;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.drawImage(rect(), m_imgList.at(getIndex()));
}

