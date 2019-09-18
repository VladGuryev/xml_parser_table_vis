#include "xmlparser.h"
#include <QtDebug>
#include <utility>

XmlParser::XmlParser()
{
    xml = new QXmlStreamReader();
}

XmlParser::~XmlParser()
{
    delete xml;
}

XmlParser::keyValueStorage XmlParser::parseXmlFile(QString filePath)
{
    keyValueStorage parameterStorage;

    QFile* file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
       qDebug() << "unable to open xml file";
    }
    xml->setDevice(file);

    while (!xml->atEnd()){
        QXmlStreamReader::TokenType token = xml->readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement){
            if (xml->name() == "root")
                continue;
            if (xml->name() == "texteditor"){
                addkeyValuePairToStorage(parameterStorage);
            }
            if (xml->name() == "fileformats"){
               addkeyValuePairToStorage(parameterStorage);
            }
            if (xml->name() == "encoding"){
                addkeyValuePairToStorage(parameterStorage);
            }
            if (xml->name() == "hasintellisense"){
               addkeyValuePairToStorage(parameterStorage);
            }
            if (xml->name() == "hasplugins"){
               addkeyValuePairToStorage(parameterStorage);
            }
            if (xml->name() == "cancompile"){
               addkeyValuePairToStorage(parameterStorage);
            }
        }
    }
    if(xml->hasError()){
        qDebug() << QObject::tr("%1\nLine %2, column %3")
                    .arg(xml->errorString())
                    .arg(xml->lineNumber())
                    .arg(xml->columnNumber());
    }
    return parameterStorage;
}

void XmlParser::addkeyValuePairToStorage(keyValueStorage& storage)
{
    QString key = xml->name().toString();
    xml->readNext();
    QString value = xml->text().toString();
    storage.push_back(qMakePair(key, value));
}
