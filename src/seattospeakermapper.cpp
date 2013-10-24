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

#include "seattospeakermapper.h"

SeatToSpeakerMapper::SeatToSpeakerMapper(QObject *parent) :
	QObject(parent)
{

}

void SeatToSpeakerMapper::addMapping(Debate *debate, Debate::RoleGroup group, int seatIndex, SpeakerModel *speaker)
{
	QHash<Debate::RoleGroup, TableNames*> * map1 =
			m_mapping[debate];
	if (!map1) {
		m_mapping.insert(debate,map1 = new QHash<Debate::RoleGroup, TableNames*>);
	}
	TableNames* tableNames = map1->value(group);
	if (!tableNames) {
		map1->insert(group, tableNames = new TableNames(this));
	}
	tableNames->addMapping(seatIndex, speaker);
}

TableNames* SeatToSpeakerMapper::getNamesForTable(Debate* debate, Debate::RoleGroup group)
{
	QHash<Debate::RoleGroup, TableNames*>* map1 = m_mapping.value(debate);
	if (map1) {
		return map1->value(group);
	}
	return NULL;
}

TableNames *SeatToSpeakerMapper::getNamesForTable(Debate *debate, int group)
{
	return getNamesForTable(debate, Debate::RoleGroup(group));
}
