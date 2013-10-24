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

#include "debate.h"

Debate::Debate(QObject *parent) :
	QObject(parent), m_room("R109"), m_judicatorPlaces(1)
{
	connect(this, SIGNAL(judicatorPlacesChanged()), this, SIGNAL(numPlacesChanged()));
}

void Debate::setRoom(QString room)
{
	if (m_room != room) {
		m_room = room;
		emit roomChanged();
	}
}

void Debate::setJudicatorPlaces(int judicatorPlaces)
{
	if (m_judicatorPlaces != judicatorPlaces) {
		m_judicatorPlaces = judicatorPlaces;
		emit judicatorPlacesChanged();
	}
}

int Debate::numPlaces() {
	return m_judicatorPlaces;
}
