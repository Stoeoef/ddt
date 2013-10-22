// import QtQuick 2.00 // to target S60 5th Edition or Maemo 5
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
