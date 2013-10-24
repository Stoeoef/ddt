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

#ifndef TABLENAMES_H
#define TABLENAMES_H

#include <QObject>
#include <QString>
#include <QHash>
#include "speakermodel.h"

/*!
 * \brief Stores speakers mapped to a table in a debate.
 */
class TableNames : public QObject
{
	Q_OBJECT
public:
	//! Creates a new \c TableNames object with a given parent.
	explicit TableNames(QObject *parent = 0);
	//! Gets the speaker sitting at \c index
	Q_INVOKABLE SpeakerModel* getSpeaker(int index);
	//! Defines the speaker sitting at \c index
	void addMapping(int index, SpeakerModel *speaker);

signals:
	
private slots:
	void speakerDestroyed(QObject* speaker);
private:

	QHash<int, SpeakerModel*> m_names;

};

#endif // TABLENAMES_H
