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

#ifndef BPSDEBATE_H
#define BPSDEBATE_H

#include "debate.h"

/*!
 * \brief Represents a debate in BPS-format.
 *
 * BPS (British Parliamentary Style) is a popular, internationally used format
 * for debating. The debate has four different speaker roles, the first and second
 * government and the first and second opposition. Each role consists
 * of two speakers. The debate is judicated by at least one judicator.
 *
 * This class allows the representation of understaffed debates with less than two
 * speakers in a speaker role or no judicator.
 * \sa Debate OPDebate
 */
class BPSDebate : public Debate
{
	Q_OBJECT
	//! Holds the number of speakers in the first government. The default value is 2.
	Q_PROPERTY(int gov1Places READ gov1Places WRITE setGov1Places NOTIFY gov1PlacesChanged)
	//! Holds the number of speakers in the second government. The default value is 2.
	Q_PROPERTY(int gov2Places READ gov2Places WRITE setGov2Places NOTIFY gov2PlacesChanged)
	//! Holds the number of speakers in the first opposition. The default value is 2.
	Q_PROPERTY(int op1Places READ op1Places WRITE setOp1Places NOTIFY op1PlacesChanged)
	//! Holds the number of speakers in the second opposition. The default value is 2.
	Q_PROPERTY(int op2Places READ op2Places WRITE setOp2Places NOTIFY op2PlacesChanged)

public:
	/*!
	 * \brief Creates a new BPS debate with default values
	 * \param parent the \c QObject's parent
	 */
	explicit BPSDebate(QObject *parent = 0);

	int gov1Places() {return m_gov1Places;}
	void setGov1Places(int gov1Places);

	int gov2Places() {return m_gov2Places;}
	void setGov2Places(int gov2Places);

	int op1Places() {return m_op1Places;}
	void setOp1Places(int op1Places);

	int op2Places() {return m_op2Places;}
	void setOp2Places(int op2Places);

	const QList<RoleGroup> *getRolesForDebate();

	DebateFormat format() {return BPS;}
	int numPlaces();
signals:
	void gov1PlacesChanged();
	void gov2PlacesChanged();
	void op1PlacesChanged();
	void op2PlacesChanged();

private:
	int m_gov1Places;
	int m_gov2Places;
	int m_op1Places;
	int m_op2Places;

	QList<RoleGroup> ROLES;
};

#endif // BPSDEBATE_H
