#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include <functional>
#include <memory>
#include <model.h>

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
    void exportXmlToFile(const CustomModel::Row& header,
                         const QVector<CustomModel::Row>& rows,
                         QFile* const file);

signals:
    void errorLogSender(QString);

private:
    void addkeyValuePairToStorage(keyValueStorage& storage);

private:
    std::unique_ptr<QXmlStreamReader> xmlReader;
    std::unique_ptr<QXmlStreamWriter> xmlWriter;
};

#endif // XMLPARSER_H
