#include <utility>
#include <QFileInfo>
#include "xmlparser.h"

XmlParser::XmlParser(QObject *parent) : QObject (parent)
{
    xmlReader = new QXmlStreamReader();
}

XmlParser::~XmlParser()
{
    delete xmlReader;
}

XmlParser::keyValueStorage XmlParser::importXmlFile(QString filePath,
                                                   bool& isParsedProperly)
{
    keyValueStorage parameterStorage;

    QFile* file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
       emit errorLogSender("unable to open xml file");
    }
    xmlReader->setDevice(file);

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

        QFileInfo fileInfo(file->fileName());
        QString error = fileInfo.fileName() + "\n" + QObject::tr("%1 Line %2, column %3")
                            .arg(xmlReader->errorString())
                            .arg(xmlReader->lineNumber())
                            .arg(xmlReader->columnNumber());
        emit errorLogSender(error);
    } else {
        isParsedProperly = true;
    }
    delete file;
    return parameterStorage;
}

void XmlParser::exportXmlToFile(QList<QString> tags, QString filePath)
{
    /* Открываем файл для Записи с помощью пути, указанного в lineEditWrite */
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    /* Создаем объект, с помощью которого осуществляется запись в файл */
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);  // Устанавливаем автоформатирование текста
    xmlWriter.writeStartDocument();     // Запускаем запись в документ
    xmlWriter.writeStartElement(tags.at(0));   // Записываем первый элемент с его именем

    xmlWriter.writeEndElement();
    /* Завершаем запись в документ
     * */
    xmlWriter.writeEndDocument();
    file.close();   // Закрываем файл
}

void XmlParser::addkeyValuePairToStorage(keyValueStorage& storage)
{
    QString key = xmlReader->name().toString();
    xmlReader->readNext();
    QString value = xmlReader->text().toString();
    storage.push_back(qMakePair(key, value));
}
