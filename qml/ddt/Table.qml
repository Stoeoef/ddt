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

/**
  * \brief Displays a debate's table and all enabled team seats.
  *
  * A table in a debate specifies how many seats are reserved for
  * a specific team (e.g. how many judicators or speakers of the
  * government take part in a debate). Furthermore, the number of
  * seats can be edited.
  * The individual seats may be labeled with an arbitrary string,
  * usually the speaker's name.
  */
Item {
	id: container
	width: implicitWidth
	height: implicitHeight

	/// This property is either \c null or maps an arbitrary label
	/// to each seat. This label is displayed at each seat.
	property TableNames names;

	/// Holds how many seats are currently being displayed.
	property int seatCount: maxSeatCount;
	/// Specifies if the seat count can be edited.
	property bool editable: true
	/// Holds a text that is displayed within the table. This
	/// can be used to display which team is seated at this table.
	property alias tableText: tableText.text
	/// Holds how many seats should be set in a perfectly set debate.
	/// This will influence the displayed length of the table.
	property int preferredSeatCount: minSeats
	/// Specifies how many seats the table can hold at most.
	property int maxSeatCount: 3
	/// Internal
	property int __seatDiameter: 35
	/// The table's rotation. Use this instead of \c Item.rotation to
	/// achieve a better useable and readable rotation.
	property alias tableRotation: rotatable.rotation
	/// The table's rotation origin. Use this instead of \c Item.transformOrigin
	/// to achieve a better useable and readable rotation.
	property alias rotationOrigin: rotatable.transformOrigin

	/// Called whenever the user indicates that he or she wants to add a new seat
	/// to the table.
	signal increment
	/// Called whenever the user indicates that he or she wants to remove a seat
	/// from the table.
	signal decrement

	Item {
		id: rotatable

		transformOrigin: Item.Left
		width: table.width
		height: table.height


		Rectangle {
			id: table
			anchors.left: list.right
			anchors.leftMargin: list.spacing
			width: __seatDiameter * 1.3

			anchors.top: list.top
			anchors.bottomMargin: list.spacing
			height: list.height
			color: "#278422"
			border.color: "#000000"

			Text {
				id: tableText
				anchors.centerIn: parent
				font.pixelSize: __seatDiameter
				color: "#BBBBBB"

				rotation: 90
			}

			Behavior on height {
				NumberAnimation {}

			}
		}

		Component {
			id: delegate
			Item {
				height: circle.height
				width: childrenRect.width
				Rectangle {
					id: circle

					width: container.__seatDiameter
					height: width
					color: "#61be8e"

					radius: width
					border.color: "#000000"
					MouseArea {
						anchors.fill: parent
						enabled: editable
						onClicked: decrement();
						cursorShape: editable ? Qt.PointingHandCursor : Qt.ArrowCursor
					}
				}
				Text {
					id: name
					transform: Rotation {
						// set the circle as rotation center
						origin.x: name.getRot() < 90 ? container.__seatDiameter / 2 + width - 35 :
													   - container.__seatDiameter / 2 - name.anchors.leftMargin
						origin.y: height / 2
						// rotates the text into it's original position
						angle: -container.tableRotation
					}


					function getRot() {
						return Math.abs(container.tableRotation);
					}

					Component.onCompleted: {
						if (getRot() < 90)
							anchors.right = circle.left
						else
							anchors.left = circle.right
					}
					anchors.leftMargin: 10
					anchors.top: circle.top

					anchors.rightMargin: 10
					font.pixelSize: 30
					text: {
						if (container.names && container.names.getSpeaker(index)) {
							return container.names.getSpeaker(index).name;
						}
						return "";
					}
				}
			}
		}

		ListView {
			id: list

			add: Transition {
				NumberAnimation { property: "x"; from: -300; duration: 500; easing {
						type: Easing.OutElastic
						amplitude: 1.0
						period: 1
					}}

			}


			remove: Transition {
				NumberAnimation { property: "x"; to: -300; duration: 500 }
			}

			removeDisplaced: Transition {
				NumberAnimation {
					properties: "x, y"; duration: 250;
				}
			}

			interactive: false
			spacing: 10
			model: seatCount
			height: Math.max(preferredSeatCount * (__seatDiameter + list.spacing), childrenRect.height)
			width: __seatDiameter
			delegate: delegate
		}

		function getAddVisibility() {
			return editable && seatCount < container.maxSeatCount
		}

		RoundButton {
			id: addButton
			onClicked: container.increment()
			text: "+"
			radius: width
			visible: rotatable.getAddVisibility()
			width: __seatDiameter
			height: width
			transformOrigin: Item.Center
			rotation: -container.tableRotation
			anchors.horizontalCenter: list.horizontalCenter
			y: list.childrenRect.height + list.spacing
			Behavior on y {
				NumberAnimation {}

			}
		}
	}
}

