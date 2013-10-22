#ifndef SPEAKERMODEL_H
#define SPEAKERMODEL_H

#include <QObject>
#include "team.h"


class Team;

//!
//! \brief Represents a speaker in a debate.

//!A speaker will be placed on tables according to his properties during the
//!matching process. The speaker can prevent himself from being mapped to
//!certain unwanted positions, marked as a beginner or assigned to a team.
class SpeakerModel : public QObject
{
    Q_OBJECT
	//!The speaker's name.
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	//!Holds if the speaker is a beginner. Beginners won't be
	//!in the same team.
	Q_PROPERTY(bool isBeginner READ isBeginner WRITE setIsBeginner NOTIFY isBeginnerChanged)
	//!The speaker's team or \c NULL if he is not member of a team. Setting this property
	//!will add the speaker to the team's speaker list. Setting a value of \c NULL will
	//!delete the speaker from the team's speaker list. Destroying the team will set this
	//!property to \c NULL
	Q_PROPERTY(Team* team READ team WRITE setTeam NOTIFY teamChanged)

	//!Holds if the the speaker can be mapped to the FFR-role of an OP-debate.
	//! \sa OPDDebate
	Q_PROPERTY(bool opdFfr READ opdFfr WRITE setOpdFfr NOTIFY opdFfrChanged)
	//!Holds if the speaker can be mapped to the government side in an OP-debate.
	//! \sa OPDDebate
	Q_PROPERTY(bool opdGov READ opdGov WRITE setOpdGov NOTIFY opdGovChanged)
	//!Holds if the speaker can be mapped to the opposition side in an OP-debate.
	//! \sa OPDDebate
    Q_PROPERTY(bool opdOp READ opdOp WRITE setOpdOp NOTIFY opdOpChanged)
	//!Holds if the speaker can be mapped to the government side in an BPS-debate.
	//! \sa BPSDebate
    Q_PROPERTY(bool bpsGov READ bpsGov WRITE setBpsGov NOTIFY bpsGovChanged)
	//!Holds if the speaker can be mapped to the oppositoin side in an BPS-debate.
	//! \sa BPSDebate
	Q_PROPERTY(bool bpsOp READ bpsOp WRITE setBpsOp NOTIFY bpsOpChanged)
	//!Holds if the speaker can be set to judicate an OP-debate.
	//! \sa Debate
	Q_PROPERTY(bool opdJud READ opdJud WRITE setOpdJud NOTIFY opdJudChanged)
	//!Holds if the speaker can be set to judicate an BPS-debate.
	//! \sa Debate
	Q_PROPERTY(bool bpsJud READ bpsJud WRITE setBpsJud NOTIFY bpsJudChanged)


public:
	//!Creates a new speaker without any role restrictions. The speaker is set as
	//!no beginner and has no team.
	SpeakerModel(QObject *parent = 0);

	QString name() {return m_name;}
	void setName(QString name);

	bool isBeginner() {return m_isBeginner;}
	void setIsBeginner(bool isBeginner);

	Team* team() {return m_team;}
	void setTeam(Team* team);

	bool opdFfr() {return m_opdFfr;}
	void setOpdFfr(bool opdFfr);

	bool opdGov() {return m_opdGov;}
	void setOpdGov(bool opdGov);

	bool opdOp() {return m_opdOp;}
	void setOpdOp(bool opdOp);

	bool bpsOp() {return m_bpsOp;}
	void setBpsOp(bool bpsOp);

	bool bpsGov() {return m_bpsGov;}
	void setBpsGov(bool bpsGov);

	bool opdJud() {return m_opdJud;}
	void setOpdJud(bool opdJud);

	bool bpsJud() {return m_bpsJud;}
	void setBpsJud(bool bpsJud);

signals:
    void nameChanged(QString);
	void isBeginnerChanged(bool);
	void opdFfrChanged(bool);
	void opdGovChanged(bool);
	void opdOpChanged(bool);
	void bpsGovChanged(bool);
	void bpsOpChanged(bool);
	void opdJudChanged(bool);
	void bpsJudChanged(bool);
	void teamChanged(SpeakerModel*);
public slots:
	//!Same as
	//!\code setTeam(NULL) \endcode
	//!\sa setTeam
	void resetTeam(){setTeam(NULL);}
private:
    QString m_name;
	bool m_isBeginner;
	bool m_opdFfr;
	bool m_opdOp;
	bool m_opdGov;
	bool m_bpsOp;
	bool m_bpsGov;
	bool m_opdJud;
	bool m_bpsJud;
	Team* m_team;
};

#endif // SPEAKERMODEL_H
