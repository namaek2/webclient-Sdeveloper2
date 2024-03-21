#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();

    QObject::connect(&socket_, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&socket_, &QAbstractSocket::disconnected, this,  &Widget::doDisconnected);
    QObject::connect(&socket_, &QAbstractSocket::readyRead, this,  &Widget::doReadyRead);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init() {

    ui->pbConnect->setDisabled(false);
    ui->pbDisconnect->setDisabled(true);

    filefunc ff;
    QString val = ff.loadFile();
    if(val=="failed"){
        initFailed();
    }

    QByteArray data = val.toUtf8();
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(data, &errorPtr);

    if (doc.isNull()) {
        initFailed();
    }

    QJsonObject datas = doc.object();
    ui->leHost->setText(datas.value("host").toString());
    ui->lePort->setText(datas.value("port").toString());

    if(datas.value("ssl") == 0) {
        ui->cbSSL->setCheckState(Qt::Unchecked);
        return;
    }
    ui->cbSSL->setCheckState(Qt::Checked);
}

void Widget::initFailed() {
    ui->leHost->setText("www.naver.com");
    ui->lePort->setText("80");
    ui->cbSSL->setCheckState(Qt::Unchecked);
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

void Widget::on_pbSaveExit_clicked()
{
    filefunc ff;
    ff.saveFile(ui->leHost->text(), ui->cbSSL->checkState(), ui->lePort->text());
    QApplication::quit();
}

