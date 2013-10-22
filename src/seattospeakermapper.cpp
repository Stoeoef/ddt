#include "seattospeakermapper.h"

SeatToSpeakerMapper::SeatToSpeakerMapper(QObject *parent) :
	QObject(parent)
{

}

void SeatToSpeakerMapper::addMapping(Debate *debate, Debate::RoleGroup group, int seatIndex, SpeakerModel *speaker)
{
	QHash<Debate::RoleGroup, TableNames*> * map1 =
			m_mapping[debate];
	if (!map1) {
		m_mapping.insert(debate,map1 = new QHash<Debate::RoleGroup, TableNames*>);
	}
	TableNames* tableNames = map1->value(group);
	if (!tableNames) {
		map1->insert(group, tableNames = new TableNames(this));
	}
	tableNames->addMapping(seatIndex, speaker);
}

TableNames* SeatToSpeakerMapper::getNamesForTable(Debate* debate, Debate::RoleGroup group)
{
	QHash<Debate::RoleGroup, TableNames*>* map1 = m_mapping.value(debate);
	if (map1) {
		return map1->value(group);
	}
	return NULL;
}

TableNames *SeatToSpeakerMapper::getNamesForTable(Debate *debate, int group)
{
	return getNamesForTable(debate, Debate::RoleGroup(group));
}
