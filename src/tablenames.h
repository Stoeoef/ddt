#ifndef TABLENAMES_H
#define TABLENAMES_H

#include <QObject>
#include <QString>
#include <QHash>
#include "speakermodel.h"

/*!
 * \brief Stores speakers mapped to a table in a debate.
 */
class TableNames : public QObject
{
	Q_OBJECT
public:
	//! Creates a new \c TableNames object with a given parent.
	explicit TableNames(QObject *parent = 0);
	//! Gets the speaker sitting at \c index
	Q_INVOKABLE SpeakerModel* getSpeaker(int index);
	//! Defines the speaker sitting at \c index
	void addMapping(int index, SpeakerModel *speaker);

signals:
	
private slots:
	void speakerDestroyed(QObject* speaker);
private:

	QHash<int, SpeakerModel*> m_names;

};

#endif // TABLENAMES_H
