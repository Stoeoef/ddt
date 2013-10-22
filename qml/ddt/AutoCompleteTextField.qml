import QtQuick 2.0

import QtQuick 2.0
import QtQuick.Controls 1.0
import ddt.model 1.0

/**
  * \brief A text field with autocompletion.
  *
  * Possible entries for autocompletion are shown in a list below the text field.
  */
FocusScope {
	id: container
	/// Used to obtain possible completions
	property AutoCompleteModel completeModel
	/// type:string the text field's text
	property alias text: field.text
	/// type:string shown if text is empty
	property alias alternativeText: alternativeText.text
	/// type:bool \c true if focus can be gained by clicking in the text field
	property alias activeFocusOnPress: field.activeFocusOnPress
	/// type: bool use clipping to prevent long text being rendered outside the component
	property alias clip: field.clip
	/// true if the text field is hidden by default. The alternative text is shown instead.
	/// Gaining focus will show the text field.
	property bool preferAlternativeText: false
	/// Emitted when return is pressed while the text field has focus.
	/// @param type: KeyEvent the key event
	signal returnPressed(var event)

	width: 200
	height: field.height

	/// Accepts the currently selected auto completion.
	function acceptCurrentCompletion() {
		if (lv.currentIndex >= 0) {
			var completed = lv.model[lv.currentIndex];
			if (completed)
				text = completed
		}
		container.focus = false;
	}

	TextField {
		id: field
		clip:true
		focus: true
		font.pixelSize: 17
		height: font.pixelSize + 14
		anchors.left: parent.left; anchors.right: parent.right

		opacity: 0
		onTextChanged: lv.model = completeModel.completions(text)

		onActiveFocusChanged: {container.focus = activeFocus; if (activeFocus) selectAll()}

		Keys.onEscapePressed: {
			container.focus = false
		}

		Keys.onDownPressed: {
			if (lv.model && lv.model.length !== 0) {
				lv.incrementCurrentIndex();
			} else {
				event.accepted = false;
			}
		}

		Keys.onUpPressed: {
			if (lv.model && lv.model.length !== 0) {
				lv.decrementCurrentIndex();
			} else {
				event.accepted = false;
			}
		}

		Keys.onReturnPressed: {
			container.acceptCurrentCompletion();
			container.returnPressed(event);
		}


		Text {
			id: completionText
			focus: false
			text: {
				if (lv.currentIndex >= 0) {
					var completed = lv.model[lv.currentIndex];
					if (!completed)
						return ""
					return completed.substring(field.length);
				}
				return "";
			}
			color: "#aaaaaa";
			clip: container.clip;
			visible: field.activeFocus
			anchors.left: txtCopy.right
			anchors.verticalCenter: parent.verticalCenter
			anchors.leftMargin: 5
			font.pixelSize: field.font.pixelSize
		}

		Text {
			id: txtCopy
			// placeholder to determine the completion text position
			focus: false
			text: field.text
			visible: false
			font.pixelSize: field.font.pixelSize
		}
	}
	Rectangle {
		color: "white"
		focus: false
		anchors.fill: lv
		z: -2
		border.width: 1
		visible: lv.visible
	}

	Text {
		id: alternativeText
		focus: false
		anchors.verticalCenter: parent.verticalCenter
		anchors.leftMargin: horizontalAlignment == Text.AlignLeft ? 10 : 0
		anchors.right: container.right; anchors.left: parent.left
		color: "gray"
		clip: container.clip
		font.italic: true
		font.pixelSize: field.font.pixelSize
	}

	ListView {
		id: lv
		z: -1
		focus: false
		visible: field.activeFocus
		anchors.left: parent.left; anchors.right: parent.right
		anchors.top: field.bottom
		keyNavigationWraps: true
		highlight: Rectangle {
			color: "lightblue"
			width: lv.width - 2
			x: 1
		}
		clip: true
		Behavior on height {NumberAnimation{}}
		// Fixed binding produces a binding loop warning, bind on load
		Component.onCompleted: {
			height = Qt.binding(function () {return contentHeight});
		}

		delegate: Text {
			id: delegate
			width: parent.width
			text: modelData
			font.pixelSize: 20
			MouseArea {
				anchors.fill: parent
				onClicked: {lv.currentIndex = index; container.acceptCurrentCompletion()}
			}
		}
	}

	states: State {
		name: "hasText"; when:
			((!preferAlternativeText && textField.text != '') || textField.activeFocus)
		PropertyChanges { target: alternativeText; opacity: 0 }
		PropertyChanges { target: field; opacity: 1 }

	}

	transitions: [
		Transition {
			from: ""; to: "hasText"
			NumberAnimation { properties: "opacity" }
		},
		Transition {
			from: "hasText"; to: ""
			NumberAnimation { properties: "opacity" }
		}
	]

}
