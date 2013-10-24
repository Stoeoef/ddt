#include "mainmodel.h"
#include "bpsdebate.h"
#include <QThread>
#include <QSet>
#include <QApplication>

MainModel::MainModel(QString configFile, QObject *parent) :
	QObject(parent), m_config(configFile), m_solutionState(UNKNOWN), m_configFile(configFile)
{
	SpeakerModel speakerProto;
	m_speakerList = new PrototypeQListQmlWrapper(&speakerProto, this);
	addSpeaker("Jonas");
	addSpeaker("Lisa");
	addSpeaker("Johannes");
	addSpeaker("Magda");
	addSpeaker("Hannes");
	addSpeaker("Stephan");
	addSpeaker("Christian");
	addSpeaker("Timo");
	addSpeaker("Jannis");

	OPDebate debateProto;
	m_debates = new PrototypeQListQmlWrapper(&debateProto, this);
	addOpDebate();

	Team teamProto;
	m_teams = new PrototypeQListQmlWrapper(&teamProto, this);
	addTeam("Team 1");
	addTeam("Team 2");
	m_workThread = NULL;
	m_solver = NULL;

	m_autoCompleteModel = NULL;
	readNames();

	QTranslator *translator = new QTranslator(this);
    translator->load(m_config.language(), "languages/");
    qApp->installTranslator(translator);

	connect(m_debates, SIGNAL(listChanged()), this, SIGNAL(numPlacesChanged()));
}

MainModel::~MainModel()
{
	qDeleteAll(m_mappers);
	m_mappers.clear();
}

void MainModel::addSpeaker() {
	addSpeaker(QString());
}

void MainModel::addSpeaker(QString name) {
	SpeakerModel* speaker = new SpeakerModel(this);
	speaker->setName(name);
	m_speakerList->appendRow(speaker);
}

void MainModel::removeSpeaker(int index) {
	m_speakerList->getRow(index)->deleteLater();
}

void MainModel::removeSpeaker(SpeakerModel *speaker)
{
	speaker->deleteLater();
}

void MainModel::addOpDebate()
{
	m_debates->appendRow(new OPDebate(m_debates));
}

void MainModel::addBpsDebate()
{
	m_debates->appendRow(new BPSDebate(m_debates));
}

void MainModel::removeDebate(int index)
{
	m_debates->getRow(index)->deleteLater();
}

int MainModel::numPlaces() {
	int sum = 0;
	for (int i = 0; i < m_debates->rowCount(); ++i) {
		Debate* debate = qobject_cast<Debate*>(m_debates->getRow(i));
		sum += debate->numPlaces();
	}
	return sum;
}

Team *MainModel::addTeam()
{
	return addTeam("Neues Team");
}

Team *MainModel::addTeam(QString name)
{
	Team *team = new Team(m_teams);
	team->setTeamName(name);
	m_teams->appendRow(team);
	return team;
}

void MainModel::removeTeam(Team *team) {
	m_teams->remove(team);
}

int MainModel::solutionCount()
{
	return m_mappers.size();
}

template <class T>
const QList<T> *castList(const QList<QObject*> *m_list)
{
	return reinterpret_cast<const QList<T> *> (m_list);
}

void MainModel::initSolver() {

	clearSolutions();

	const QList<Debate*> *debates = castList<Debate*>(m_debates->list());
	const QList<Team*> *teams = castList<Team*>(m_teams->list());
	const QList<SpeakerModel*> *speakers = castList<SpeakerModel*>(m_speakerList->list());
	m_solver = new MatchingSolver(debates, speakers, teams, NULL);
	m_workThread = new QThread(this);
	m_solver->moveToThread(m_workThread);
	connect(m_solver, SIGNAL(destroyed()), m_workThread, SLOT(deleteLater()));
	m_workThread->start();
	connect(m_solver, SIGNAL(finished(SeatToSpeakerMapper*)), this,
			SLOT(calculationFinished(SeatToSpeakerMapper*)));
	connect(m_solver, SIGNAL(interrupted()), this, SLOT(interrupted()));
	connect(m_solver, SIGNAL(noSolutionFound(QStringList)), this, SLOT(noSolutionsFound(QStringList)));

}



void MainModel::generateNewSolution()
{
	if (m_solutionState == NO_SOLUTION_EXISTS)
		return;	// don't calculate if there is no solution

	if (m_solutionState == CALCULATING)
		return; // dont start a second calculation
	m_previousState = solutionState();
	setSolutionState(CALCULATING);
	Q_ASSERT(m_solver);
	QMetaObject::invokeMethod(m_solver, "getRandomSolution");
}

void MainModel::clearSolutions()
{
	qDeleteAll(m_mappers);
	m_mappers.clear();
	emit solutionCountChanged();

	if (m_workThread) {
		m_workThread->quit();

	}
	if (m_solver) {
		m_solver->interrupt();
		m_solver->deleteLater();
	}
	setSolutionState(UNKNOWN);
}

SeatToSpeakerMapper *MainModel::getSolution(int index)
{
	if (0 <= index && index < m_mappers.size()) {
		return m_mappers[index];
	}
	return NULL;
}

void MainModel::interruptCalculation() {
	if (m_solutionState == CALCULATING) {
		Q_ASSERT(m_solver);
		m_solver->interrupt();
	}
}

void MainModel::updateNames()
{
	for (int i = 0; i < speakerList()->rowCount(); ++i) {
		SpeakerModel *speaker = qobject_cast<SpeakerModel*>(speakerList()->getRow(i));
		m_config.addName(speaker->name());
	}
	readNames();
}

void MainModel::writeConfiguration()
{
	m_config.writeConfigurationFile(m_configFile);
}

void MainModel::setLanguage(QString lang)
{
	m_config.setLanguage(lang);
}

void MainModel::readNames()
{
	QSet<QString> names = m_config.names();
	QStringList list;
	QSetIterator<QString> i(names);
	while (i.hasNext())
		list.append(i.next());
	delete m_autoCompleteModel;
	m_autoCompleteModel = new AutoCompleteModel(&list);
}


void MainModel::calculationFinished(SeatToSpeakerMapper *mapper)
{
	Q_ASSERT(mapper);
	setSolutionState(SOLUTION_EXISTS);
	m_mappers.push_back(mapper);
	emit solutionCountChanged();
}

void MainModel::noSolutionsFound(QStringList errors) {
	m_errors = errors;
	setSolutionState(NO_SOLUTION_EXISTS);
}

void MainModel::interrupted()
{
	setSolutionState(m_previousState);
}


void MainModel::setSolutionState(MainModel::SolutionState state)
{
	if (m_solutionState != state) {
		m_solutionState = state;
		emit solutionStateChanged();
	}
}
