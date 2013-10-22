#ifndef MATCHINGSOLVER_H
#define MATCHINGSOLVER_H

#include <constraint_solver/constraint_solver.h>
#include "debate.h"
#include "seattospeakermapper.h"
#include <QObject>
#include <QHash>
#include <QStringList>

using namespace operations_research;

///\internal
struct RoleLink {
	Debate* debate;
	Debate::RoleGroup group;
	int seatIndex;
	int seatGroupIndex;
	bool hasSpeaker;
};

/*!
 * \brief Solves the main DDT matching problem.
 *
 * DDT matches speakers to speaker roles in a debate. Only feasible matchings will
 * be calculated, otherwise the solver signals that no solution exists. A matching is
 * only feasible if
 *	- No speaker is mapped to a role that is not allowed for him (e.g. by setting
 *	SpeakerModel::opdFfr to false
 *	- Two beginner must not be placed in the same team
 *	- Teams must be placed in the same team
 * The computational complexity of the problem hight due to the last two
 * feasability criteria. This class uses google's constraint programming solver
 * \i or-tools to calculate optimal solutions.\n
 * The solver is not designed to calculate all solutions (the number of solutions grows
 * exponentionally with the number of speakers) but guarantees highly randomised
 * single solutions by shuffling the input data. New random solutions can be obtained
 * without recreating the solver. \n
 * The class is designed to operate in its own thread, communication is done by
 * Qt's signal/slot mechanism.
 */
class MatchingSolver : public QObject {
	Q_OBJECT
public:
	/*!
	 * \brief MatchingSolver creates a new matching solver with given problem data.
	 * \param debates The debates considered for this matching.
	 * \param speakers The speakers considered for this matching.
	 * \param teams The teams considered for this matching.
	 * \param parent The solver's parent object
	 */
	MatchingSolver(const QList<Debate*> *debates, const QList<SpeakerModel*> *speakers,
				   const QList<Team*> *teams, QObject* parent);

	//! Returns \c true if the solver was interrupted.
	bool isInterrupted();
public slots:

	//! Starts the calculation of a new random solution. Once the calculation is finished,
	//! \ref noSolutionFound or \ref finished is signaled.
	void getRandomSolution();

	//! Asks the solver to interrupt the current solving process. The call has no effect if
	//! no calculation is running. After a succesfull interrupt, \ref interrupted is signaled.
	void interrupt();
signals:
	/*!
	 * \brief Emitted after a solution was found.
	 * \param mapper a feasible, freshly calculated solution.
	 */
	void finished(SeatToSpeakerMapper *mapper);
	//! Emitted after a successfull interrupt.
	void interrupted();
	/*!
	 * \brief Emitted if no feasible solution exists.
	 * \param errors a (potentially empty) list of reasons why
	 * no solution could be found.
	 */
	void noSolutionFound(QStringList errors);

	//! Called after either \ref finished, \ref interrupted or \ref noSolutionFound
	//! have been called. The solver becomes inactive afterwards.
	void done();
private:

	QList<SpeakerModel*> m_speakers;
	QList<Debate*> m_debates;
	QList<Team*> m_teams;
	QStringList m_errors;
	Solver m_solver;
	std::vector<RoleLink*> m_links;
	std::vector<IntVar*> m_speakerVars;
	std::vector<int> m_speakerToGroupMapping;
	int m_currentSeatGroupIndex;
	std::vector<int> m_beginnersInTeam;
	std::vector<IntVar*> m_teamVars;
	SeatToSpeakerMapper *m_result;
	volatile bool m_interrupted;

	void setLinks();
	bool isForbidden(SpeakerModel* speakerModel, RoleLink &link);
	void addLinksForGroup(Debate *d, int seatCount, Debate::RoleGroup group);
	QString trRole(Debate::RoleGroup group);
	void detectSimpleErrors();
	void createSpeakerConstraints();
	void setupVariables();
	void createTeamConstraints();
	void createBeginnerConstraints();
	DecisionBuilder *createDecisionBuilder();
	void readVars(SolutionCollector *collector);
};

#endif // MATCHINGSOLVER_H
