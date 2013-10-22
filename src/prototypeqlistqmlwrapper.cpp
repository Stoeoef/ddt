#include "prototypeqlistqmlwrapper.h"

PrototypeQListQmlWrapper::PrototypeQListQmlWrapper(const QObject* prototype, QObject *parent) :
	QListQmlWrapper(parent)
{
	Q_ASSERT(prototype);
	const QMetaObject *metaObject = prototype->metaObject();
	int count = metaObject->propertyCount();
	for (int i = 0; i < count; ++i) {
		QMetaProperty metaProperty = metaObject->property(i);
		const char *name = metaProperty.name();
		m_roleNames.insert(Qt::UserRole + i, name);
	}
}

QHash<int, QByteArray> PrototypeQListQmlWrapper::roleNames() const
{
	return m_roleNames;
}
