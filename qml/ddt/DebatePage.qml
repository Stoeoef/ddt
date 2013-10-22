import QtQuick 2.0
import ddt.model 1.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

/**
  * \brief Displays a component that allows editing the applications debate's.
  *
  * Allows adding and removing debates. The debates speaker counts and
  * room names can be edited, too.
  */
Item {
	id: container
	/// The application's mainModel
	property MainModel mainModel;

	/// A component with tool buttons that can be displayed in a tool bar
	property Component toolBarComponent: Row {
		ToolButton {
			height: 42; width: height
			action: container.addBpsDebateAction;
		}
		ToolButton {
			height: 42; width: height
			action: container.addOpdAction;
		}
	}

	/// An action that should be used when adding a BPS debate
	property Action addBpsDebateAction;
	/// An action that should be used when adding an OPD
	property Action addOpdAction;


	Rectangle {
		anchors.fill: parent
	}


	GridView {
		id: grid
		clip: true
		anchors.left: parent.left; anchors.right: parent.right
		anchors.top: parent.top; anchors.bottom: buttonRect.top


		// TODO: if set to true, onClick events are consumed by the flickable, disabling further event
		// processing. This prevents the user from unfocusing the room fields. Thus, flicking is disabled
		// until a proper way to handle those mouse events is found
		interactive: false
		cellWidth: 270
		cellHeight: 300

		anchors.margins: 5
		model: mainModel.debates

		removeDisplaced: Transition {
			NumberAnimation { properties: "x,y"; duration: 300 }
		}

		add: Transition {
			NumberAnimation { property: "opacity"; from: 0; to:1.0; duration: 500 }
		}


		remove: Transition {
			NumberAnimation { property: "opacity"; from: 1; to:0; duration: 200 }
		}

		delegate: Loader {
			id: loader

			property QtObject __debate: mainModel.debates ? mainModel.debates.getRow(index) : null
			sourceComponent: __debate ? (__debate.format === Debate.OPD ?
											 opdDebateView : bpsDebateView) : null

			Component {
				id: bpsDebateView
				Rectangle {
					color: "white"
					width: grid.cellWidth
					height: grid.cellHeight
					BPSDebateView {
						scale: 0.7
						anchors.centerIn: parent
						onRemove: mainModel.removeDebate(index)

						bpsDebate: loader.__debate && loader.__debate.format === Debate.BPS ?
									   loader.__debate : null
						editable: true
					}
				}
			}

			Component {
				id: opdDebateView

				Rectangle {
					width: grid.cellWidth
					height: grid.cellHeight
					color: "white"
					OPDDebateView {
						scale: 0.7
						anchors.centerIn: parent
						opDebate: loader.__debate && loader.__debate.format === Debate.OPD ?
									   loader.__debate : null
						onRemove: mainModel.removeDebate(index)
						editable: true
					}
				}
			}
		}

	}
	Row {
		height: 70
		id: buttonRect
		spacing: 10
		anchors.bottom: parent.bottom
		anchors.horizontalCenter: parent.horizontalCenter
		Button {
			id: addOpdDebate
			anchors.verticalCenter: parent.verticalCenter
			height: 50
			width: 160
			action: addOpdAction
		}

		Button {
			id: addBPSDebate
			anchors.verticalCenter: parent.verticalCenter
			width: addOpdDebate.width
			height: addOpdDebate.height
			action: addBpsDebateAction
		}

	}

}
