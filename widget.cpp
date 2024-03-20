#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->pbConnect->setDisabled(false);
    ui->pbDisconnect->setDisabled(true);
    ui->cbSSL->setCheckState(Qt::Unchecked);

    QObject::connect(&socket_, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&socket_, &QAbstractSocket::disconnected, this,  &Widget::doDisconnected);
    QObject::connect(&socket_, &QAbstractSocket::readyRead, this,  &Widget::doReadyRead);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::disableButtons(bool connected){
    ui->pbConnect->setDisabled(connected);
    ui->pbDisconnect->setDisabled(!connected);
    ui->pbSend->setDisabled(!connected);
    ui->cbSSL->setDisabled(!connected);
}

void Widget::doConnected(){
    ui->pteMessage->insertPlainText("Connected\n");
    disableButtons(true);
}

void Widget::doDisconnected(){
    ui->pteMessage->insertPlainText("Disconnected\n");
    disableButtons(false);
}

void Widget::doReadyRead(){
    ui->pteMessage->insertPlainText(socket_.readAll());
}

void Widget::on_pbConnect_clicked()
{
    try {
        if(ui->cbSSL->checkState() == Qt::Checked) {
            socket_.connectToHostEncrypted(ui->leHost->text(), ui->lePort->text().toUShort());
            ui->pteMessage->insertPlainText("SSL Connecting...");
            throw socket_.waitForConnected(2000);
        }
        socket_.connectToHost(ui->leHost->text(), ui->lePort->text().toUShort());
        ui->pteMessage->insertPlainText("Connecting...");
        throw socket_.waitForConnected(2000);

    } catch (bool e) {
        if(!e) {
            ui->pteMessage->insertPlainText("Connection Failed\n");
        }
    }
}

void Widget::on_pbDisconnect_clicked()
{
    socket_.close();
}

void Widget::on_pbSend_clicked()
{
    socket_.write(ui->pteSend->toPlainText().toUtf8());
}

void Widget::on_cbSSL_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked) {
        ui->lePort->setText("443");
        return;
    }
    ui->lePort->setText("80");
}

void Widget::on_pbClear_clicked()
{
    ui->pteMessage->clear();
}

