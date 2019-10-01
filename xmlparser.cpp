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

void XmlParser::exportXmlToFile(const CustomModel::Row& header,
                                const QVector<CustomModel::Row>& rows,
                                QFile* const file)
{
    /* Создаем объект, с помощью которого осуществляется запись в файл */
    xmlWriter->setDevice(file);
    xmlWriter->setAutoFormatting(true);  // Устанавливаем автоформатирование текста
    xmlWriter->writeStartDocument();     // Запускаем запись в документ
    xmlWriter->writeStartElement(header.at(0));   // Записываем первый элемент с его именем
    qDebug() << header;
    xmlWriter->writeEndElement();
    /* Завершаем запись в документ
     * */
    xmlWriter->writeEndDocument();
}

void XmlParser::addkeyValuePairToStorage(keyValueStorage& storage)
{
    QString key = xmlReader->name().toString();
    xmlReader->readNext();
    QString value = xmlReader->text().toString();
    storage.push_back(qMakePair(key, value));
}
