// import QtQuick 2.00 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import QtQuick.Controls 1.0
import ddt.model 1.0
/**
  * A text field with editing and display state, depending on focus.
  *
  * This component holds a text field which displays its content differently
  * depending on having focus or not. In editing mode, text is shown as a usual
  * \c TextField, in display mode, an alternative text is shown as an uneditable
  * \c Text component. A single mouse click will attract focus and switch to
  * editing state.
  */
FocusScope {
	id: container

	/// Holds if the text is editable. This component will act as a normal \c Text
	/// if set to \c false.
	property bool editable: true
	/// The text field's editable text.
	property alias text: textField.text
	/// The text field's alternative text, shown when not focussed.
	property alias alternativeText: enterText.text
	/// The component will be shown as a normal text field, the alternative text is
	/// only shown when \c text is empty.
	property bool preferAlternativeText: true
	/// The text field's horizontal aligmnent.
	property alias horizontalAlignment: enterText.horizontalAlignment

	// The text field's font.
	property alias font: textField.font
	height: 40

	Keys.forwardTo: textField
	TextField {
		id: textField
		focus: true
		font.pixelSize: 19
		anchors.fill: parent
		opacity: preferAlternativeText ? 0 : 1
		onActiveFocusChanged: {container.focus = activeFocus; if (activeFocus) selectAll();}
		horizontalAlignment: enterText.horizontalAlignment
		verticalAlignment: enterText.verticalAlignment
		//visible: editable
	}

	Text {
		id: enterText
		focus: false
		anchors.fill: textField
		anchors.leftMargin: horizontalAlignment == Text.AlignLeft ? 8 : 0
		verticalAlignment: Text.AlignVCenter
		color: "gray"
		clip: true
		font.italic: true
		font.pixelSize: textField.font.pixelSize
	}


	states: State {
		name: "hasText"; when:
			editable && ((!preferAlternativeText && textField.text != '') || textField.activeFocus)
		PropertyChanges { target: enterText; opacity: 0 }
		PropertyChanges { target: textField; opacity: 1 }

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

