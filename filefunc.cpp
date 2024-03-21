#include "filefunc.h"

filefunc::filefunc() {
}

void filefunc::saveFile(QString host_t, int ssl_t, QString port_t) {
    QJsonObject data;
    data["host"] = host_t;
    data["ssl"] = ssl_t;
    data["port"] = port_t;
    QFile file("data.json");
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "writing failed " << file.errorString();
        return;
    }

    QJsonDocument doc(data);
    file.write(doc.toJson());
    file.close();
}

QString filefunc::loadFile() {
    QFile file("data.json");

    if(!QFileInfo::exists("data.json")) {
        return "nofile";
    }
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QString val = file.readAll();
    file.close();

    return val;
}

