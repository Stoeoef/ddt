import QtQuick 2.0
import ddt.model 1.0
import QtQuick.Controls 1.0

/**
  * Displays the results of a mapping process.
  *
  * Shows the mapping between speakers and seats. Furthermore,
  * the configuration of different rooms can be edited.
  */
Rectangle {
	id: container

	/// The application's main model
	property MainModel mainModel
	/// Specifies which debate is currently selected.
	/// \sa MainModel#debates
	property int currentDebateIndex: 0
	/// A \c MainModel stores multiple precalculated solutions. This
	/// property specifies which solution is displayed.
	property int currentSolutionIndex

	/// A component containing control elements for this page. Should
	/// Be used in a \c ToolBar
	property Component toolBarComponent: Row {
		anchors.centerIn: parent
		spacing: 10

		ToolButton {
			tooltip: qsTr("Go to previous room")
			iconSource: "images/left.svg"
			anchors.top: parent.top; anchors.bottom: parent.bottom
			onClicked: currentDebateIndex--
			enabled: currentDebateIndex > 0 &&
					 mainModel.solutionState == MainModel.SOLUTION_EXISTS
		}

		SpinBox {
			id: spinBox
			height: 42
			minimumValue: 1
			maximumValue: mainModel.solutionCount
			enabled: mainModel.solutionCount > 0
			value: container.currentSolutionIndex + 1
			onValueChanged: {
				currentSolutionIndex = value - 1;
			}

			anchors.top: parent.top; anchors.bottom:  parent.bottom
			width: 70

			Connections {
				target: mainModel
				onSolutionCountChanged: {
					container.currentSolutionIndex = mainModel.solutionCount - 1;
				}
			}
		}

		Text {
			text: switch (mainModel.solutionState) {
				  case (MainModel.SOLUTION_EXISTS):
					  return qsTr("Solution count: %1").arg(mainModel.solutionCount);
				  case (MainModel.UNKNOWN):
					  return "";
				  case (MainModel.NO_SOLUTION_EXISTS):
					  return qsTr("No solution exists");
				  case (MainModel.CALCULATING):
					  return qsTr("Calculating...");
				  }
			verticalAlignment: Text.AlignVCenter
			anchors.bottom: parent.bottom; anchors.top: parent.top
			font.pixelSize: 30
		}

		ToolButton {
			height: 42; width: height
			action: generateSolutionAction
		}

		ToolButton {
			height: 42; width: height
			action: interruptCalculationAction
		}

		ToolButton {
			tooltip: qsTr("Go to next room")
			iconSource: "images/right.svg"
			anchors.top: parent.top; anchors.bottom: parent.bottom
			onClicked: currentDebateIndex++
			enabled: mainModel && currentDebateIndex < mainModel.debates.rowCount - 1
					 && mainModel.solutionState == MainModel.SOLUTION_EXISTS
		}
	}

	/// An action that starts the calculation of a new solution
	property Action generateSolutionAction
	/// An action that interrupts the current calculation
	property Action interruptCalculationAction

	function currentDebate() {
		return mainModel.debates.rowCount > 0 ? mainModel.debates.getRow(currentDebateIndex) : null
	}

	Connections {
		target: mainModel
		onSolutionCountChanged: {
			opdView.mapper
			= Qt.binding (function() {return mainModel.getSolution(container.currentSolutionIndex);});
		}
	}

	BPSDebateView {
		id: bpsView
		visible: mainModel.solutionState === MainModel.SOLUTION_EXISTS && bpsDebate
		editable: false
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.top: parent.top
		transformOrigin: Item.Top
		scale: 1.6
		bpsDebate: currentDebate() && currentDebate().format === Debate.BPS ? currentDebate() : null
		mapper: opdView.mapper
	}

	OPDDebateView {
		id: opdView
		editable: false
		visible: mainModel.solutionState === MainModel.SOLUTION_EXISTS && opDebate
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.top: parent.top
		transformOrigin: Item.Top
		scale: bpsView.scale
		opDebate: currentDebate() && currentDebate().format === Debate.OPD ? currentDebate() : null
		mapper: mainModel.getSolution(container.currentSolutionIndex)
	}
	ListView {
		id: errorList
		spacing: 10
		anchors.left: parent.left; anchors.right: parent.right
		anchors.leftMargin: 20; anchors.rightMargin: 20
		anchors.fill: parent
		anchors.topMargin: 25
		visible: mainModel.solutionState === MainModel.NO_SOLUTION_EXISTS
		model: mainModel.errors()

		header: Text {
			text: qsTr("Reasons:");
			font.pixelSize: 30
			font.italic: true
		}

		delegate: Text {
			text: modelData
			color: "red"
			font.pixelSize: 20
		}

		onVisibleChanged: {
			if (visible) {
				model = mainModel.errors();
				headerItem.visible = model.length > 0
			}
		}
	}
}
