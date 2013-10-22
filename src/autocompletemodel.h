#ifndef AUTOCOMPLETEMODEL_H
#define AUTOCOMPLETEMODEL_H

#include <QObject>
#include <QStringList>
#include <QCompleter>


/*!
 * \brief Offers autocompletion accessible from QML.
 *
 * Offers simple auto completion functions similar to QCompleter that
 * can be accessed from QML. The pool of possible completions is stored
 * a \ref QStringList .
 */
class AutoCompleteModel : public QObject
{
	Q_OBJECT
public:
	/*!
	 * \brief Creates a new \c AutoCompleteModel with a given list of completions.
	 *
	 * Change \c list afterwards to use a different set of autocompletions.
	 * \param list a list of possible completions
	 * \param parent the object's parent
	 */
	explicit AutoCompleteModel(QStringList *list = 0, QObject *parent = 0);

	/*!
	 * \brief Returns a list of possible completions for a given search term.
	 *
	 * The search is not case sensitive. The list of search terms is defined by
	 * \ref AutoCompleteModel#AutoCompleteModel
	 * \param prefix a prefix that will be completed
	 * \return a list with possible completions beginning with \c prefix
	 */
	Q_INVOKABLE QStringList completions(QString prefix);

signals:
	
public slots:
private:
	QCompleter m_completer;
};

#endif // AUTOCOMPLETEMODEL_H
