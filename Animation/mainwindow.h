#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	// 声明属性
	Q_PROPERTY(int index READ getIndex WRITE setIndex)
public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	void setIndex(int index);
	int getIndex();

	void paintEvent(QPaintEvent* e);
//	void showEvent(QShowEvent* e);

private:
	Ui::MainWindow *ui;

	QPropertyAnimation* m_pAnimation = nullptr;
	QList<QImage> m_imgList;
	int m_pixIndex;
	QImage m_curImg;
};

#endif // MAINWINDOW_H
