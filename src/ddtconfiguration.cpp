#include "ddtconfiguration.h"
#include <QFile>
#include <QDebug>

DDTConfiguration::DDTConfiguration(QString fileName)
{
	QFile device(fileName);
	if (!device.open(QIODevice::ReadOnly)) {
		qWarning() << "Error opening configuration file: " + fileName;
		return;
	}
	QXmlStreamReader reader(&device);
	try {
		readAll(reader);
	}
	catch (QString err) {
		qWarning() << err;
	}
	device.close();
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


}

void DDTConfiguration::readAll(QXmlStreamReader &reader) throw (QString){
	readElement(reader, "options");
	readElement(reader, "names");
	readElementSet(reader, m_names, "name");
	readEndElement(reader); // names
	readElement(reader, "language");
	m_language = readAttribute(reader);
	readEndElement(reader); // language
	readEndElement(reader); // options
}

void DDTConfiguration::addName(QString name)
{
	m_names.insert(name);
}

QString DDTConfiguration::readAttribute(QXmlStreamReader &reader, QString attributeName) throw(QString)
{
	if (!reader.attributes().hasAttribute(attributeName))
		throw attributeName.append(" attribute expected");
	QString result = reader.attributes().value(attributeName).toString();
	reader.readNext();
	return result;
}

void DDTConfiguration::readElementSet(QXmlStreamReader &reader, QSet<QString> &set, QString childName,
										 QString valueName) throw(QString)
{
	while (reader.readNextStartElement()) {
		if (reader.name() != childName)
			throw QString(childName.append(" element expected"));
		set.insert(readAttribute(reader, valueName));
		readEndElement(reader);
	}
}

void DDTConfiguration::readEndElement(QXmlStreamReader &reader) throw(QString) {
	while (reader.isWhitespace())
		reader.readNext();
	if (reader.tokenType() != QXmlStreamReader::EndElement)
		throw QString(reader.name().toString().prepend("End element expected, found "));
	reader.readNext();
}

void DDTConfiguration::readElement(QXmlStreamReader &reader, QString elementName) throw(QString)
{
	if (!reader.readNextStartElement() || reader.name() != elementName)
		throw QString(elementName.append(" expected"));
}

void DDTConfiguration::writeConfigurationFile(QString fileName)
{
	QFile device(fileName);
	if (!device.open(QIODevice::WriteOnly)) {
		qDebug("Error writing configuration file");
		return;
	}
	QXmlStreamWriter writer(&device);

	writer.setAutoFormatting(true);
	writer.writeStartDocument();
	writer.writeStartElement("options");
	writer.writeStartElement("names");
	QSetIterator<QString> i(m_names);
	while (i.hasNext()) {
		writer.writeStartElement("name");
		writer.writeAttribute("value", i.next());
		writer.writeEndElement(); // name
	}
	writer.writeEndElement(); // names
	writer.writeStartElement("language");
	writer.writeAttribute("value", m_language);
	writer.writeEndDocument();
	device.close();
}
