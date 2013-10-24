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

#ifndef PROTOTYPEQLISTQMLWRAPPER_H
#define PROTOTYPEQLISTQMLWRAPPER_H

#include "qlistqmlwrapper.h"

/*!
 * \brief A QListQmlWrapper with roles extrated from a prototype object.
 *
 * QListQmlWrapper cannot be used without specifying its roleNames.
 * \c PrototypeQListQmlWrapper inherits QListQmlWrapper and redefines \c roleNames,
 * the role names are obtained by reading all properties of a given prototype object.
 * For each property
 */
class PrototypeQListQmlWrapper : public QListQmlWrapper
{
	Q_OBJECT
public:
	/*!
	 * \brief Creates a new \c PrototypeQListQmlWrapper with a given prototype.
	 * The objects roleNames are read from the prototype's properties.
	 * \param prototype an arbitrary object
	 * \param parent the object's parent
	 */
	PrototypeQListQmlWrapper(const QObject *prototype, QObject *parent = 0);
	QHash<int, QByteArray> roleNames() const;
signals:
	
public slots:
private:
	QHash<int, QByteArray> m_roleNames;
};

#endif // PROTOTYPEQLISTQMLWRAPPER_H
