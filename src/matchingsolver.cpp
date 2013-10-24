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

#include "matchingsolver.h"

#include "opdebate.h"
#include "bpsdebate.h"

using namespace operations_research;

//!\internal
class InterruptHandler : public SearchMonitor {
public:
	explicit InterruptHandler(MatchingSolver *ms, Solver * const s) : SearchMonitor(s), m_ms(ms) {}

	void BeginNextDecision(DecisionBuilder * const b) {
		Q_UNUSED(b);
		if (m_ms->isInterrupted()) {
			solver()->AddConstraint(solver()->MakeFalseConstraint("Interrupted"));
		}
	}

private:
	MatchingSolver *m_ms;
};

MatchingSolver::MatchingSolver(const QList<Debate*> *debates, const QList<SpeakerModel*> *speakers,
			   const QList<Team*> *teams, QObject* parent) : QObject(parent),
	m_speakers(*speakers), m_debates(*debates), m_teams(*teams), m_solver("ddt"),
	m_result(NULL), m_interrupted(false) {

}

bool MatchingSolver::isInterrupted()
{
	return m_interrupted;
}

void MatchingSolver::addLinksForGroup(Debate *d, int seatCount, Debate::RoleGroup group) {
	for (int i = 0; i < seatCount; ++i) {
		RoleLink *l = new RoleLink();
		l->hasSpeaker = false;
		l->debate = d;
		l->group = group;
		l->seatIndex = i;
		l->seatGroupIndex = m_currentSeatGroupIndex;
		m_links.push_back(l);

	}
	m_currentSeatGroupIndex++;
}

void MatchingSolver::detectSimpleErrors() {
	// check if speaker and role conuts are okay
	if (m_speakers.size() < m_links.size())
		m_errors.append(tr("%n place(s)", "", m_links.size()) +
						tr(", but only %n speaker(s)","", m_speakers.size()));
	else if (long (m_speakers.size()) > m_links.size())
		m_errors.append(tr("%n speaker(s)", "", m_speakers.size()) +
						tr("but only %n places(s)", "", m_links.size()));

	// check if every role can be matched to a speaker
	for (uint i = 0; i < m_links.size(); i++) {
		if (!m_links.at(i)->hasSpeaker) {
			RoleLink *l = m_links.at(i);
			m_errors.append(tr("Speaker missing for \"%1\"").arg(trRole(l->group)));
		}
	}
}

QString MatchingSolver::trRole(Debate::RoleGroup group) {
	switch (group) {
	case Debate::BPS_GOV1:
		return tr("1. Gov (BPS)");
	case Debate::BPS_GOV2:
		return tr("2. Gov (BPS)");
	case Debate::BPS_OP1:
		return tr("1. Op (BPS)");
	case Debate::BPS_OP2:
		return tr("2. Op (BPS)");
	case Debate::OPD_GOV:
		return tr("Gov (OPD)");
	case Debate::OPD_OP:
		return tr("Op (OPD)");
	case Debate::OPD_FFR:
		return tr("FFR");
	case Debate::JUDICATOR:
		return tr("Judicator");
	default:
		Q_ASSERT(false);
	}
}

void MatchingSolver::setLinks()
{
	m_currentSeatGroupIndex = 0;
	for (uint i = 0; i < m_links.size(); i++) {
		delete m_links[i];
	}
	m_links.clear();
	for (int i = 0; i < m_debates.size(); ++i) {
		Debate* d = m_debates[i];
		addLinksForGroup(d, d->judicatorPlaces(), Debate::JUDICATOR);

		switch(d->format()) {
		case Debate::OPD: {
			OPDebate* opdDebate = dynamic_cast<OPDebate*>(d);
			addLinksForGroup(opdDebate, opdDebate->ffrPlaces(), Debate::OPD_FFR);
			addLinksForGroup(opdDebate, opdDebate->govPlaces(), Debate::OPD_GOV);
			addLinksForGroup(opdDebate, opdDebate->opPlaces(), Debate::OPD_OP);
		}
			break;
		case Debate::BPS: {
			BPSDebate* bpsDebate = dynamic_cast<BPSDebate*>(d);
			addLinksForGroup(bpsDebate, bpsDebate->gov1Places(), Debate::BPS_GOV1);
			addLinksForGroup(bpsDebate, bpsDebate->gov2Places(), Debate::BPS_GOV2);
			addLinksForGroup(bpsDebate, bpsDebate->op1Places(), Debate::BPS_OP1);
			addLinksForGroup(bpsDebate, bpsDebate->op2Places(), Debate::BPS_OP2);
		}
		}
	}
}

