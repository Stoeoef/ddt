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

#include "bpsdebate.h"

BPSDebate::BPSDebate(QObject *parent) :
	Debate(parent), m_gov1Places(2), m_gov2Places(2), m_op1Places(2), m_op2Places(2)
{
	ROLES << JUDICATOR << BPS_GOV1 << BPS_GOV2 << BPS_OP1 << BPS_OP2;
	connect(this, SIGNAL(gov1PlacesChanged()), this, SIGNAL(numPlacesChanged()));
	connect(this, SIGNAL(gov2PlacesChanged()), this, SIGNAL(numPlacesChanged()));
	connect(this, SIGNAL(op1PlacesChanged()), this, SIGNAL(numPlacesChanged()));
	connect(this, SIGNAL(op2PlacesChanged()), this, SIGNAL(numPlacesChanged()));
}

const QList<Debate::RoleGroup> *BPSDebate::getRolesForDebate()
{
	return &ROLES;
}


int BPSDebate::numPlaces() {
	return Debate::numPlaces() + m_gov1Places + m_gov2Places + m_op1Places + m_op2Places;
}

void BPSDebate::setGov1Places(int gov1Places)
{
	if (gov1Places != m_gov1Places) {
		m_gov1Places = gov1Places;
		emit gov1PlacesChanged();
	}
}

void BPSDebate::setGov2Places(int gov2Places)
{
	if (gov2Places != m_gov2Places) {
		m_gov2Places = gov2Places;
		emit gov2PlacesChanged();
	}
}

void BPSDebate::setOp1Places(int op1Places)
{
	if (op1Places != m_op1Places) {
		m_op1Places = op1Places;
		emit op1PlacesChanged();
	}
}

void BPSDebate::setOp2Places(int op2Places)
{
	if (op2Places != m_op2Places) {
		m_op2Places = op2Places;
		emit op2PlacesChanged();
	}
}
