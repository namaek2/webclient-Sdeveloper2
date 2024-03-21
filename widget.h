#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QSslSocket>
#include <QUdpSocket>
#include "filefunc.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    //QSslSocket inherite QTcpSocket
    QSslSocket socket_;
    void init();
    void initFailed();
    //QTcpSocket tcp_socket_;
    //QUdpSocket socket_;

public slots:
    void doConnected();
    void doDisconnected();
    void doReadyRead();
    void disableButtons(bool connected);

private slots:

    void on_pbConnect_clicked();

    void on_pbDisconnect_clicked();

    void on_pbSend_clicked();

    void on_cbSSL_stateChanged(int arg1);

    void on_pbClear_clicked();

    void on_pbSaveExit_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
