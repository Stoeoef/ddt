#include "debate.h"

Debate::Debate(QObject *parent) :
	QObject(parent), m_room("R109"), m_judicatorPlaces(1)
{
	connect(this, SIGNAL(judicatorPlacesChanged()), this, SIGNAL(numPlacesChanged()));
}

void Debate::setRoom(QString room)
{
	if (m_room != room) {
		m_room = room;
		emit roomChanged();
	}
}

void Debate::setJudicatorPlaces(int judicatorPlaces)
{
	if (m_judicatorPlaces != judicatorPlaces) {
		m_judicatorPlaces = judicatorPlaces;
		emit judicatorPlacesChanged();
	}
}

int Debate::numPlaces() {
	return m_judicatorPlaces;
}
