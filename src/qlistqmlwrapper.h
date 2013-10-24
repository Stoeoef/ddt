/**
 * Copyright (C) 2013  Stefan Altmayer
 *
 * This file is part of DDT.
 *
 * DDT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DDT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DDT.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QLISTQMLWRAPPER_H
#define QLISTQMLWRAPPER_H
#include <QAbstractListModel>
#include <QMetaMethod>

/*!
 * \brief Allows accessing a cpp list in QML.
 *
 * QML cannot handle lists of type \c QList. This class fills this gap
 * by offering a list like interface on the cpp side while inheriting
 * \c QAbstractListModel, which is neccessary for elements like the QML
 * \c ListView. A QListQmlWrapper can be set directly as a ListView's,
 * model, the model item roles are specified by a concrete subclass of
 * \QListQmlWrapper . See \ref roleNames for details.\n
 * Deleting an item will automatically remove it from the wrapper's list.
 */
class QListQmlWrapper : public QAbstractListModel
{
	Q_OBJECT
	//! Holds the wrapper's number of items.
	Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)

public:
	/*!
	 * \brief Creates a new QListQmlWrapper with a given parent.
	 *
	 * The wrapper's list is initially empty.
	 * \param parent the object's parent
	 */
	explicit QListQmlWrapper(QObject* parent = 0);


	// QAbstractListModel
	QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	virtual QHash<int, QByteArray> roleNames() const {
		return QHash<int, QByteArray>();
	}
	Qt::ItemFlags flags(const QModelIndex &index) const;


	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	//! Appends an item to the wrapper's list
	Q_INVOKABLE void appendRow(QObject* item);
	//! Appends multiple item's to the wrapper's list
	void appendRows(const QList<QObject*> &items);
	//! Inserts an item at a specific position
	Q_INVOKABLE void insertRow(int row, QObject* item);

	//! Removes an item with a given index.
	//! \returns \c true the given row index was valid and an item was removed.
	Q_INVOKABLE bool removeRow(int row, const QModelIndex &parent = QModelIndex());
	//! Removes \c count items, starting from index \c row .
	Q_INVOKABLE bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
	//! Removes and returns an item at position \c row .
	Q_INVOKABLE QObject* takeRow(int row);
	//! Removes all roles.
	Q_INVOKABLE void clear();

	//! Returns \c true if \c object is contained in the wrappers list.
	Q_INVOKABLE bool contains(QObject* object);
	//! Return the first occurence of a given object in the wrappers list.
	//! Returns -1 if the object is not contained.
	Q_INVOKABLE int indexOf(QObject* object);

	//! Returns the row at a given position.
	Q_INVOKABLE QObject *getRow(int row) const;

	//! Returns the list represented by this wrapper.
	const QList<QObject*> *list();

signals:
	//! Emitted when items were added, moved or removed, or when an item's
	//! proeprty has changed.
	void listChanged();
	void rowCountChanged();

public slots:
	//! Removes one occurence of \object from the list. Returns \c true if
	//! an occurence could be removed
	Q_INVOKABLE bool remove(QObject *object);

private slots:
	void handleItemChange();
private:
	QModelIndex indexFromItem( const QObject* item) const;
	void connectToItem(QObject* item);
	QList<QObject*> m_list;
	QMetaMethod m_handleItemChangeMethod;
};


#endif // QLISTQMLWRAPPER_H
