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
import QtQuick.Controls 1.0

/**
  * \brief Displays a BPS debate.
  *
  * Allows editing the number of speakers participating in the debate.
  */
Rectangle {
	id: container
	width: 335
	height: 330 + (mapper ? 70 : 10)

	/// The debate that is displayed.
	property BPSDebate bpsDebate
	/// \c true if the room and the number of speakers should be editable.
	property bool editable;
	// The speakers names are shown if this mapper is set.
	property SeatToSpeakerMapper mapper;

	// Called when the user indicates to remove the debate shown by this component.
	signal remove

	FocusAndEditTextField {
		id: roomField
		editable: container.editable
		anchors.horizontalCenter: parent.horizontalCenter
		horizontalAlignment: Text.AlignHCenter
		width: 120
		height: 48
		font.pixelSize: 40
		anchors.top: parent.top
		alternativeText: text
		text: bpsDebate ? bpsDebate.room : ""
		onTextChanged: {if (bpsDebate) bpsDebate.room = text}
	}

	ToolButton {
		iconSource: "images/delete.svg"
		tooltip: qsTr("Remove this debate")
		width: 40
		height: width
		scale: 1

		visible: editable
		anchors.top: parent.top
		anchors.right: parent.right
		anchors.margins: 8
		onClicked: parent.remove()
	}



	Table {
		id: gov1Table
		names: mapper ? mapper.getNamesForTable(bpsDebate, Debate.BPS_GOV1) : null
		tableText: qsTr("GOV")
		preferredSeatCount: 2
		maxSeatCount: 2
		y: 50
		editable: container.editable
		seatCount: bpsDebate ? bpsDebate.gov1Places : 0
		onIncrement: bpsDebate.gov1Places++
		onDecrement: bpsDebate.gov1Places--
	}

	Table {
		id: gov2Table
		y: 150
		names: mapper ? mapper.getNamesForTable(bpsDebate, Debate.BPS_GOV2) : null
		tableText: qsTr("GOV")
		editable: container.editable
		preferredSeatCount: 2
		maxSeatCount: 2
		seatCount: bpsDebate ? bpsDebate.gov2Places : 0
		onIncrement: bpsDebate.gov2Places++
		onDecrement: bpsDebate.gov2Places--
	}


	Table {
		id: op1Table
		x: 330
		y: 50
		names: mapper ? mapper.getNamesForTable(bpsDebate, Debate.BPS_OP1) : null
		tableRotation: 180
		tableText: qsTr("OP")
		editable: container.editable
		preferredSeatCount: 2
		maxSeatCount: 2
		seatCount: bpsDebate ? bpsDebate.op1Places : 0
		onIncrement: bpsDebate.op1Places++
		onDecrement: bpsDebate.op1Places--

	}

	Table {
		id: op2Table
		names: mapper ? mapper.getNamesForTable(bpsDebate, Debate.BPS_OP2) : null
		y: 150
		x: 330
		tableRotation: 180
		tableText: qsTr("OP")
		editable: container.editable
		preferredSeatCount: 2
		maxSeatCount: 2
		seatCount: bpsDebate ? bpsDebate.op2Places : 0
		onIncrement: bpsDebate.op2Places++
		onDecrement: bpsDebate.op2Places--
	}


	Table {
		id: judTable
		y: mapper ? 120 : 314
		x: mapper ? 230 : 70
		rotationOrigin: mapper ? Item.Bottom : Item.Top
		names: mapper ? mapper.getNamesForTable(bpsDebate, Debate.JUDICATOR) : null
		tableRotation: mapper ? -135 : -90
		tableText: qsTr("Jud")
		editable: container.editable
		preferredSeatCount: 3
		maxSeatCount: 4
		seatCount: bpsDebate ? bpsDebate.judicatorPlaces : 0
		onIncrement: bpsDebate.judicatorPlaces++
		onDecrement: bpsDebate.judicatorPlaces--
	}
}
