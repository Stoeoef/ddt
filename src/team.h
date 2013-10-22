#ifndef TEAM_H
#define TEAM_H

#include <QObject>
#include "qlistqmlwrapper.h"

#include "speakermodel.h"

class SpeakerModel;

/*!
 * \brief Represents a team in a debate.
 *
 * A team consists of several speakers which will be matched to the same role.
 * Deleting a speaker will automatically remove him from the team.
 */
class Team : public QObject
{
	Q_OBJECT
	//! \property Holds the team's name.
	Q_PROPERTY(QString teamName READ teamName WRITE setTeamName NOTIFY teamNameChanged)

public:
	//! Creates a new team with no speakers.
	explicit Team(QObject *parent = 0);

	QString teamName(){return m_teamName;}
	void setTeamName(QString teamName);

	//! Adds a speaker to this team.
	void addSpeaker(SpeakerModel* speaker);
	//! Returns \c true if a speaker is contained in a specific team.
	bool containsSpeaker(SpeakerModel* speaker);

	//! Returns all speakers of this team.
	const QList<SpeakerModel*> *getSpeakers() {return &m_speakers;}

	//! Returns the speaker with a given index.
	Q_INVOKABLE SpeakerModel* getSpeaker(int index) {return m_speakers[index];}
	//! Returns the number of speakers in this team.
	Q_INVOKABLE int speakerCount() {return m_speakers.size();}
	//! Removes the speaker with a given index.
	Q_INVOKABLE void removeSpeaker(int index);

signals:
	//! Emitted when a speaker was added or removed.
	void speakersChanged();
	void teamNameChanged();
public slots:
private slots:
	void updateSpeaker(SpeakerModel*);
	void removeSpeaker(QObject* obj);
private:
	QString m_teamName;
	QList<SpeakerModel*> m_speakers;
};

#endif // TEAM_H
