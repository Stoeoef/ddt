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

#ifndef DEBATE_H
#define DEBATE_H

#include <QObject>

/*!
 * \brief Represents an abstract debate.
 *
 * This class serves as a superclass for all different debate formats.
 * \sa OPDebate BPSDebate
 */
class Debate : public QObject
{
	Q_OBJECT
	//! Holds the number of judicators used in this debates. Defaults to 1.
	Q_PROPERTY(int judicatorPlaces READ judicatorPlaces WRITE setJudicatorPlaces
			   NOTIFY judicatorPlacesChanged)
	//! Holds the debates format
	Q_PROPERTY(DebateFormat format READ format CONSTANT)
	//! Holds the debates room. The room can be in arbitrary string.
	Q_PROPERTY(QString room READ room WRITE setRoom NOTIFY roomChanged)
	//! Holds the complete number of places within this debate.
	Q_PROPERTY(int numPlaces READ numPlaces NOTIFY numPlacesChanged)
	Q_ENUMS(DebateFormat)
	Q_ENUMS(RoleGroup)

public:
	//! Contains all supported debate formats.
	enum DebateFormat {
		OPD, /*!< Offene Parlamentarische Debatte, see \ref OPDebate */
		BPS /*!< British Parliamentary Style debate, see \ref BPSDebate */
	};

	//! Contains all possible roles that speakers of a debate can have
	enum RoleGroup {
		JUDICATOR,  /*!< The judicator role */
		OPD_GOV, /*!< The government role of OPD */
		OPD_OP,  /*!< The opposition role of OPD */
		OPD_FFR, /*!< The FFR role of OPD */
		BPS_GOV1,  /*!< The first government role of BPS */
		BPS_GOV2,  /*!< The second government role of BPS */
		BPS_OP1,  /*!< The first opposition role of BPS */
		BPS_OP2 /*!< The second opposition role of BPS */
	};

	/*!
	 * \brief Creates a new debate with a given parent.
	 */
	explicit Debate(QObject *parent = 0);


	QString room() {return m_room;}
	void setRoom(QString room);

	int judicatorPlaces() {return m_judicatorPlaces;}
	void setJudicatorPlaces(int judicatorPlaces);

	virtual DebateFormat format() = 0;

	virtual int numPlaces();

	//! Returns all roles that belong to this debate
	virtual const QList<RoleGroup> *getRolesForDebate() = 0;

signals:
	void roomChanged();
	void numPlacesChanged();
	void judicatorPlacesChanged();
private:
	QString m_room;
	int m_judicatorPlaces;

};

#endif // DEBATE_H
