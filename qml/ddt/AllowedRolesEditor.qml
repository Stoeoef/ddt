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

import QtQuick 2.0
import ddt.model 1.0
import "SelectionHelper.js" as SH

/**
  * \brief An editor used to edit the allowed roles of a one ore more speakers.
  *
  * The editor allows easy and fast editing of some speakers allowed roles.
  * If some speakers have different property values, a mixed value will be shown.
  * Editing will set a property of all speakers to the same value.
  * \sa SpeakerModel MixedValueSpeakerModel
  */
Grid {
	id: container

	/// The editors model
	property MixedValueSpeakerModel mixedModel;

	rows: 5
	columns: 3

	Item {
		width: 1
		height: 1
	}

	TriCell {
		id: opdcell
		text: qsTr("OPD")
		state: SH.getMixedState(mixedModel.opdFfr, mixedModel.opdGov, mixedModel.opdOp, mixedModel.opdJud)
		onToggle: SH.setMultiPropertyById(mixedModel, newState == "on",
									   mouse.modifiers & Qt.ControlModifier,
									   MixedValueSpeakerModel.OPD_FFR_ID,
									   MixedValueSpeakerModel.OPD_GOV_ID,
									   MixedValueSpeakerModel.OPD_OP_ID,
									   MixedValueSpeakerModel.OPD_JUD_ID);
	}

	TriCell {
		id: bpscell
		text: qsTr("BPS")
		state: SH.getMixedState(mixedModel.bpsGov, mixedModel.bpsOp, mixedModel.bpsJud)
		onToggle: SH.setMultiPropertyById(mixedModel, newState == "on",
									   mouse.modifiers & Qt.ControlModifier,
									   MixedValueSpeakerModel.BPS_GOV_ID,
									   MixedValueSpeakerModel.BPS_OP_ID,
									   MixedValueSpeakerModel.BPS_JUD_ID);
	}

	TriCell {
		id: govcell
		text: qsTr("Gov.")
		state: SH.getMixedState(mixedModel.opdGov, mixedModel.bpsGov)
		onToggle: SH.setMultiPropertyById(mixedModel, newState == "on",
									   mouse.modifiers & Qt.ControlModifier,
									   MixedValueSpeakerModel.OPD_GOV_ID,
									   MixedValueSpeakerModel.BPS_GOV_ID)
	}

	TriCell {
		id: opdgovcell
		state: SH.getMixedState(mixedModel.opdGov)
		onToggle: SH.setMultiPropertyById(mixedModel, newState == "on",
									   mouse.modifiers & Qt.ControlModifier,
									   MixedValueSpeakerModel.OPD_GOV_ID)
	}

	TriCell {
		id: bpsgovcell
		state: SH.getMixedState(mixedModel.bpsGov)
		onToggle: SH.setMultiPropertyById(mixedModel, newState == "on",
									   mouse.modifiers & Qt.ControlModifier,
									   MixedValueSpeakerModel.BPS_GOV_ID)
		}

	TriCell {
		id: opcell
		text: qsTr("Op.")
		state: SH.getMixedState(mixedModel.opdOp, mixedModel.bpsOp)
		onToggle: SH.setMultiPropertyById(mixedModel, newState == "on",
									   mouse.modifiers & Qt.ControlModifier,
									   MixedValueSpeakerModel.OPD_OP_ID,
									   MixedValueSpeakerModel.BPS_OP_ID)
	}

	TriCell {
		id: opdopcell
		state: SH.getMixedState(mixedModel.opdOp)
		onToggle: SH.setMultiPropertyById(mixedModel, newState == "on",
									   mouse.modifiers & Qt.ControlModifier,
									   MixedValueSpeakerModel.OPD_OP_ID)

	}

	TriCell {
		id: bpsopcell
		state: SH.getMixedState(mixedModel.bpsOp)
		onToggle: SH.setMultiPropertyById(mixedModel, newState == "on",
									   mouse.modifiers & Qt.ControlModifier,
									   MixedValueSpeakerModel.BPS_OP_ID)
	}

	TriCell {
		id: judcell
		text: qsTr("Jud.")
		state: SH.getMixedState(mixedModel.opdJud, mixedModel.bpsJud)
		onToggle: SH.setMultiPropertyById(mixedModel, newState == "on",
									   mouse.modifiers & Qt.ControlModifier,
									   MixedValueSpeakerModel.OPD_JUD_ID,
									   MixedValueSpeakerModel.BPS_JUD_ID)
	}

	TriCell {
		id: opdjudcell
		state: SH.getMixedState(mixedModel.opdJud)
		onToggle: SH.setMultiPropertyById(mixedModel, newState == "on",
									   mouse.modifiers & Qt.ControlModifier,
									   MixedValueSpeakerModel.OPD_JUD_ID)
	}

	TriCell {
		id: bpsjudcell
		state: SH.getMixedState(mixedModel.bpsJud)
		onToggle: SH.setMultiPropertyById(mixedModel, newState == "on",
									   mouse.modifiers & Qt.ControlModifier,
									   MixedValueSpeakerModel.BPS_JUD_ID)
	}

	Item {
		width: 1
		height: 1
	}

	TriCell {
		id: opdffrcell
		text: qsTr("FFR");
		state: SH.getMixedState(mixedModel.opdFfr)
		onToggle: SH.setMultiPropertyById(mixedModel, newState == "on",
									   mouse.modifiers & Qt.ControlModifier,
									   MixedValueSpeakerModel.OPD_FFR_ID)
	}
}
