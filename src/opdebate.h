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

#ifndef OPDEBATE_H
#define OPDEBATE_H

#include "debate.h"

/*! \brief Represents a debate in OPD-format.
 *
 * OPD (Offene Parlamentarische Debatte) is a popular German format for debating.
 * The debate has 3 different roles, the government, the opposition and the FFR
 * (Fraktionsfreie Redner). Each role consists of 3 speakers.
 * The debate is judicated by at least one judicator.
 *
 * This class allows the representation of understaffed debates with less than 3
 * speakers in a speaker role or no judicator.
 * \sa Debate BPSDebate
 */
class OPDebate : public Debate
{
	Q_OBJECT
	//! Holds the number of places on the government side
	Q_PROPERTY(int govPlaces READ govPlaces WRITE setGovPlaces NOTIFY govPlacesChanged)
	//! Holds the number of places on the opposition side
	Q_PROPERTY(int opPlaces READ opPlaces WRITE setOpPlaces NOTIFY opPlacesChanged)
	//! Holds the number of FFRs
	Q_PROPERTY(int ffrPlaces READ ffrPlaces WRITE setFfrPlaces NOTIFY ffrPlacesChanged)

public:
	explicit OPDebate(QObject *parent = 0);

	int govPlaces() {return m_govPlaces;}
	void setGovPlaces(int govPlaces);

	int opPlaces() {return m_opPlaces;}
	void setOpPlaces(int opPlaces);

	int ffrPlaces() {return m_ffrPlaces;}
	void setFfrPlaces(int ffrPlaces);

	const QList<RoleGroup> *getRolesForDebate();

	DebateFormat format() {return OPD;}
	int numPlaces();
signals:
	void govPlacesChanged();
	void opPlacesChanged();
	void ffrPlacesChanged();
private:
	int m_govPlaces;
	int m_opPlaces;
	int m_ffrPlaces;
	QList<RoleGroup> ROLES;
};

#endif // OPDEBATE_H
