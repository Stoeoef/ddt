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

#include "opdebate.h"

OPDebate::OPDebate(QObject *parent) :
	Debate(parent), m_ffrPlaces(3), m_opPlaces(3), m_govPlaces(3)
{
	ROLES << JUDICATOR << OPD_FFR << OPD_GOV << OPD_OP;
	connect(this, SIGNAL(govPlacesChanged()), this, SIGNAL(numPlacesChanged()));
	connect(this, SIGNAL(opPlacesChanged()), this, SIGNAL(numPlacesChanged()));
	connect(this, SIGNAL(ffrPlacesChanged()), this, SIGNAL(numPlacesChanged()));
}

void OPDebate::setGovPlaces(int govPlaces)
{
	if (m_govPlaces != govPlaces) {
		m_govPlaces = govPlaces;
		emit govPlacesChanged();
	}
}

void OPDebate::setOpPlaces(int opPlaces)
{
	if (m_opPlaces != opPlaces) {
		m_opPlaces = opPlaces;
		emit opPlacesChanged();
	}
}

void OPDebate::setFfrPlaces(int ffrPlaces)
{
	if (m_ffrPlaces != ffrPlaces) {
		m_ffrPlaces = ffrPlaces;
		emit ffrPlacesChanged();
	}
}

const QList<Debate::RoleGroup> *OPDebate::getRolesForDebate()
{
	return &ROLES;
}

int OPDebate::numPlaces() {
	return Debate::numPlaces() + m_opPlaces + m_govPlaces + m_ffrPlaces;
}