void MatchingSolver::interrupt()
{
	m_interrupted = true;
}

template<class T>
void shuffle(QList<T> &list) {
	int size = list.size();
	for (int i = 1; i < size; i++) {
		list.swap(i, qrand() % size);
	}
}

bool MatchingSolver::isForbidden(SpeakerModel* speakerModel, RoleLink &link)
{
	return
				(link.group == Debate::OPD_FFR && !speakerModel->opdFfr())
			|| (link.group == Debate::OPD_GOV && !speakerModel->opdGov())
			|| (link.group == Debate::OPD_OP && !speakerModel->opdOp())
			|| ((link.group == Debate::BPS_GOV1 || link.group == Debate::BPS_GOV2)
				&& !speakerModel->bpsGov())
			|| ((link.group == Debate::BPS_OP1 || link.group == Debate::BPS_OP2)
				&& !speakerModel->bpsOp())
			|| (link.group == Debate::JUDICATOR && link.debate->format() == Debate::OPD
				&& !speakerModel->opdJud())
			|| (link.group == Debate::JUDICATOR && link.debate->format() == Debate::BPS
				&& !speakerModel->bpsJud());
}

void MatchingSolver::createSpeakerConstraints() {
	// All speakers must be matched to a different role
	m_solver.AddConstraint(m_solver.MakeAllDifferent(m_speakerVars));

	for (int i = 0; i < m_speakers.size(); ++i) {
		SpeakerModel* speakerModel = m_speakers[i];
		uint forbiddenCount = 0;
		for (uint j = 0; j < m_links.size(); ++j) {
			RoleLink *link = m_links[j];
			// Forbid roles that the speaker doesn't want to have
			if (isForbidden(speakerModel, *link)) {
				forbiddenCount++;
				m_solver.AddConstraint(m_solver.MakeNonEquality(m_speakerVars[i], int(j)));
			} else
				m_links[j]->hasSpeaker = true;
		}
		if (forbiddenCount == m_links.size())
			// no role allowed
			m_errors.append(tr("%1 cannot be mapped to any position").arg(speakerModel->name()));
	}
}

void MatchingSolver::createTeamConstraints()
{
	// Ensure that teams are matched together.
	// The team members seat group indices are the same.
	m_teamVars.clear();
	for (int i = 0; i < m_teams.size(); ++i) {
		int beginnersInTeam = 0;
		if (m_teams[i]->getSpeakers()->size() > 0) {
			IntVar *teamSeatGroupValue = m_solver.MakeIntVar(0, m_currentSeatGroupIndex - 1, "team");
			Team* team = m_teams[i];
			const QList<SpeakerModel*> *speakers = team->getSpeakers();
			for (int j = 0; j < speakers->size(); ++j) {
				if (speakers->at(j)->isBeginner())
					beginnersInTeam++;
				int speakerId = m_speakers.indexOf(speakers->at(j));
				Constraint *c = m_solver.MakeElementEquality(m_speakerToGroupMapping,
										m_speakerVars[speakerId], teamSeatGroupValue);
				m_solver.AddConstraint(c);
			}
			m_teamVars.push_back(teamSeatGroupValue);
		}
		m_beginnersInTeam.push_back(beginnersInTeam);
	}
}

void MatchingSolver::createBeginnerConstraints()
{
	// check for emptyness, otherwise MakeAllDifferent(roleToGroupMapping, ...) will fail
	if (!m_speakerToGroupMapping.empty()) {
		std::vector<IntVar*> beginnerGroups;
		for (int j = 0; j < m_speakers.size(); ++j) {
			SpeakerModel *speaker = m_speakers[j];
			if (speaker->isBeginner()) {
				beginnerGroups.push_back(m_solver.MakeElement(m_speakerToGroupMapping,
															   m_speakerVars[j])->VarWithName("beginner"));
			}
		}
		if (!beginnerGroups.empty()) {
			m_solver.AddConstraint(m_solver.MakeAllDifferent(beginnerGroups));

			// ensure that there is at most one beginner in each team
			int max = 0;
			int maxIndex = -1;
			for (uint i = 0; i < m_beginnersInTeam.size(); ++i) {
				if (m_beginnersInTeam[i] > max) {
					max = m_beginnersInTeam[i];
					maxIndex = i;
				}
			}
			if (max > 1) {
				m_errors.append(tr("More than 2 beginners in team \"%1\"")
								.arg(m_teams[maxIndex]->teamName()));
				m_solver.AddConstraint(m_solver.MakeFalseConstraint());
			}
			if (beginnerGroups.size() > uint(m_currentSeatGroupIndex)) {
				m_errors.append(tr("More beginners than tables"));
				m_solver.AddConstraint(m_solver.MakeFalseConstraint());
			}
		}
	}


}

