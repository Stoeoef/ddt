import QtQuick 2.0
import ddt.model 1.0
import QtQuick.Controls 1.0

/**
  * \brief Displays an OP debate.
  *
  * Allows editing the number of speakers participating in the debate.
  */
Rectangle {

	id: container
	width: 330
	height: 395

	/// Holds if the number of speakers can be edited
	property bool editable: false
	/// The underlying data model
	property OPDDebate opDebate
	/// Maps positions to speaker names. The names will be displayed next to the speakers.
	property SeatToSpeakerMapper mapper

	/// Called when the user indicates that the diplayed debate should be removed.
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
		text: opDebate ? opDebate.room : ""
		onTextChanged: {if (opDebate) opDebate.room = text}
	}

	ToolButton {
		iconSource: "images/delete.svg"
		tooltip: qsTr("Remove this debate");
		width: 40
		height: width

		visible: editable
		anchors.top: parent.top
		anchors.right: parent.right
		anchors.margins: 8
		onClicked: parent.remove()
	}


	Table {
		id: govTable
		y: 50
		names: mapper ? mapper.getNamesForTable(opDebate, Debate.OPD_GOV) : null;
		tableText: qsTr("GOV")
		editable: container.editable
		preferredSeatCount: 3
		maxSeatCount: 3
		seatCount: opDebate ? opDebate.govPlaces : 0

		onIncrement: opDebate.govPlaces++
		onDecrement: opDebate.govPlaces--
	}

	Table {
		id: opTable
		names: mapper ? mapper.getNamesForTable(opDebate, Debate.OPD_OP) : null;
		x: 330
		y: 50
		editable: container.editable
		tableRotation: 180
		tableText: qsTr("OP")
		preferredSeatCount: 3
		maxSeatCount: 3
		seatCount: opDebate ? opDebate.opPlaces : 0
		onIncrement: opDebate.opPlaces++
		onDecrement: opDebate.opPlaces--
	}

	Table {
		id: ffrTable
		names: mapper ? mapper.getNamesForTable(opDebate, Debate.OPD_FFR) : null;
		y: 265
		x: -10
		editable: container.editable
		tableRotation: -45
		rotationOrigin: Item.Top
		tableText: qsTr("FFR")
		preferredSeatCount: 3
		maxSeatCount: 5
		seatCount: opDebate ? opDebate.ffrPlaces : 0
		onIncrement: opDebate.ffrPlaces++
		onDecrement: opDebate.ffrPlaces--
	}

	Table {
		id: judTable
		names: mapper ? mapper.getNamesForTable(opDebate, Debate.JUDICATOR) : null;
		y: 365
		x: 195
		rotationOrigin: Item.Top
		editable: container.editable
		tableRotation: -135
		tableText: qsTr("Jud")
		preferredSeatCount: 3
		maxSeatCount: 4
		seatCount: opDebate ? opDebate.judicatorPlaces : 0
		onIncrement: opDebate.judicatorPlaces++
		onDecrement: opDebate.judicatorPlaces--
	}
}
