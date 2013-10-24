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
  * A simple round button with gradient background and hover animation.
  */
Rectangle {

	/// text displayed on the button's surface
	property alias text: textComp.text
	/// called when the button is clicked
	signal clicked

	id: container
	width: textComp.implicitWidth + 10
	height: 40
	radius: 35
	gradient: Gradient {
		GradientStop {
			position: 0
			color: "#a4a4a4"
		}

		GradientStop {
			position: 1
			color: "#777777"
		}
	}
	smooth: true
	border.width: 2
	border.color: "#000000"

	Text {
		id: textComp
		text: "text"
		verticalAlignment: Text.AlignVCenter
		horizontalAlignment: Text.AlignHCenter
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.verticalCenter: parent.verticalCenter
		font.pixelSize: 24
	}

	MouseArea {
		id: mouseArea
		smooth: false
		hoverEnabled: true
		anchors.fill: parent
		onEntered: container.state = "hovered"
		onExited: container.state = ""
		onClicked: container.clicked()
	}
	states: [
		State {
			name: "hovered"
			PropertyChanges {
				target: container
				border.width: 2
				border.color: "#a4aac6"
			}
		}
	]
}
