#include "tablenames.h"

TableNames::TableNames(QObject *parent) :
	QObject(parent)
{
}

SpeakerModel* TableNames::getSpeaker(int index)
{
	return m_names[index];

}

void TableNames::speakerDestroyed(QObject *speaker) {
	Q_ASSERT(speaker);
	m_names.remove(m_names.key(static_cast<SpeakerModel*>(speaker)));
}

void TableNames::addMapping(int index, SpeakerModel *speaker)
{
	Q_ASSERT(speaker);
	m_names.insert(index, speaker);
	connect(speaker, SIGNAL(destroyed(QObject*)), this, SLOT(speakerDestroyed(QObject*)));
}
