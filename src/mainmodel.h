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

#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QObject>
#include <QQmlListProperty>
#include "speakermodel.h"
#include <QAbstractListModel>
#include "qlistqmlwrapper.h"
#include "opdebate.h"
#include "debate.h"
#include "seattospeakermapper.h"
#include "matchingsolver.h"
#include "prototypeqlistqmlwrapper.h"
#include <QCompleter>
#include <QStringList>
#include "autocompletemodel.h"
#include "ddtconfiguration.h"
#include <QTranslator>
#include <QStringList>

/*!
 * \brief Offers access to all parts of the application's model.
 *
 * The \c MainModel models the application's state. It stores the
 * current speakers, debates and solution. Furthermore, it allows
 * various operations on the model elements.
 */
class MainModel : public QObject
{
	Q_OBJECT
	//! List of the application's speakers
	Q_PROPERTY(QListQmlWrapper* speakerList READ speakerList CONSTANT)
	//! List of the application's debates
	Q_PROPERTY(QListQmlWrapper* debates READ debates CONSTANT)
	//! List of the application's teams
	Q_PROPERTY(QListQmlWrapper* teams READ teams CONSTANT)
	//! A completion model for name autocompletion
	Q_PROPERTY(AutoCompleteModel* autoCompleteModel READ autoCompleteModel CONSTANT)
	//! The application's solution state
	Q_PROPERTY(SolutionState solutionState READ solutionState NOTIFY solutionStateChanged)
	//! Holds the number of calculated and stored solutions
	Q_PROPERTY(int solutionCount READ solutionCount NOTIFY solutionCountChanged)
	//! Holds the complete number of seats in all debate's
	Q_PROPERTY(int numPlaces READ numPlaces NOTIFY numPlacesChanged)
	Q_ENUMS(SolutionState)

public:
	//! Specifies the application's solution state
	enum SolutionState {
		UNKNOWN, /*!< It is unknown whether one, none or more solutions exist */
		SOLUTION_EXISTS, /*!< At least one solution exists */
		NO_SOLUTION_EXISTS,  /*!< No solution exists */
		CALCULATING /*!< A calculation is in progress, solutionState is updated when
					  the calculation is done */
	};

	/*!
	 * \brief Constructs a new \c MainModel.
	 *
	 * The main model is initialized with options from a given configuration file.
	 * \param configFile a (relative) path to a configuration file
	 * \param parent the object's parent
	 * \sa DDTConfiguration
	 */
	explicit MainModel(QString configFile = "options.xml", QObject *parent = 0);

	//! Deletes the main model and releases all ressources.
	~MainModel();

	QListQmlWrapper* debates() {return m_debates;}

	QListQmlWrapper* speakerList() {return m_speakerList;}

	QListQmlWrapper* teams() {return m_teams;}

	AutoCompleteModel* autoCompleteModel() {return m_autoCompleteModel;}

	//! Same as \code addSpeaker("") \endcode
	Q_INVOKABLE void addSpeaker();
	//! Creates a new speaker with a given name and adds him to the speaker list.
	Q_INVOKABLE void addSpeaker(QString name);

	//! Removes a speaker at a given position in the speaker list.
	Q_INVOKABLE void removeSpeaker(int index);
	//! Removes a given speaker.
	Q_INVOKABLE void removeSpeaker(SpeakerModel *speaker);

	//! Adds a new OP-Debate to the application's debate list.
	Q_INVOKABLE void addOpDebate();
	//! Adds a new BPS-Debate to the application's debate list.
	Q_INVOKABLE void addBpsDebate();
	//! Removes a debate from the debate list with a given index.;
	Q_INVOKABLE void removeDebate(int index);

	int numPlaces();

	/*!
	 * \brief Adds a new, empty team to the application's team list.
	 * \return the added team
	 */
	Q_INVOKABLE Team* addTeam();

	/*!
	 * \brief addTeam Adds a new, empty team with a given name to the team list.
	 * \param name the team's name
	 * \return the added team
	 */
	Team *addTeam(QString name);
	//! Removes a given team from the applications team list
	Q_INVOKABLE void removeTeam(Team *team);

	int solutionCount();
	SolutionState solutionState() {return m_solutionState;}
	//! \brief Returns a list of error descriptions.
	//!
	//! The errors need to be fixed before a solution can be generated.
	//! They can be presented to the users and give clues why no solution exists
	//! (see \ref solutionState). The return value is undefined if the solution state
	//! is different than \ref NO_SOLUTION_EXISTS .
	Q_INVOKABLE QStringList errors() {return m_errors;}

	/*!
	 * \brief Prepares a calculation.
	 * The methods takes the current speaker, debate and team list and creates an passes them
	 * to an internal solver to prepare further solution calculations. After calling this method
	 * solutions can be generated with generateNewSolution();
	 */
	Q_INVOKABLE void initSolver();

	/*!
	 * \brief Resets the applications solution state
	 * Deletes all calculated solutions and frees the internal solver setup. The solutionState
	 * is set to UNKNOWN afterwards. \n
	 * Remember to call initSolver(); before attempting to generate new solutions.
	 */
	Q_INVOKABLE void clearSolutions();

	//! Returns a stored solution with a given index.
	Q_INVOKABLE SeatToSpeakerMapper *getSolution(int index);

	/*!
	 * \brief Generates a new solution and stores it internally.
	 *
	 * Call initSolver(); before attempting to call this method. The
	 * solution state will be set to CALCULATING, once the solution is finished the
	 * state changes to NO_SOLUTION_EXISTS or SOLUTION_EXISTS. The generated solution
	 * is stored internally and can be read with getSolution.
	 * \sa solutionCount
	 */
	Q_INVOKABLE void generateNewSolution();

	//! Interrupts the current calculation. Calling this method when no calculation takes
	//! place has no effect.
	Q_INVOKABLE void interruptCalculation();

	/*!
	 * Reads all names of the application's speakers and adds them to the name pool of
	 * autoCompleteModel. Furthermore, all names will be stored in a configuration file once
	 * writeConfiguration is called.
	 */
	Q_INVOKABLE void updateNames();

	/*!
	 * Writes all options into a configuration file.
	 */
	Q_INVOKABLE void writeConfiguration();

	/*!
	 * Sets the applications language. The change will only take effect if the configuration
	 * file is written (writeConfiguration) and the application restarted.
	 */
	Q_INVOKABLE void setLanguage(QString lang);

signals:
	void solutionCountChanged();
	void solutionStateChanged();
	void numPlacesChanged();
private slots:
	void calculationFinished(SeatToSpeakerMapper* mapper);
	void interrupted();
	void noSolutionsFound(QStringList errors);
private:
	void setSolutionState(SolutionState state);
	void readNames();
	QListQmlWrapper* m_speakerList;
	QListQmlWrapper* m_debates;
	QListQmlWrapper* m_teams;
	MatchingSolver *m_solver;
	AutoCompleteModel *m_autoCompleteModel;

	QStringList m_errors;
	QList<SeatToSpeakerMapper*> m_mappers;
	QThread *m_workThread;
	SolutionState m_solutionState;
	QCompleter *m_completer;
	DDTConfiguration m_config;
	QString m_configFile;
	SolutionState m_previousState;
};


#endif // MAINMODEL_H
