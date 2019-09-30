#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
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
    keyValueStorage importXmlFile(QString filePath, bool& isParsedProperly);
    void exportXmlToFile(QList<QString> tags, QString filePath);

signals:
    void errorLogSender(QString);

private:
    void addkeyValuePairToStorage(keyValueStorage& storage);

private:
    QXmlStreamReader* xmlReader;
    QXmlStreamWriter* xmlWriter;
};

#endif // XMLPARSER_H
