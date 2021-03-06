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

/**
  * \brief A styled cell with three different possible states.
  *
  * This component is functional similiar to a tri-state check box.
  * The states are indicated with three different colors, the cell
  * has a hover animation.
  * The user can read and set the component's state with the \c state
  * property. Three values are allowed:
  *		-\c "off": indicates that the cell is inactive
  *		-\c "on": indicates that the cell is active
  *		-\c "mixed": indicates that the cell state is unknown
  */
Item {
	id: cell
	width: 94
	height: 32
	/// A text displayed within the cell
	property alias text: label.text
	/// called when the state should be toggled (by clicking in the cell)
	signal toggle (string newState, var mouse)

	/// Set this to \c true if clicking in the cell should toggle
	/// the component's state.
	property bool autoToggle: false

	function toggleState() {
		return state != "on" ? "on" : "off"
	}

	transitions: [
		Transition {
			from: "*"
			to: "*"
			ColorAnimation { easing.type: Easing.Linear; target: gradientstop1; duration: 300 }
		}
	]

	Rectangle {
		id:rect
		gradient: Gradient {
			GradientStop {
				id: gradientstop1
				position: 0
				color: "#000000"
			}

			GradientStop {
				position: 1
				color: "#555555"
			}
		}





		anchors.fill: parent

		border.width: 1
		border.color: "#808080"

		MouseArea {
			id: mouse_area1
			anchors.fill: parent
			hoverEnabled: true
			onEntered: rect.border.color = "#aaaaaa"
			onExited: rect.border.color = "#808080"
			onClicked: {cell.toggle(toggleState(), mouse); if (cell.autoToggle) {
					cell.state = toggleState()
				}
			}

			Text {
				id: label
				color: "#eeeeee"
				anchors.fill: parent
				horizontalAlignment: Text.AlignHCenter
				verticalAlignment: Text.AlignVCenter
				styleColor: "#c0c0c0"
				font.pixelSize: 22
			}

		}
	}


	states: [
		State {
			name: "on"

			PropertyChanges {
				target: gradientstop1
				position: 0
				color: "#4ac428"
			}


		},
		State {
			name: "off"

			PropertyChanges {
				target: gradientstop1
				position: 0
				color: "#c42929"

			}

		},
		State {
			name: "mixed"
			PropertyChanges {
				target: gradientstop1
				position: 0
				color: "#F2E06D"

			}
		}
	]

}
