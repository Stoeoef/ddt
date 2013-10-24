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
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.0
import ddt.model 1.0

/**
  * \c A list with all speaker names.
  *
  * Displays a list with all speaker names given by a \c MainModel . The component allows
  * multiple selection of speakers.
  */
ListView {
	id: container
	keyNavigationWraps: true
	anchors.margins: 5
	highlightFollowsCurrentItem: false
	/// The applications \c MainModel
	property MainModel mainModel;
	/// A list of \c SpeakerModel objects representing the currently selected speakers.
	property QListQMLWrapper selectionModel;

	model: mainModel.speakerList;

	function clearSelection() {selectionModel.clear();}

	function addBetweenToSelection(a, b) {
		var minIndex = a <= b ? a : b;
		var maxIndex = a >= b ? a : b;
		for (var i = minIndex; i <= maxIndex; i++)
			addToSelection(i);
	}

	function toggleSelection(index) {
		if (selectionContains(index))
			removeFromSelection(index);
		else
			addToSelection(index);
	}

	function addToSelection(index) {
		if (!selectionContains(index))
			selectionModel.appendRow(container.mainModel.speakerList.getRow(index));
	}

	function selectionContains(index) {
		return selectionModel.contains(container.mainModel.speakerList.getRow(index));
	}

	function removeFromSelection(index) {
		selectionModel.remove(container.mainModel.speakerList.getRow(index));
	}

	function selectCurrentElement(event, forceFocus) {
		selectionModel.clear();
		if (!(event.modifiers & Qt.ShiftModifier)) {
			container.selectionPivot = container.currentIndex;
		}
		if (container.selectionPivot >= 0) {
			container.addBetweenToSelection(container.selectionPivot, container.currentIndex);
		}
		if (forceFocus) {
			container.currentItem._selectTextField();
		}
	}

	function selectionEvent(mouse, index, delegate) {
		// implements a multi-selection list
		var shift = mouse.modifiers & Qt.ShiftModifier;
		var ctrl = mouse.modifiers & Qt.ControlModifier;

		if (!shift && !ctrl) {
			// start new selection
			if (delegate._selected) {
				delegate._selectTextField();
			} else {
				clearSelection();
				addToSelection(index);
				selectionPivot = index;
			}
		}
		if (shift) {
			if (selectionPivot >= 0) {
				if (!ctrl)
					clearSelection();
				addBetweenToSelection(selectionPivot, index);
			} else {
				addToSelection(index);
				selectionPivot = index;
			}
		} else 	if (ctrl) {
			// no shift
			toggleSelection(index);
			selectionPivot = index;
		}
		currentIndex = index;
	}

	// needed for shift-selection, all elements between selectionPivot and the
	// currently selected element are added to the selection
	property int selectionPivot: -1;
	Connections {
		target: mainModel.speakerList
		// reset if pivot becomes invalid due to row removal
		onRowsRemoved: {if (container.selectionPivot >= selectionModel.rowCount) container.selectionPivot = -1}
	}

	Keys.onDownPressed: {
		container.incrementCurrentIndex();
		selectCurrentElement(event);
	}

	Keys.onUpPressed: {
		container.decrementCurrentIndex();
		selectCurrentElement(event);
	}

	Keys.onReturnPressed: {
		if (container.currentIndex >= 0)
			container.currentItem._selectTextField();
	}


	removeDisplaced: Transition {
		NumberAnimation { property: "y"; duration: 300 }
	}

	add: Transition {
		NumberAnimation { property: "opacity"; from: 0; to:1.0; duration: 500 }
	}


	remove: Transition {
		NumberAnimation { property: "opacity"; from: 1.0; to:0.0; duration: 2000 }
	}

	// Contains an autocomplete text field and a removal button
	delegate: FocusScope {
		id: delegate
		height: textField.height + 5
		// set margin manually, anchors.XXXmargin doesn't work with resizing
		x: (container.width - width) / 2
		width: container.width - 14
		focus: true
		z: container.currentIndex == index ? 1 : 0

		// true if this element is contained in the selection
		property bool _selected;
		property bool _hovered: false;
		Connections {
			target: selectionModel
			onListChanged: _selected = selectionModel.contains(
							   container.mainModel.speakerList.getRow(index));
		}

		onActiveFocusChanged: {
			if (activeFocus && container.currentIndex != index) {
				container.clearSelection();
				container.addToSelection(index);
				container.selectionPivot = container.currentIndex = index;
			}
		}

		function _selectTextField() {textField.forceActiveFocus();}

		Rectangle {
			id: selectionHighlight
			anchors.fill: parent
			focus: false;
			color: {
				if (delegate._selected)
					return "lightblue";
				if (delegate._hovered)
					return "#DAECF5";
				if (index % 2 == 1)
					return "#F2F2F2";
				return "white";
			}
			z: -1
		}

		// Captures clicks and adds the element to the selection
		MouseArea {
			anchors.fill: parent
			onClicked: container.selectionEvent(mouse, index, delegate);
			hoverEnabled: true
			onEntered: _hovered = true
			onExited: _hovered = false
		}
		RowLayout {
			anchors.fill: parent
			anchors.margins: 5

			AutoCompleteTextField {
				id: textField
				text: name
				clip: true;
				onReturnPressed: {
					if (container.currentIndex + 1 !== container.model.rowCount) {
						container.incrementCurrentIndex();
						container.selectCurrentElement(event, true);
					} else {
						// we're at the end of the list, just loose focus
						container.currentItem.focus = false
					}
				}
				//: shown if no name is entered
				alternativeText: name != "" ? name : qsTr("Enter name")
				preferAlternativeText: true
				completeModel: container.mainModel.autoCompleteModel
				onTextChanged: name = text
				focus: false
				activeFocusOnPress: false;
				Layout.fillWidth: true
				// The text field hides all mouse events, thus we need a second area to
				// register clicks in textField
				MouseArea {
					anchors.fill: parent
					hoverEnabled: true
					onEntered: _hovered = true
					onExited: _hovered = false
					onClicked: container.selectionEvent(mouse, index, delegate);
				}
			}

			Button {
				Layout.fillWidth: false
				Layout.fillHeight: true
				Layout.maximumWidth: 30
				style: ButtonStyle {
					label: Image {
						source: control.iconSource
					}
				}

				focus: false
				onActiveFocusChanged: {
					if (activeFocus)
						delegate.focus = true;}
				iconSource: "images/delete.svg"
				onClicked: {
					mainModel.removeSpeaker(index);
				}
			}
		}
	}

}
