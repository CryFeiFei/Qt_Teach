#include "mainwindow.h"
#include "ui_mainwindow.h"

//-------------------------------
KFileItemModel::KFileItemModel(QObject *parent)
{
	m_listInfo = QStringList()<<"1"<<"2"<<"3"<<"4";
}

int KFileItemModel::rowCount(const QModelIndex& parent) const
{
	return m_listInfo.size();
}

int KFileItemModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

QVariant KFileItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant(QObject::tr("cryfeifei"));
}

QVariant KFileItemModel::data(const QModelIndex &index, int role) const
{
	int nIndex = index.row();
	if (role == Qt::DisplayRole)
	{
		return QVariant(m_listInfo.at(nIndex));
	}
	return QVariant();
}

QModelIndex KFileItemModel::parent(const QModelIndex& index) const
{
	return QModelIndex();
}

QModelIndex KFileItemModel::index(int row, int column, const QModelIndex& parent) const
{
	if (m_listInfo.size() > row)
	{
		return createIndex(row, 0);
	}
	return QModelIndex();
}

////------------------------

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	KFileItemModel* model = new KFileItemModel(this);

	ui->listView->setModel(model);

}

MainWindow::~MainWindow()
{
	delete ui;
}
