#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QXmlStreamReader>
#include <QFile>
#include <QMap>

class XmlParser: QObject
{
    Q_OBJECT

public:
    using keyValue = QPair<QString, QString>;
    using keyValueStorage = QVector<keyValue>;

public:
    explicit XmlParser();
    virtual ~XmlParser();
    keyValueStorage parseXmlFile(QString filePath);

private:
    void addkeyValuePairToStorage(keyValueStorage& storage);

private:
    QXmlStreamReader* xml;
};

#endif // XMLPARSER_H
