#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QXmlStreamReader>
#include <QFile>
#include <QMap>

class XmlParser: QObject
{
    Q_OBJECT
    using keyValueStorage = QVector<QPair<QString, QString>>;

public:
    explicit XmlParser();
    virtual ~XmlParser();
    keyValueStorage parseXmlFile(QString filePath);

private:
    void addkeyValuePairToStorage();

private:
    QXmlStreamReader* xml;
    keyValueStorage parameterStorage;
};

#endif // XMLPARSER_H
