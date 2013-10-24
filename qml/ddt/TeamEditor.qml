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
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import ddt.model 1.0
import "SelectionHelper.js" as SH

/**
  * \brief An editor allowing the editing of teams.
  *
  * The editor allows for operations:
  *		- Add all selected speakers (according to a selection model) to some team
  *		- Move a speaker into another team (by drag & drop)
  *		- remove a team (the speakers won't be removed)
  *		- Edit a team's name
  */
ListView {
	id: container
	clip: true
	focus: false
	anchors.left: parent.left; anchors.right: parent.right
	height: 130
	/// A List containing all current \ref Team objects.
	property QListQMLWrapper teams
	/// A selection model holding the currently selected \ref SpeakerModel objects.
	property QListQMLWrapper selectionModel
	model: teams

	// line in team editor
	delegate: Item {
		id: teamDelegate
		height: 30;
		width: parent.width;
		property bool _dragActive: false
		// make index visible for internal memberList ("index" identifier will be overriden)
		property int _teamIndex: index
		z: _dragActive ? 1 : 0

		DropArea {
			id: dropArea
			anchors.fill: parent
		}

		// highlights current drop target
		Rectangle {
			anchors.fill: parent
			opacity: dropArea.containsDrag ? 1. : 0
			border.width: 1
			border.color: "lightgray"
			radius: 5
			Behavior on opacity {
				NumberAnimation{}
			}

			color: "lightblue";
		}

		RowLayout {
			id: delegate
			focus: false
			spacing: 10
			anchors.fill: parent

			Rectangle {
				width: 90
				height: 30
				Layout.fillHeight: true
				z: 1
				FocusAndEditTextField {
					id: teamNameField
					anchors.fill: parent
					onTextChanged: teamName = text
					text: teamName
					alternativeText: text
					clip: false
				}
			}

			ListView {
				id: memberList
				Layout.fillWidth: true;
				anchors.verticalCenter: parent.verticalCenter
				//Layout.fillHeight: true;
				height: parent.height - 10
				anchors.leftMargin: 10
				spacing: 5
				clip: false
				orientation: ListView.Horizontal

				model: ListModel {
					id: listMemberModel
				}

				Component.onCompleted: updateMemberListModel();
				function updateMemberListModel() {
					listMemberModel.clear();
					var teamSize = teams.getRow(index).speakerCount();
					for (var i = 0; i < teamSize; i++) {
						listMemberModel.append({"speaker": teams.getRow(_teamIndex).getSpeaker(i)});
					}
					memberList.model = listMemberModel;
				}

				// Entry in team member list
				delegate {
					Rectangle {
						id: delegate
						Drag.active: dragMouseArea.drag.active
						Drag.proposedAction: Qt.MoveAction
						width: childrenRect.width + 10
						height: memberName.implicitHeight + 4
						property int _index: index
						property bool _selected: selectionModel.contains(speaker)
						Connections {
							target: selectionModel
							onListChanged: delegate._selected = selectionModel.contains(speaker)
						}

						border.width: 1
						border.color: "lightgray"
						radius: 4

						gradient: Gradient {
							GradientStop {
								color: "#eeeeee"
								position: 0
							}
							GradientStop {
								color: _selected ? "lightblue" : "#898989"
								position: 1.
							}
						}

						Text {
							id: memberName
							anchors.verticalCenter: parent.verticalCenter
							anchors.left: parent.left; anchors.leftMargin: 4
							font.pixelSize: 16
							text: speaker.name

							MouseArea {
								id: dragMouseArea
								anchors.fill: parent
								drag.target: delegate
								drag.onActiveChanged: teamDelegate._dragActive = drag.active
								cursorShape: delegate.Drag.target ? Qt.DragMoveCursor : Qt.PointingHandCursor

								onReleased: {
									if (mouse.isClick) {
										var shift = mouse.modifiers & Qt.ShiftModifier

										if (!shift) {
											selectionModel.clear();
											selectionModel.appendRow(speaker);
										} else {
											if (delegate._selected)
												selectionModel.remove(speaker);
											else {
												selectionModel.appendRow(speaker);
											}
										}
									}
									else if (!Drag.active) {
										if (delegate.Drag.target) {
											// drop team member into new team
											speaker.team =
													teams.getRow(delegate.Drag.target.parent._teamIndex);
										} else {
											delegate.Drag.cancel();
										}
										// resets the item's graphical position
										memberList.updateMemberListModel();
									}
								}
							}
						}
						Button {
							text: "x"
							width: 20
							height: 20
							anchors.left: memberName.right; anchors.leftMargin: 4
							anchors.verticalCenter: parent.verticalCenter
							onClicked: teams.getRow(teamDelegate._teamIndex).removeSpeaker(index);
						}
					}
				}
				Connections {
					target: teams.getRow(index);
					onSpeakersChanged: {memberList.updateMemberListModel();}

				}
			}

			Rectangle {
				z: 1
				width: childrenRect.width;
				Layout.fillHeight: true
				ToolButton {
					id: deleteTeamButton
					width: height; height: addCurrentToTeamButton.height
					onClicked: teams.removeRow(index);
					tooltip: "Delete team"
					iconSource: "images/delete.svg"
				}
				ToolButton {
					id: addCurrentToTeamButton
					anchors.left: deleteTeamButton.right
					width: height; height: 30
					onClicked: SH.setMultiPropertyByName(selectionModel, ["team"], teams.getRow(index));
					tooltip: "Add to team"
					iconSource: "images/addtoteam.svg"
					text: qsTr("Add to team")
				}
			}
		}
	}
}
