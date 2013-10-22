#include "team.h"

Team::Team(QObject *parent) :
	QObject(parent)
{

}

void Team::setTeamName(QString teamName)
{
	if (teamName != m_teamName) {
		m_teamName = teamName;
		emit teamNameChanged();
	}
}


void Team::addSpeaker(SpeakerModel *speaker)
{
	Q_ASSERT(speaker);
	if (!m_speakers.contains(speaker)) {
		m_speakers << speaker;
		connect(speaker, SIGNAL(teamChanged(SpeakerModel*)), this, SLOT(updateSpeaker(SpeakerModel*)));
		connect(speaker, SIGNAL(destroyed(QObject*)), this, SLOT(removeSpeaker(QObject*)));
		emit speakersChanged();
	}
}


void Team::updateSpeaker(SpeakerModel* speaker)
{
	Q_ASSERT(speaker);
	if (speaker->team() != this) {
		m_speakers.removeAll(speaker);
		speaker->disconnect(this);
		emit speakersChanged();
	}
}

void Team::removeSpeaker(int index)
{
	removeSpeaker(m_speakers[index]);
}

void Team::removeSpeaker(QObject *obj)
{
	SpeakerModel *speaker = static_cast<SpeakerModel*>(obj);
	Q_ASSERT(speaker);
	if (speaker->team() == this)
		// this will call updateSpeaker and thus remove the speaker from this team
		speaker->resetTeam();
}

