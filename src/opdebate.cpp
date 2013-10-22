#include "opdebate.h"

OPDebate::OPDebate(QObject *parent) :
	Debate(parent), m_ffrPlaces(3), m_opPlaces(3), m_govPlaces(3)
{
	ROLES << JUDICATOR << OPD_FFR << OPD_GOV << OPD_OP;
	connect(this, SIGNAL(govPlacesChanged()), this, SIGNAL(numPlacesChanged()));
	connect(this, SIGNAL(opPlacesChanged()), this, SIGNAL(numPlacesChanged()));
	connect(this, SIGNAL(ffrPlacesChanged()), this, SIGNAL(numPlacesChanged()));
}

void OPDebate::setGovPlaces(int govPlaces)
{
	if (m_govPlaces != govPlaces) {
		m_govPlaces = govPlaces;
		emit govPlacesChanged();
	}
}

void OPDebate::setOpPlaces(int opPlaces)
{
	if (m_opPlaces != opPlaces) {
		m_opPlaces = opPlaces;
		emit opPlacesChanged();
	}
}

void OPDebate::setFfrPlaces(int ffrPlaces)
{
	if (m_ffrPlaces != ffrPlaces) {
		m_ffrPlaces = ffrPlaces;
		emit ffrPlacesChanged();
	}
}

const QList<Debate::RoleGroup> *OPDebate::getRolesForDebate()
{
	return &ROLES;
}

int OPDebate::numPlaces() {
	return Debate::numPlaces() + m_opPlaces + m_govPlaces + m_ffrPlaces;
}
