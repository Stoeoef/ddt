/**
 * Copyright (C) 2013  Stefan Altmayer
 *
 * This file is part of DDT.
 *
 * DDT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DDT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DDT.  If not, see <http://www.gnu.org/licenses/>.
 */

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
