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

#include "team.h"

Team::Team(QObject *parent) :
	QObject(parent)
{

}

void Team::setTeamName(QString teamName)
{
	if (teamName != m_teamName) {
		m_teamName = teamName;
		emit teamNameChanged();
	}
}


void Team::addSpeaker(SpeakerModel *speaker)
{
	Q_ASSERT(speaker);
	if (!m_speakers.contains(speaker)) {
		m_speakers << speaker;
		connect(speaker, SIGNAL(teamChanged(SpeakerModel*)), this, SLOT(updateSpeaker(SpeakerModel*)));
		connect(speaker, SIGNAL(destroyed(QObject*)), this, SLOT(removeSpeaker(QObject*)));
		emit speakersChanged();
	}
}


void Team::updateSpeaker(SpeakerModel* speaker)
{
	Q_ASSERT(speaker);
	if (speaker->team() != this) {
		m_speakers.removeAll(speaker);
		speaker->disconnect(this);
		emit speakersChanged();
	}
}

void Team::removeSpeaker(int index)
{
	removeSpeaker(m_speakers[index]);
}

void Team::removeSpeaker(QObject *obj)
{
	SpeakerModel *speaker = static_cast<SpeakerModel*>(obj);
	Q_ASSERT(speaker);
	if (speaker->team() == this)
		// this will call updateSpeaker and thus remove the speaker from this team
		speaker->resetTeam();
}

