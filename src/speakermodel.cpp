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

#include "speakermodel.h"

SpeakerModel::SpeakerModel(QObject *parent) : QObject(parent) {
	m_name = "";
	m_isBeginner = false;
	m_opdFfr = true;
	m_opdOp = true;
	m_opdJud = true;
	m_opdGov = true;
	m_bpsGov = true;
	m_bpsOp = true;
	m_bpsJud = true;
	m_team = NULL;
}

void SpeakerModel::setName(QString name) {
	if (m_name != name) {
		m_name = name;
		emit nameChanged(name);
	}
}

void SpeakerModel::setIsBeginner(bool isBeginner) {
	if (m_isBeginner != isBeginner) {
		m_isBeginner = isBeginner;
		emit isBeginnerChanged(isBeginner);
	}
}

void SpeakerModel::setTeam(Team *team)
{
	if (m_team != team) {
		if (m_team) {
			m_team->disconnect(this);
		}
		m_team = team;
		if (team) {
			team->addSpeaker(this);
			connect(team, SIGNAL(destroyed()), this, SLOT(resetTeam()));
		}
		emit teamChanged(this);
	}
}

void SpeakerModel::setOpdJud(bool opdJud)
{
	if (m_opdJud != opdJud) {
		m_opdJud = opdJud;
		emit opdJudChanged(opdJud);
	}
}

void SpeakerModel::setOpdFfr(bool opdFfr) {
	if (m_opdFfr != opdFfr) {
		m_opdFfr = opdFfr;
		emit opdFfrChanged(opdFfr);
	}
}

void SpeakerModel::setOpdGov(bool opdGov) {
	if (m_opdGov != opdGov) {
		m_opdGov = opdGov;
		emit opdGovChanged(opdGov);
	}
}

void SpeakerModel::setOpdOp(bool opdOp) {
	if (m_opdOp != opdOp) {
		m_opdOp = opdOp;
		emit opdOpChanged(opdOp);
	}
}

void SpeakerModel::setBpsOp(bool bpsOp) {
	if (m_bpsOp != bpsOp) {
		m_bpsOp = bpsOp;
		emit bpsOpChanged(bpsOp);
	}
}

void SpeakerModel::setBpsGov(bool bpsGov) {
	if (m_bpsGov != bpsGov) {
		m_bpsGov = bpsGov;
		emit bpsGovChanged(bpsGov);
	}
}

void SpeakerModel::setBpsJud(bool bpsJud) {
	if (m_bpsJud != bpsJud) {
		m_bpsJud = bpsJud;
		emit bpsJudChanged(bpsJud);
	}
}
