#include "autocompletemodel.h"

AutoCompleteModel::AutoCompleteModel(QStringList *list, QObject *parent) :
	QObject(parent), m_completer(*list)
{
	m_completer.setCaseSensitivity(Qt::CaseInsensitive);
}

QStringList AutoCompleteModel::completions(QString prefix) {
	QStringList result;
	if (prefix.isEmpty())
		return result;
	m_completer.setCompletionPrefix(prefix);
	for (int i = 0; i < m_completer.completionCount(); ++i) {
		m_completer.setCurrentRow(i);
		QString currentCompletion = m_completer.currentCompletion();
		if (currentCompletion != prefix)
			result.push_back(currentCompletion);
	}
	return result;
}
