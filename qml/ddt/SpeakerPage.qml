// import QtQuick 2.00 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import ddt.model 1.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.0
import "SelectionHelper.js" as SH

/**
  * \brief A component that allows the selection and editing of on ore more speakers.
  *
  * The component consists of two parts: A \c SpeakerList , allowing to select various
  * speakers, and an \c AllowedRolesEditor which allows the editing of the selected
  * speakers properties. Additionally, a \c TeamEditor is used which enables the editing
  * of teams.
  */
Rectangle {
	id: container

	/// The page's toolbar additions
	property Component toolBarComponent: Row {
		ToolButton {
			height: 42; width: height
			action: container.addSpeakerAction;
		}
	}

	/// The application's selection model. This is a list containing \c SpeakerModel objects.
	property QListQMLWrapper selectionModel
	/// The application's \c MainModel
	property MainModel mainModel
	/// An action that adds a speaker
	property Action addSpeakerAction
	Keys.forwardTo: list
	SplitView {
		anchors.fill: parent

		handleDelegate: Rectangle {
			width: 8
			height: 1
			color: Qt.darker("darkGrey")
		}

		ScrollView {
			Layout.minimumWidth: 200
			width: 300

			// embed list in an item to allow focus loss
			// when clicking in an empty region
			SpeakerList {
				id: list
				anchors.left: parent.left; anchors.right: parent.right;
				anchors.top: parent.top;

				// bind height on completion, otherwise we have a binding loop
				Component.onCompleted: {
					height = Qt.binding(function() {return contentHeight;});
				}

				mainModel: container.mainModel
				selectionModel: container.selectionModel;
			}
		}

		Column {
			id: column
			spacing: 15
			Layout.minimumWidth: 200

			state: "selectionEmpty"
			Connections {
				target: selectionModel
				onRowCountChanged: column.state = selectionModel.rowCount == 0 ? "selectionEmpty" : "notEmpty"
			}

			move: Transition {
				id: moveAnimation
				enabled: false
				NumberAnimation { properties: "x,y"; easing.type: Easing.OutCubic; duration: 400 }
			}

			transitions: [
				Transition {
					from: "*"
					to: "selectionEmpty"
					SequentialAnimation {
						PropertyAction {target: moveAnimation; property: "enabled"; value: true}
						NumberAnimation {property: "opacity"; duration: 200}
						PropertyAction {property: "visible"}
						PauseAnimation {duration: 400}
						PropertyAction {target: moveAnimation; property: "enabled"; value: false}
					}
				},
				Transition {
					from: "*"
					to: "notEmpty"
					SequentialAnimation {
						PropertyAction { target: moveAnimation; property: "enabled"; value: true }
						PropertyAction{property: "visible";}
						PauseAnimation {duration: 400}
						NumberAnimation {property: "opacity"; duration: 200}
						PropertyAction { target: moveAnimation; property: "enabled"; value: false}
					}
				}

			]

			states: [
				State {
					name: "selectionEmpty"
					PropertyChanges {target: hbar1; opacity: 0.; visible: false}
					PropertyChanges {target: hbar2; opacity: 0.; visible: false}
					PropertyChanges {target: hbar3; opacity: 0.; visible: false}
					PropertyChanges {target: allowedRolesHeader; opacity: 0.; visible: false}
					PropertyChanges {target: allowedRolesRow; opacity: 0.; visible: false}
					PropertyChanges {target: isBeginnerRow; opacity: 0.; visible: false}
				},
				State {
					name: "notEmpty"
					PropertyChanges {target: hbar1; opacity: 1.; visible: true}
					PropertyChanges {target: hbar2; opacity: 1.; visible: true}
					PropertyChanges {target: hbar3; opacity: 1.; visible: true}
					PropertyChanges {target: allowedRolesHeader; opacity: 1.; visible: true}
					PropertyChanges {target: allowedRolesRow; opacity: 1.; visible: true}
					PropertyChanges {target: isBeginnerRow; opacity: 1.; visible: true}
				}

			]

			Text {
				anchors.right: parent.right; anchors.left: parent.left
				//: header of speaker options page
				text: qsTr("Options")
				horizontalAlignment: Text.AlignHCenter
				clip: true;
				font.pixelSize: 40
			}

			Text {
				id: nameField
				font.italic: true
				font.pixelSize: 25
				horizontalAlignment: Text.AlignHCenter
				clip: true;
				elide: Text.ElideRight
				text: setNames();
				function setNames() {
					if (selectionModel.rowCount == 0)
						return text = qsTr("Select speakers to edit their options...");
					var names = SH.getPropertyArray(selectionModel, "name");
					return text = names.join(", ");
				}

				Connections {
					target: selectionModel
					onListChanged: nameField.setNames();
				}

				anchors.right: parent.right
				anchors.left: parent.left
			}

			HBar {id: hbar1}

			Text {
				id: allowedRolesHeader
				//: header of allowed roles section
				text: qsTr("Allowed roles:")
				anchors.left: parent.left; anchors.leftMargin: 10
				font.pixelSize: 20
			}

			Row {
				id: allowedRolesRow
				anchors.left: parent.left; anchors.right: parent.right;
				anchors.leftMargin: 10; spacing: 20

				AllowedRolesEditor {
					id: allowedRoles
					mixedModel: MixedValueSpeakerModel {
						selectionModel: container.selectionModel
					}
				}
				Text {
					id: allowedRolesDescription
					anchors.verticalCenter: parent.verticalCenter
					text: "Use Ctrl+Click to select\na role exclusively"
					font.pixelSize: 20
					color: "gray"
				}
			}

			HBar {id: hbar2}

			Row {
				id: isBeginnerRow
				anchors.right: parent.right; anchors.rightMargin: 4
				anchors.left: parent.left; anchors.leftMargin: 4
				spacing: 40

				CheckBox {
					id: isBeginnerBox

					partiallyCheckedEnabled: true
					text: qsTr("Is beginner?")
					onClicked: {
						var newValue = allowedRoles.mixedModel.isBeginner !=
								MixedValueSpeakerModel.ON;
						SH.setMultiPropertyById(allowedRoles.mixedModel, newValue, false,
											 MixedValueSpeakerModel.IS_BEGINNER_ID);
						checkedState = getCheckedState();
					}

					Connections {
						target: allowedRoles.mixedModel
						onIsBeginnerChanged: isBeginnerBox.checkedState =
											 isBeginnerBox.getCheckedState();
					}

					function getCheckedState() {
						switch (allowedRoles.mixedModel.isBeginner) {
						case MixedValueSpeakerModel.ON: return Qt.Checked
						case MixedValueSpeakerModel.OFF: return Qt.Unchecked
						default: return Qt.PartiallyChecked
						}
					}

					checkedState: getCheckedState();
				}

				Text {
					color: "gray"
					//: use a linebreak in the middle of the sentence
					text: qsTr("Beginners won't be set\non the same table")
					anchors.topMargin: 3
					anchors.top: parent.top
					font.pixelSize: 20
				}
			}

			HBar{id: hbar3}

			Text {
				font.pixelSize: 20
				anchors.left: parent.left; anchors.leftMargin: 10
				//: header of teams section
				text: qsTr("Teams:")
			}

			TeamEditor {
				selectionModel: container.selectionModel
				teams: mainModel.teams;
			}

			Button {
				text: qsTr("Assign to new team")
				width: implicitWidth + 10
				anchors.left: parent.left; anchors.leftMargin: 10
				iconSource: "images/newteam.svg"
				onClicked: SH.setMultiPropertyByName(selectionModel, ["team"], mainModel.addTeam());
			}
		}
	}
}
