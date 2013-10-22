#ifndef PROTOTYPEQLISTQMLWRAPPER_H
#define PROTOTYPEQLISTQMLWRAPPER_H

#include "qlistqmlwrapper.h"

/*!
 * \brief A QListQmlWrapper with roles extrated from a prototype object.
 *
 * QListQmlWrapper cannot be used without specifying its roleNames.
 * \c PrototypeQListQmlWrapper inherits QListQmlWrapper and redefines \c roleNames,
 * the role names are obtained by reading all properties of a given prototype object.
 * For each property
 */
class PrototypeQListQmlWrapper : public QListQmlWrapper
{
	Q_OBJECT
public:
	/*!
	 * \brief Creates a new \c PrototypeQListQmlWrapper with a given prototype.
	 * The objects roleNames are read from the prototype's properties.
	 * \param prototype an arbitrary object
	 * \param parent the object's parent
	 */
	PrototypeQListQmlWrapper(const QObject *prototype, QObject *parent = 0);
	QHash<int, QByteArray> roleNames() const;
signals:
	
public slots:
private:
	QHash<int, QByteArray> m_roleNames;
};

#endif // PROTOTYPEQLISTQMLWRAPPER_H
