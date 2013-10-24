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

#include "prototypeqlistqmlwrapper.h"

PrototypeQListQmlWrapper::PrototypeQListQmlWrapper(const QObject* prototype, QObject *parent) :
	QListQmlWrapper(parent)
{
	Q_ASSERT(prototype);
	const QMetaObject *metaObject = prototype->metaObject();
	int count = metaObject->propertyCount();
	for (int i = 0; i < count; ++i) {
		QMetaProperty metaProperty = metaObject->property(i);
		const char *name = metaProperty.name();
		m_roleNames.insert(Qt::UserRole + i, name);
	}
}

QHash<int, QByteArray> PrototypeQListQmlWrapper::roleNames() const
{
	return m_roleNames;
}