void MatchingSolver::setupVariables() {
	// roleToGroupMapping[i] stores the seat group of speaker i
	m_speakerToGroupMapping.clear();
	for (uint i = 0; i < m_links.size(); ++i) {
		m_speakerToGroupMapping.push_back(m_links[i]->seatGroupIndex);
	}
}


 void MatchingSolver::readVars(SolutionCollector *collector) {

	m_result = NULL;
	if (collector->solution_count() == 0) {
		return;
	}
	m_result = new SeatToSpeakerMapper(this);
	Assignment *assignment = collector->solution(0);
	for (int i = 0; i < m_speakers.size(); i++) {
		SpeakerModel* speakerModel = m_speakers[i];
		int j = assignment->Value(m_speakerVars[i]);
		RoleLink *link = m_links[j];
		m_result->addMapping(link->debate, link->group, link->seatIndex, speakerModel);
	}
}

template<class T>
void appendAll(std::vector<T> &a, const std::vector<T> &b) {
	for (uint i = 0; i < b.size(); ++i) {
		a.push_back(b[i]);
	}
}

DecisionBuilder *MatchingSolver::createDecisionBuilder() {
	std::vector<IntVar*> resultVars;
	appendAll(resultVars, m_teamVars);
	// set beginners first as they are more difficult to set
	for (int i = 0; i < m_speakers.size(); i++) {
		if (m_speakers[i]->isBeginner())
			resultVars.push_back(m_speakerVars[i]);
	}
	for (int i = 0; i < m_speakers.size(); i++) {
		if (!m_speakers[i]->isBeginner())
			resultVars.push_back(m_speakerVars[i]);
	}
	DecisionBuilder *db = m_solver.MakePhase(resultVars,
										  Solver::CHOOSE_FIRST_UNBOUND, Solver::ASSIGN_RANDOM_VALUE);

	return db;
}

void MatchingSolver::getRandomSolution()
{
	m_interrupted = false;

	shuffle(m_speakers);
	shuffle(m_debates);
	shuffle(m_teams);

	// Set links to the roles of all debates
	setLinks();

	// create speaker variables
	m_speakerVars.clear();
	m_solver.MakeIntVarArray(m_speakers.size(), 0, m_links.size() - 1, "speaker_", &m_speakerVars);

	m_errors.clear();
	createSpeakerConstraints();
	setupVariables();
	createTeamConstraints();
	createBeginnerConstraints();
	detectSimpleErrors();
	DecisionBuilder *db = createDecisionBuilder();

	SolutionCollector *collector = m_solver.MakeFirstSolutionCollector();
	collector->Add(m_speakerVars);
	InterruptHandler *interrupt = new InterruptHandler(this, &m_solver);
	m_solver.Solve(db, collector, interrupt);
	if (isInterrupted())
		emit interrupted();
	else {
		readVars(collector);
		if (!m_result || !m_errors.isEmpty())
			emit noSolutionFound(m_errors);
		else
			emit finished(m_result);
	}

	emit done();
}

/*
 *if (!beginnerGroups.empty()) {
			std::vector<int> counter;
			for (int i = 0; i < m_currentSeatGroupIndex; i++) {
				counter.push_back(i);
			}
			m_solver.MakeIntVarArray(m_currentSeatGroupIndex, 0, beginnerGroups.size(), &m_counts);
			m_solver.AddConstraint(m_solver.MakeDistribute(beginnerGroups, counter, m_counts));
			m_bestValue = beginnerGroups.size();
			if (m_currentSeatGroupIndex < beginnerGroups.size())
				m_bestValue += 2 * (beginnerGroups.size() - m_currentSeatGroupIndex);
			qDebug("begGroups: %d", beginnerGroups.size());
			qDebug("seatInd: %d", m_currentSeatGroupIndex);
			qDebug("bestVal: %d", m_bestValue);
			//std::vector<IntVar*> squares;
			m_squares.clear();
			for (int i = 0; i < m_counts.size(); i++) {
				m_squares.push_back(m_solver.MakeSquare(m_counts[i])->VarWithName("square"));
			}
			m_goal = m_solver.MakeSum(m_squares)->VarWithName("goal");
			//m_goal->SetMin(beginnerGroups.size() + 2 * (m_currentSeatGroupIndex - beginnerGroups.size());
			return m_solver.MakeMinimize(m_goal, 1);
		}

*/



