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

#include "mixedvaluespeakermodel.h"

MixedValueSpeakerModel::MixedValueSpeakerModel(QObject *parent) :
	QObject(parent), m_selectionModel(NULL), m_updating(false)
{}

void MixedValueSpeakerModel::setSelectionModel(QListQmlWrapper *selectionModel)
{
	m_selectionModel = selectionModel;
	connect(m_selectionModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
			this, SLOT(rowsInserted(QModelIndex, int, int)));
	connect(m_selectionModel, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)),
			this, SLOT(rowsRemoved(QModelIndex, int,int)));
}

void MixedValueSpeakerModel::setMultiProperty(PropertyId id, bool newState)
{
	void (SpeakerModel::*setter)(bool);
	void (MixedValueSpeakerModel::*update)();
	switch (id) {
	case OPD_FFR_ID:
		setter = &SpeakerModel::setOpdFfr;
		update = &MixedValueSpeakerModel::someOpdFfrChanged;
		break;
	case OPD_GOV_ID:
		setter = &SpeakerModel::setOpdGov;
		update = &MixedValueSpeakerModel::someOpdGovChanged;
		break;
	case OPD_OP_ID:
		setter = &SpeakerModel::setOpdOp;
		update = &MixedValueSpeakerModel::someOpdOpChanged;
		break;
	case BPS_GOV_ID:
		setter = &SpeakerModel::setBpsGov;
		update = &MixedValueSpeakerModel::someBpsGovChanged;
		break;
	case BPS_OP_ID:
		setter = &SpeakerModel::setBpsOp;
		update = &MixedValueSpeakerModel::someBpsOpChanged;
		break;
	case OPD_JUD_ID:
		setter = &SpeakerModel::setOpdJud;
		update = &MixedValueSpeakerModel::someOpdJudChanged;
		break;
	case BPS_JUD_ID:
		setter = &SpeakerModel::setBpsJud;
		update = &MixedValueSpeakerModel::someBpsJudChanged;
		break;
	case IS_BEGINNER_ID:
		setter = &SpeakerModel::setIsBeginner;
		update = &MixedValueSpeakerModel::someIsBeginnerChanged;
		break;
	default: Q_ASSERT(false);
	}

	// Disable automatic updating
	m_updating = true;
	for (int i = 0; i < m_selectionModel->rowCount(); ++i) {
		SpeakerModel *speakerModel = qobject_cast<SpeakerModel *>(m_selectionModel->getRow(i));
		(speakerModel->*setter)(newState);
	}
	m_updating = false;
	// manual value update
	(this->*update)();

}


void MixedValueSpeakerModel::setIsBeginner(MixedValueSpeakerModel::PropertyState state)
{
	if (state != m_isBeginner) {
		m_isBeginner = state;
		emit isBeginnerChanged();
	}
}

void MixedValueSpeakerModel::setOpdFfr(MixedValueSpeakerModel::PropertyState state)
{
	if (state != m_opdFfr) {
		m_opdFfr = state;
		emit opdFfrChanged();
	}
}

void MixedValueSpeakerModel::setOpdGov(MixedValueSpeakerModel::PropertyState state)
{
	if (state != m_opdGov) {
		m_opdGov = state;
		emit opdGovChanged();
	}
}

void MixedValueSpeakerModel::setOpdOp(MixedValueSpeakerModel::PropertyState state)
{
	if (state != m_opdOp) {
		m_opdOp = state;
		emit opdOpChanged();
	}
}

void MixedValueSpeakerModel::setBpsGov(MixedValueSpeakerModel::PropertyState state)
{
	if (state != m_bpsGov) {
		m_bpsGov = state;
		emit bpsGovChanged();
	}
}

void MixedValueSpeakerModel::setBpsOp(MixedValueSpeakerModel::PropertyState state)
{
	if (state != m_bpsOp) {
		m_bpsOp = state;
		emit bpsOpChanged();
	}
}

void MixedValueSpeakerModel::setBpsJud(MixedValueSpeakerModel::PropertyState state)
{
	if (state != m_bpsJud) {
		m_bpsJud = state;
		emit bpsJudChanged();
	}
}

void MixedValueSpeakerModel::setOpdJud(MixedValueSpeakerModel::PropertyState state)
{
	if (state != m_opdJud) {
		m_opdJud = state;
		emit opdJudChanged();
	}
}

void MixedValueSpeakerModel::connectToSpeaker(SpeakerModel *model)
{
	connect(model, SIGNAL(isBeginnerChanged(bool)), this, SLOT(someIsBeginnerChanged()));
	connect(model, SIGNAL(opdFfrChanged(bool)), this, SLOT(someOpdFfrChanged()));
	connect(model, SIGNAL(opdGovChanged(bool)), this, SLOT(someOpdGovChanged()));
	connect(model, SIGNAL(opdOpChanged(bool)), this, SLOT(someOpdOpChanged()));
	connect(model, SIGNAL(bpsGovChanged(bool)), this, SLOT(someBpsGovChanged()));
	connect(model, SIGNAL(bpsOpChanged(bool)), this, SLOT(someBpsOpChanged()));
	connect(model, SIGNAL(bpsJudChanged(bool)), this, SLOT(someBpsJudChanged()));
	connect(model, SIGNAL(opdJudChanged(bool)), this, SLOT(someOpdJudChanged()));
}

void MixedValueSpeakerModel::updateProperty(bool (SpeakerModel::*getter)(),
											void (MixedValueSpeakerModel::*setter)(PropertyState))
{
	// don't update while still setting values
	// - see setMultiProperty
	if (m_updating)
		return;
	PropertyState result = UNDEFINED;
	for (int i = 0; i < m_selectionModel->rowCount(); ++i) {
		SpeakerModel *speakerModel = qobject_cast<SpeakerModel *>(m_selectionModel->getRow(i));
		if (speakerModel) {
			bool currentValue = (speakerModel->*getter)();
			if (result == UNDEFINED)
				result = currentValue ? ON : OFF;
			else if (result != currentValue) {
				result = MIXED;
				break;
			}
		}
	}
	(this->*setter)(result);
}


void MixedValueSpeakerModel::rowsInserted(QModelIndex index, int start, int end)
{
	Q_UNUSED(index);
	for (int i = start; i <= end; ++i) {
		SpeakerModel *speaker = qobject_cast<SpeakerModel*>(m_selectionModel->getRow(i));
		connectToSpeaker(speaker);
	}
	updateAll();
}

void MixedValueSpeakerModel::rowsRemoved(QModelIndex index, int start, int end)
{
	Q_UNUSED(index);
	for (int i = start; i <= end; ++i) {
		m_selectionModel->getRow(i)->disconnect(this);
	}
	updateAll();
}

void MixedValueSpeakerModel::updateAll() {
	someOpdFfrChanged();
	someOpdGovChanged();
	someOpdOpChanged();
	someBpsGovChanged();
	someBpsOpChanged();
	someOpdJudChanged();
	someBpsJudChanged();
	someIsBeginnerChanged();
}
