#ifndef SEATTOSPEAKERMAPPER_H
#define SEATTOSPEAKERMAPPER_H

#include <QObject>
#include "speakermodel.h"
#include "debate.h"
#include "tablenames.h"

/*!
 * \brief Represents the results of a matching process.
 *
 * \c SeatToSpeakerMapper describes a mapping from different places in a debate to
 * speakers that were mapped to these places.
 */
class SeatToSpeakerMapper : public QObject
{
	Q_OBJECT
public:
	//! Creates a new SeatToSpeakerMapper with a given parent.
	explicit SeatToSpeakerMapper(QObject *parent = 0);
	/*!
	 * \brief Maps a speaker to a specific seat in a debate.
	 * \param debate the debate in which the speaker is seated
	 * \param group the role to which the speaker was mapped
	 * \param seatIndex the speaker's position at his table
	 * \param speaker a speaker that is mapped to a given seat
	 */
	void addMapping(Debate* debate, Debate::RoleGroup group, int seatIndex, SpeakerModel* speaker);

	/*!
	 * \brief Returns a \c TableNames object representing the speakers at some table.
	 * \param debate the debate where the table is placed
	 * \param group the role group to which the table belongs to
	 * \return A \c TableNames object representing a table.
	 */
	TableNames* getNamesForTable(Debate* debate, Debate::RoleGroup group);
	Q_INVOKABLE TableNames* getNamesForTable(Debate* debate, int group);

private:
	QHash<Debate*, QHash<Debate::RoleGroup, TableNames* > * > m_mapping;
};

#endif // SEATTOSPEAKERMAPPER_H
