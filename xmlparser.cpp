#include <utility>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>
#include "xmlparser.h"

XmlParser::XmlParser(QObject *parent) : QObject (parent)
{
    xmlReader = std::make_unique<QXmlStreamReader>();
    xmlWriter = std::make_unique<QXmlStreamWriter>();
}

XmlParser::~XmlParser()
{
}

XmlParser::keyValueStorage XmlParser::importXmlFile(QString filePath,
                                                   bool& isParsedProperly)
{
    keyValueStorage parameterStorage;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       emit errorLogSender("unable to open xml file for import");
    }
    xmlReader->setDevice(&file);

    while (!xmlReader->atEnd() && !xmlReader->hasError()){
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement){
            if (xmlReader->name() == "root")
                continue;
            if (xmlReader->name() == "texteditor"){
                addkeyValuePairToStorage(parameterStorage);
            }
            if (xmlReader->name() == "fileformats"){
               addkeyValuePairToStorage(parameterStorage);
            }
            if (xmlReader->name() == "encoding"){
                addkeyValuePairToStorage(parameterStorage);
            }
            if (xmlReader->name() == "hasintellisense"){
               addkeyValuePairToStorage(parameterStorage);
            }
            if (xmlReader->name() == "hasplugins"){
               addkeyValuePairToStorage(parameterStorage);
            }
            if (xmlReader->name() == "cancompile"){
               addkeyValuePairToStorage(parameterStorage);
            }
        }
    }
    if(xmlReader->hasError()){
        isParsedProperly = false;

        QFileInfo fileInfo(file.fileName());
        QString error = fileInfo.fileName() + "\n" + QObject::tr("%1 Line %2, column %3")
                            .arg(xmlReader->errorString())
                            .arg(xmlReader->lineNumber())
                            .arg(xmlReader->columnNumber());
        emit errorLogSender(error);
    } else {
        isParsedProperly = true;
    }
    return parameterStorage;
}

void XmlParser::addkeyValuePairToStorage(keyValueStorage& storage)
{
    QString key = xmlReader->name().toString();
    xmlReader->readNext();
    QString value = xmlReader->text().toString();
    storage.push_back(qMakePair(key, value));
}

void XmlParser::exportXmlToFile(const CustomModel::Row& tagNames,
                                const QVector<CustomModel::Row>& tagDataRows,
                                QFile* const file)
{
    /* Создаем объект, с помощью которого осуществляется запись в файл */
    xmlWriter->setDevice(file);
    xmlWriter->setAutoFormatting(true);     // Устанавливаем автоформатирование текста
    xmlWriter->writeStartDocument();        // Запускаем запись в документ
    xmlWriter->writeStartElement("TextEditorList");   // Записываем root элемент с его именем

    const int fileNumber(tagDataRows.count());
    for(int i = 0; i < fileNumber; i++){
        xmlWriter->writeStartElement(tagNames.at(0));
        xmlWriter->writeAttribute("name", tagDataRows.at(i).at(0));
        for(int j = 1; j < tagNames.count(); j++){
            this->writeXmlTag(tagNames.at(j), tagDataRows.at(i).at(j));
        }
        xmlWriter->writeEndElement();
    }
    /* Закрываем тег "TextEditorList" */
    xmlWriter->writeEndElement();
    /* Завершаем запись в документ */
    xmlWriter->writeEndDocument();
}

void XmlParser::writeXmlTag(QString tagName, QString tagData)
{
    xmlWriter->writeStartElement(tagName);  // Открываем тег
    bool hasBooleanAttribute((tagData == "true") || (tagData == "false"));
    if(hasBooleanAttribute){
        xmlWriter->writeAttribute("boolean", tagData);
    }
    xmlWriter->writeCharacters(tagData);
    xmlWriter->writeEndElement();        // Закрываем тег
}

