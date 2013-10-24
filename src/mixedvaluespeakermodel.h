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

#ifndef MIXEDVALUESPEAKERMODEL_H
#define MIXEDVALUESPEAKERMODEL_H

#include <QObject>
#include <QItemSelectionModel>
#include "debate.h"
#include "qlistqmlwrapper.h"
#include "speakermodel.h"

/*!
 * \brief Allows editing multiple speakers at once.
 *
 * \c MixedValueSpeakerModel has an interface similar to \ref SpeakerModel,
 * it contains all its boolean typed properties.
 * It accesses a list of different \c SpeakerModel and combines their
 * properties. If the list contains different values for some property,
 * PropertyState::MIXED will be returned as the property's value. Moreover,
 * setting some property for all speakers at once is possible.
 */
class MixedValueSpeakerModel : public QObject
{
	Q_OBJECT
public:
	explicit MixedValueSpeakerModel(QObject *parent = 0);

	//! The selection model. Updates to the selection will automatically be detected.
	//! It must contain elements of type SpeakerModel, no element should be contained
	//! twice.
	Q_PROPERTY(QListQmlWrapper *selectionModel WRITE setSelectionModel)

	//! \see SpeakerModel#isBeginner
	Q_PROPERTY(PropertyState isBeginner READ isBeginner WRITE setIsBeginner NOTIFY isBeginnerChanged)
	//! \see SpeakerModel#opdFfr
	Q_PROPERTY(PropertyState opdFfr READ opdFfr NOTIFY opdFfrChanged)
	//! \see SpeakerModel#opdGov
	Q_PROPERTY(PropertyState opdGov READ opdGov NOTIFY opdGovChanged)
	//! \see SpeakerModel#opdOp
	Q_PROPERTY(PropertyState opdOp READ opdOp NOTIFY opdOpChanged)
	//! \see SpeakerModel#bpsGov
	Q_PROPERTY(PropertyState bpsGov READ bpsGov NOTIFY bpsGovChanged)
	//! \see SpeakerModel#bpsOp
	Q_PROPERTY(PropertyState bpsOp READ bpsOp NOTIFY bpsOpChanged)
	//! \see SpeakerModel#opdJud
	Q_PROPERTY(PropertyState opdJud READ opdJud NOTIFY opdJudChanged)
	//! \see SpeakerModel#bpsJud
	Q_PROPERTY(PropertyState bpsJud READ bpsJud NOTIFY bpsJudChanged)

	Q_ENUMS(PropertyState)
	Q_ENUMS(PropertyId)
	//! Instead of offering setters for all propertie's, only one setter is given.
	//! \PropertyId is used to specify the property that should be set.
	//! \sa setMultiProperty
	enum PropertyId {
		OPD_FFR_ID, /*!< Used to edit SpeakerModel::opdFfr */
		OPD_GOV_ID, /*!< Used to edit SpeakerModel::opdGov */
		OPD_OP_ID,  /*!< Used to edit SpeakerModel::opdOp */
		BPS_GOV_ID, /*!< Used to edit SpeakerModel::bpsGov */
		BPS_OP_ID,  /*!< Used to edit SpeakerModel::bpsOpd */
		OPD_JUD_ID, /*!< Used to edit SpeakerModel::opdJud */
		BPS_JUD_ID, /*!< Used to edit SpeakerModel::bpsJud */
		IS_BEGINNER_ID /*!< Used to edit SpeakerModel::isBeginner */
	};
	//! Identifies the mixed state of a property of all speakers contained
	//! in the selection model.
	enum PropertyState {
		OFF, /*!< All properties are set to false */
		ON,  /*!< All properties are set to true */
		UNDEFINED,  /*!< The selection model is empty */
		MIXED /*!< The properties have multiple values */
	};

	PropertyState isBeginner() {return m_isBeginner;}
	PropertyState opdFfr() {return m_opdFfr;}
	PropertyState opdGov() {return m_opdGov;}
	PropertyState opdOp() {return m_opdOp;}
	PropertyState bpsGov() {return m_bpsGov;}
	PropertyState bpsOp() {return m_bpsOp;}
	PropertyState opdJud() {return m_opdJud;}
	PropertyState bpsJud() {return m_bpsJud;}

	void setSelectionModel(QListQmlWrapper *selectionModel);

	/*!
	 * \brief Sets a bool property of all currently selected speakers to a new value
	 * \param id identifies the property that should be set
	 * \param newState the properties new value
	 */

	Q_INVOKABLE void setMultiProperty(PropertyId id, bool newValue);

signals:
	void isBeginnerChanged();
	void opdFfrChanged();
	void opdGovChanged();
	void opdOpChanged();
	void bpsGovChanged();
	void bpsOpChanged();
	void opdJudChanged();
	void bpsJudChanged();

public slots:
	//!\internal
	void rowsInserted(QModelIndex, int start, int end);
	//!\internal
	void rowsRemoved(QModelIndex, int start, int end);

private slots:
	void someIsBeginnerChanged() {
		updateProperty(&SpeakerModel::isBeginner, &MixedValueSpeakerModel::setIsBeginner);
	}
	void someOpdFfrChanged() {updateProperty(&SpeakerModel::opdFfr, &MixedValueSpeakerModel::setOpdFfr);}
	void someOpdGovChanged() {updateProperty(&SpeakerModel::opdGov, &MixedValueSpeakerModel::setOpdGov);}
	void someOpdOpChanged() {updateProperty(&SpeakerModel::opdOp, &MixedValueSpeakerModel::setOpdOp);}
	void someBpsGovChanged() {updateProperty(&SpeakerModel::bpsGov, &MixedValueSpeakerModel::setBpsGov);}
	void someBpsOpChanged() {updateProperty(&SpeakerModel::bpsOp, &MixedValueSpeakerModel::setBpsOp);}
	void someOpdJudChanged() {updateProperty(&SpeakerModel::opdJud, &MixedValueSpeakerModel::setOpdJud);}
	void someBpsJudChanged() {updateProperty(&SpeakerModel::bpsJud, &MixedValueSpeakerModel::setBpsJud);}
private:
	void setIsBeginner(PropertyState state);
	void setOpdFfr(PropertyState state);
	void setOpdGov(PropertyState state);
	void setOpdOp(PropertyState state);
	void setBpsGov(PropertyState state);
	void setBpsOp(PropertyState state);
	void setOpdJud(PropertyState state);
	void setBpsJud(PropertyState state);

	void connectToSpeaker(SpeakerModel* model);
	void updateAll();
	void updateProperty(bool (SpeakerModel::*getter)(),
						void (MixedValueSpeakerModel::*setter)(PropertyState));

	bool m_updating;
	QListQmlWrapper *m_selectionModel;
	// used for caching
	PropertyState m_isBeginner, m_opdFfr, m_opdGov, m_opdOp, m_bpsGov, m_bpsOp, m_opdJud, m_bpsJud;
};

#endif // MIXEDVALUESPEAKERMODEL_H
