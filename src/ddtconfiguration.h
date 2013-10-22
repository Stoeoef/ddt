#ifndef DDTCONFIGURATION_H
#define DDTCONFIGURATION_H

#include <QSet>
#include <QXmlStreamReader>

/*!
 * \brief Reads options from a file.
 *
 * Reads generic configuration options from a given file, e.g. the applications language.
 * The configuration is an xml-formatted document. Furthermore, writing or creating property
 * files is supported.
 */
class DDTConfiguration
{
public:
	/*!
	 * \brief DDTConfiguration Creates a new configuration object for a given configuration file
	 *
	 * A warning is given if the configuration file could not be loaded. In this case the class
	 * will return default values as properties
	 * \param fileName the (relative) path to the configuration file.
	 */
	DDTConfiguration(QString fileName);
	void writeConfigurationFile(QString fileName);

	/*!
	 * \brief Returns a list of names used for autocompletion.
	 *
	 * An empty list is returned if the configuration file could not be loaded.
	 */
	QSet<QString> names() const {return m_names;}

	/*!
	 * \brief addName Adds a name to the list of known names used for autocompletion.
	 *
	 */
	void addName(QString name);

	/*! \brief Returns the applications language (e.g. "en" or "de")
	 *
	 * An empty string is returned if the configuration file could not be loaded.
	 */
	QString language() const {return m_language;}

	/*!
	 * \brief Sets the applications language that will be written in the configuration file.
	 *
	 * \param language a country code
	 */
	void setLanguage(QString language) {m_language = language;}


private:
	void readAll(QXmlStreamReader &reader) throw (QString);
	QString readAttribute(QXmlStreamReader &reader, QString attributeName="value") throw (QString);
	void readElementSet(QXmlStreamReader &reader, QSet<QString> &set,
						   QString childName, QString valueName="value") throw (QString);
	void readEndElement(QXmlStreamReader &reader) throw(QString);
	void readElement(QXmlStreamReader &reader, QString elementName) throw (QString);

	QSet<QString> m_names;
	QString m_language;
};

#endif // DDTCONFIGURATION_H
