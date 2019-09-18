#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QXmlStreamReader>
#include <QFile>
#include <functional>

class XmlParser: public QObject
{
    Q_OBJECT

public:
    using keyValue = QPair<QString, QString>;
    using keyValueStorage = QVector<keyValue>;

public:
    explicit XmlParser(QObject *parent = nullptr);
    virtual ~XmlParser();
    keyValueStorage parseXmlFile(QString filePath, bool& isParsedProperly);

signals:
    void errorLogSender(QString);

private:
    void addkeyValuePairToStorage(keyValueStorage& storage);

private:
    QXmlStreamReader* xml;
};

#endif // XMLPARSER_H
