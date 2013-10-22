#include "qlistqmlwrapper.h"
#include <Qt>
#include <QMetaProperty>

QListQmlWrapper::QListQmlWrapper(QObject *parent) : QAbstractListModel(parent) {

	m_handleItemChangeMethod = metaObject()->method(metaObject()->indexOfMethod("handleItemChange()"));
	connect(this, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(listChanged()));
	connect(this, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
			this, SIGNAL(listChanged()));
	connect(this, SIGNAL(rowCountChanged()), this, SIGNAL(listChanged()));

	connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(rowCountChanged()));
	connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(rowCountChanged()));
	connect(this, SIGNAL(layoutChanged()), this, SIGNAL(rowCountChanged()));
	connect(this, SIGNAL(modelReset()), this, SIGNAL(rowCountChanged()));
}

QVariant QListQmlWrapper::data(const QModelIndex &index, int role) const {
	if(index.row() < 0 || index.row() >= m_list.size())
		return QVariant();
	QObject* entry = m_list[index.row()];
	return entry->property(roleNames()[role]);
}

bool QListQmlWrapper::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.row() < 0 || index.row() >= m_list.size())
		return false;
	QObject* entry = m_list[index.row()];
	return entry->setProperty(roleNames()[role], value);
}

Qt::ItemFlags QListQmlWrapper::flags(const QModelIndex &index) const
{
	return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

int QListQmlWrapper::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return m_list.size();
}

void QListQmlWrapper::appendRow(QObject *item)
{
	appendRows(QList<QObject*>() << item);
}

void QListQmlWrapper::appendRows(const QList<QObject*> &items)
{
	beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex())+items.size()-1);
	foreach(QObject *item, items) {

		connectToItem(item);

		m_list.append(item);
	}
	endInsertRows();
}

void QListQmlWrapper::connectToItem(QObject *item) {
	const QMetaObject *metaObject = item->metaObject();

	int count = metaObject->propertyCount();

	for (int i=0; i<count; ++i) {
		QMetaProperty metaProperty = metaObject->property(i);
		if (metaProperty.hasNotifySignal()) {
			connect(item, metaProperty.notifySignal(), this, m_handleItemChangeMethod);
		}
	}
	connect(item, SIGNAL(destroyed(QObject*)), this, SLOT(remove(QObject*)));
}

void QListQmlWrapper::insertRow(int row, QObject *item)
{
	beginInsertRows(QModelIndex(), row, row);
	connectToItem(item);
	m_list.insert(row, item);
	endInsertRows();
}

void QListQmlWrapper::handleItemChange()
{
	QObject* item = static_cast<QObject*>(sender());
	QModelIndex index = indexFromItem(item);
	if(index.isValid()) {
		emit dataChanged(index, index);
	}
}

QModelIndex QListQmlWrapper::indexFromItem(const QObject *item) const
{
	Q_ASSERT(item);
	for(int row=0; row<m_list.size(); ++row) {
		if(m_list.at(row) == item) return index(row);
	}
	return QModelIndex();
}

void QListQmlWrapper::clear()
{
	removeRows(0, rowCount());
}

bool QListQmlWrapper::contains(QObject *object)
{
	return indexOf(object) >= 0;
}


int QListQmlWrapper::indexOf(QObject* obj) {
	return m_list.indexOf(obj);
}

bool QListQmlWrapper::removeRow(int row, const QModelIndex &parent)
{
	Q_UNUSED(parent);
	return removeRows(row, 1, parent);
}

bool QListQmlWrapper::removeRows(int row, int count, const QModelIndex &parent)
{
	Q_UNUSED(parent);
	if(row < 0 || (row+count) > m_list.size()) return false;
	beginRemoveRows(QModelIndex(), row, row+count-1);
	for(int i=0; i<count; ++i) {
		m_list.removeAt(row);
	}
	endRemoveRows();
	return true;
}

QObject* QListQmlWrapper::takeRow(int row)
{
	beginRemoveRows(QModelIndex(), row, row);
	QObject* item = m_list.takeAt(row);
	endRemoveRows();
	return item;
}

QObject *QListQmlWrapper::getRow(int row) const
{
	if (row < 0 || row >= m_list.size())
		return 0;
	return m_list[row];
}

bool QListQmlWrapper::remove(QObject *object)
{
	int index = m_list.indexOf(object);
	return index >= 0 && removeRow(index);
}

const QList<QObject*> *QListQmlWrapper::list() {
	return &m_list;
}
