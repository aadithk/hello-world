/*Basic Async Serial Communication - Reading using Qt*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), //Creating object ui.
    mSerialPort{new QSerialPort} //Creating object mSerialPort.
{
    ui->setupUi(this); //Setting up the ui file of the Project.

    mSerialPort->setBaudRate(QSerialPort::Baud19200);
    mSerialPort->setDataBits(QSerialPort::Data8);
    mSerialPort->setParity(QSerialPort::NoParity);
    mSerialPort->setStopBits(QSerialPort::OneStop);
    mSerialPort->setFlowControl(QSerialPort::NoFlowControl);

    connect(mSerialPort, SIGNAL(readyRead()), this, SLOT(readSerialData()));

    /* Syntax of connect() - connect(object1, signal1, object2, slot1)
     * When a signal is emitted, the slots connected to it are usually executed immediately, just like a normal function call.

    /* readyRead() -- This signal is emitted once every time new data is available for reading 
    from the device. It will only be emitted again once new data is available, such as when a new 
    payload of network data has arrived on your network socket also readyRead() is not emitted recursively*/
}

//Destructor.
MainWindow::~MainWindow()
{
    delete mSerialPort;
    delete ui;
}

void MainWindow::readSerialData()
{
    QByteArray lTmpBA;
    lTmpBA = mSerialPort->readAll(); //Reading data from the Serial Port.

    qDebug() << "Received data: " << lTmpBA; //Displaying the data.
}

// Slot to the Button present in the GUI.
void MainWindow::on_pushButton_connect_clicked()
{
    foreach(QSerialPortInfo item, QSerialPortInfo::availablePorts()) {
        if (item.manufacturer() == "Product") { //paste manufacturer here
            mSerialPort->setPort(item);
            if(!mSerialPort->open(QIODevice::ReadWrite)){ //Opening Serial Port for Read and Write.
                QMessageBox::information(this, tr("connect"),
                                         "serialcommunication start"); //Shows an information Dialog Box.
            } else {
                QMessageBox::critical(this, tr("fail"), mSerialPort->errorString());
            }
        } else {
            qDebug() << "No connected device found";
        }
    }
}

// Slot to the Button to disconnect the Serial Communication.
void MainWindow::on_pushButton_disconnect_clicked()
{
    mSerialPort->close();
}