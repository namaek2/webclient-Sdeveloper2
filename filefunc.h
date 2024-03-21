#ifndef FILEFUNC_H
#define FILEFUNC_H

#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QJsonDocument>

class filefunc
{
public:
    filefunc();

    void saveFile(QString host, int ssl_t, QString port);
    QString loadFile();
};

#endif // FILEFUNC_H
