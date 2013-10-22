#include "bpsdebate.h"

BPSDebate::BPSDebate(QObject *parent) :
	Debate(parent), m_gov1Places(2), m_gov2Places(2), m_op1Places(2), m_op2Places(2)
{
	ROLES << JUDICATOR << BPS_GOV1 << BPS_GOV2 << BPS_OP1 << BPS_OP2;
	connect(this, SIGNAL(gov1PlacesChanged()), this, SIGNAL(numPlacesChanged()));
	connect(this, SIGNAL(gov2PlacesChanged()), this, SIGNAL(numPlacesChanged()));
	connect(this, SIGNAL(op1PlacesChanged()), this, SIGNAL(numPlacesChanged()));
	connect(this, SIGNAL(op2PlacesChanged()), this, SIGNAL(numPlacesChanged()));
}

const QList<Debate::RoleGroup> *BPSDebate::getRolesForDebate()
{
	return &ROLES;
}


int BPSDebate::numPlaces() {
	return Debate::numPlaces() + m_gov1Places + m_gov2Places + m_op1Places + m_op2Places;
}

void BPSDebate::setGov1Places(int gov1Places)
{
	if (gov1Places != m_gov1Places) {
		m_gov1Places = gov1Places;
		emit gov1PlacesChanged();
	}
}

void BPSDebate::setGov2Places(int gov2Places)
{
	if (gov2Places != m_gov2Places) {
		m_gov2Places = gov2Places;
		emit gov2PlacesChanged();
	}
}

void BPSDebate::setOp1Places(int op1Places)
{
	if (op1Places != m_op1Places) {
		m_op1Places = op1Places;
		emit op1PlacesChanged();
	}
}

void BPSDebate::setOp2Places(int op2Places)
{
	if (op2Places != m_op2Places) {
		m_op2Places = op2Places;
		emit op2PlacesChanged();
	}
}
