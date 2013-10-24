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
import ddt.model 1.0
import QtQuick.Layouts 1.0

/**
  * \brief DDT's one and only application window.
  */
ApplicationWindow {
	id: container
	width: 1024
	height: 768
	color: "#aaaaaa"

	Component.onCompleted: focusDummy.focus = true
	/// The currently selected page. 0 refers to the speaker page, 1 refers to the debate page,
	/// 2 refers to the solution page.
	property int currentPage: 0;
	/// Contains all currently selected speakers.
	property QListQMLWrapper selectionModel: QListQMLWrapper{}

	/// Holds the applications main model.
	property MainModel mainModel: rootModel

	/// This action deletes all currently selected speakers.
	property Action deleteSelectedSpeakers: Action {
		text: qsTr("Delete")
		tooltip: "Delete selected speakers"
		shortcut: "Del"
		iconSource: "images/delete.svg"
		onTriggered: {
			for (var i = 0; i < selectionModel.rowCount; i++) {
				mainModel.removeSpeaker(selectionModel.getRow(i));
			}
		}
	}

	/// This action selects all available speakers.
	property Action selectAllAction: Action {
		text: qsTr("Select all")
		tooltip: "Select all speakers"
		shortcut: "Ctrl+A"
		onTriggered: {
			for (var i = 0; i < mainModel.speakerList.rowCount; i++) {
				var speaker = mainModel.speakerList.getRow(i);
				if (!selectionModel.contains(speaker))
					selectionModel.appendRow(speaker);
			}
		}
	}

	/// Adds a new speaker with an empty name.
	property Action addSpeakerAction: Action {
		text: qsTr("Add speaker")
		shortcut: "Ctrl+N"
		iconSource: "images/newspeaker.svg"
		onTriggered: {
			currentPage = 0;
			mainModel.addSpeaker();
		}
	}

	/// Adds a new BPS debate
	property Action addBpsDebateAction: Action {
		text: qsTr("Add BPS debate")
		shortcut: "Ctrl+B"
		iconSource: "images/bps.png"
		onTriggered: {
			currentPage = 1;
			mainModel.addBpsDebate();
		}
	}

	/// Adds a new OP debate.
	property Action addOpdAction: Action {
		text: qsTr("Add OPD")
		shortcut: "Ctrl+O"
		iconSource: "images/opd.png"
		onTriggered: {
			currentPage = 1;
			mainModel.addOpDebate();
		}
	}

	/// Generates a new solution. This will open the result page.
	property Action generateSolutionAction: Action {
		text: qsTr("Get solution")
		shortcut: "Ctrl+Return"
		iconSource: "images/newsolution.svg"
		enabled: mainModel.solutionState === MainModel.SOLUTION_EXISTS ||
				 mainModel.solutionState === MainModel.UNKNOWN
		onTriggered: {
			currentPage = 2;
			mainModel.generateNewSolution();
		}
	}

	/// \internal
	function restartSolver() {
		mainModel.initSolver();
		mainModel.updateNames();
	}

	/// Changes the application's language to a new value. Expects a country code as argument.
	function changeLanguage(lang) {
		changeLanguageNotification.visible = true;
		mainModel.setLanguage(lang);
		toolBar.enabled = false;
	}

	/// Interrupts the current calculation
	property Action interruptCalculationAction: Action {
		text: qsTr("Interrupt calculation")
		iconSource: "images/delete.svg"
		enabled: mainModel.solutionState === MainModel.CALCULATING
		onTriggered: {
			mainModel.interruptCalculation();
		}
	}

	onCurrentPageChanged: {
		if (mainModel.solutionState !== MainModel.UNKNOWN || currentPage == 2) {
			restartSolver();
		}
		if (currentPage != 2) {
			mainModel.interruptCalculation();
		}
	}

	toolBar: ToolBar {
		Row {
			spacing: 20


			ToolButton {
				id: speakersToolButton
				width: 42; height: width
				anchors.verticalCenter: parent.verticalCenter
				tooltip: "Configure speakers"
				iconSource: "images/speakers.svg"
				onClicked: currentPage = 0;
				checkable: true
				exclusiveGroup: mainPagesExclusiveGroup
			}

			ToolButton {
				id: debatesToolButton
				width: 42; height: width
				anchors.verticalCenter: parent.verticalCenter
				tooltip: "Configure debates"
				iconSource: "images/debate.png"
				onClicked: currentPage = 1
				checkable: true
				exclusiveGroup: mainPagesExclusiveGroup

			}
			ToolButton {
				id: resultsToolButton
				width: 42; height: width
				anchors.verticalCenter: parent.verticalCenter
				tooltip: "View solutions"
				iconSource: "images/results.svg"
				onClicked: {
					container.currentPage = 2;
					container.generateSolutionAction.trigger();
				}
				checkable: true
				exclusiveGroup: mainPagesExclusiveGroup
			}
			// placeholder
			Item {
				height: parent.height
				width: 50
			}

			Loader {
				sourceComponent: loader.item.toolBarComponent
			}

		}
		ExclusiveGroup {
			id: mainPagesExclusiveGroup
			current: {switch (currentPage) {
				case 0: return speakersToolButton;
				case 1: return debatesToolButton;
				case 2: return resultsToolButton;
				}
			}
		}
	}

	statusBar: StatusBar {

		RowLayout {
			Text {
				text: {
					var speakers = qsTr("%n speaker(s)","", mainModel.speakerList.rowCount);
					var debates = qsTr("%n debate(s)", "", mainModel.debates.rowCount);
					var places = qsTr("%n place(s)", "", mainModel.numPlaces);
					var color = mainModel.speakerList.rowCount !== mainModel.numPlaces ? "red" : "black";
					return "<font color='"+color+"'>" + qsTr("%1, %2 with %3").
					arg(speakers).arg(debates).arg(places) + "</font>";
				}
			}
		}
	}

	Item {
		// gathers all unhandled key events
		id: focusDummy

		Keys.forwardTo: loader.item
		// global shortcut to switch tabs
		Keys.onBacktabPressed: {
			if (event.modifiers & Qt.ControlModifier && currentPage > 0)
				currentPage--
		}
		Keys.onTabPressed: {
			if (event.modifiers & Qt.ControlModifier && currentPage < 2) {
				currentPage++;
				if (currentPage == 2)
					generateSolutionAction.trigger();
			}
		}
	}

	Component.onDestruction: {mainModel.writeConfiguration();}

	menuBar: MenuBar {
		Menu {
			title: qsTr("Edit...")
			MenuItem {action: container.addSpeakerAction}
			MenuItem {action: container.addOpdAction}
			MenuItem {action: container.addBpsDebateAction}
			MenuItem {action: container.selectAllAction}
			MenuItem {action: container.deleteSelectedSpeakers}
		}
		Menu {
			title: qsTr("Calculation")
			MenuItem {action: container.generateSolutionAction}
			MenuItem {action: container.interruptCalculationAction}
		}
		Menu {
			title: qsTr("Tools")
			Menu {
				title: qsTr("Set language")

				MenuItem {
					text: qsTr("English")
					onTriggered: container.changeLanguage("en")
				}
				MenuItem {
					text: qsTr("German");
					onTriggered: container.changeLanguage("de")
				}
			}
		}
	}

	MouseArea {
		// global mouse area that resets focus on click
		anchors.fill: parent
		onClicked: {
			focusDummy.focus = true;
			if (!(mouse.modifiers & Qt.ControlModifier || mouse.modifiers & Qt.ShiftModifier))
				container.selectionModel.clear();
		}
		preventStealing: true
	}

	Loader {
		id: loader
		Keys.forwardTo: focusDummy
		anchors.fill: parent
		smooth: true

		sourceComponent: {switch (currentPage) {
			case 0: return speakerPage;
			case 1: return debatePage;
			case 2: return resultPage;
			}
		}

		Component {
			id: speakerPage

			SpeakerPage {
				mainModel: container.mainModel
				selectionModel: container.selectionModel
				addSpeakerAction: container.addSpeakerAction
			}
		}

		Component {
			id: debatePage
			DebatePage {
				mainModel: container.mainModel
				addOpdAction: container.addOpdAction
				addBpsDebateAction: container.addBpsDebateAction
			}
		}

		Component {
			id: resultPage
			ResultPage {
				mainModel: container.mainModel
				generateSolutionAction: container.generateSolutionAction
				interruptCalculationAction: container.interruptCalculationAction
			}
		}

	}
	Rectangle {
		id: changeLanguageNotification
		anchors.fill: parent
		visible: false
		Text {
			id: message
			anchors.centerIn: parent
			text: qsTr("The change will take effect after\nrestarting the application.")
			font.pixelSize: 35
		}
		Button {
			anchors.top: message.bottom
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.topMargin: 20
			text: qsTr("Ok");
			onClicked: {
				container.toolBar.enabled = true;
				changeLanguageNotification.visible = false
			}
		}

	}
}
