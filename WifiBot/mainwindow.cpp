#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), robot()
{
    ui->setupUi(this);

    buttonConnect = new QPushButton("Connect", this);
    buttonConnect->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));

    buttonDisConnect = new QPushButton("Disconnect", this);
    buttonDisConnect->setGeometry(QRect(QPoint(100, 300), QSize(200, 50)));


    buttonUp = new QPushButton("^", this);
    buttonUp->setGeometry(QRect(QPoint(500, 100), QSize(50, 50)));

    buttonDown = new QPushButton("v", this);
    buttonDown->setGeometry(QRect(QPoint(500, 300), QSize(50, 50)));

    buttonLeft = new QPushButton(">", this);
    buttonLeft->setGeometry(QRect(QPoint(600, 200), QSize(50, 50)));

    buttonRight = new QPushButton("<", this);
    buttonRight->setGeometry(QRect(QPoint(400, 200), QSize(50, 50)));

    buttonStop = new QPushButton("Stop", this);
    buttonStop->setGeometry(QRect(QPoint(500, 200), QSize(50, 50)));

    connect(buttonConnect, &QPushButton::released, this, &MainWindow::connexion);
    connect(buttonDisConnect, &QPushButton::released, this, &MainWindow::deconnexion);

    connect(buttonUp, &QPushButton::released, this, &MainWindow::forward);
    connect(buttonDown, &QPushButton::released, this, &MainWindow::backward);

    connect(buttonLeft, &QPushButton::released, this, &MainWindow::left);
    connect(buttonRight, &QPushButton::released, this, &MainWindow::right);
    connect(buttonStop, &QPushButton::released, this, &MainWindow::stop);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connexion()
{
   robot.doConnect();
}

void MainWindow::deconnexion()
{
    robot.disConnect();
}

void MainWindow::forward()
{
    robot.DataToSend.resize(9);
    robot.DataToSend[2] = 120;
    robot.DataToSend[4] = 120;
    robot.DataToSend[6] = 0x50;

    updateCrc();
}

void MainWindow::accelerate()
{
    robot.DataToSend.resize(9);
    robot.DataToSend[2] += 10;
    robot.DataToSend[4] += 10;
    robot.DataToSend[6] = 0x50;

    updateCrc();
}

void MainWindow::backward()
{
    robot.DataToSend.resize(9);
    robot.DataToSend[2] = 120;
    robot.DataToSend[4] = 120;
    robot.DataToSend[6] = 0x00;

    updateCrc();
}

void MainWindow::stop()
{
    robot.DataToSend.resize(9);
    robot.DataToSend[0] = 0xFF;
    robot.DataToSend[1] = 0x07;
    robot.DataToSend[2] = 0x00;
    robot.DataToSend[3] = 0x0;
    robot.DataToSend[4] = 0x00;
    robot.DataToSend[5] = 0x0;
    robot.DataToSend[6] = 0x00;

    updateCrc();
}

void MainWindow::left()
{
    robot.DataToSend.resize(9);
    robot.DataToSend[2] = 120;
    robot.DataToSend[4] = 120;
    robot.DataToSend[6] = 16;

    updateCrc();
}
void MainWindow::right()
{
    robot.DataToSend.resize(9);
    robot.DataToSend[2] = 120;
    robot.DataToSend[4] = 120;
    robot.DataToSend[6] = 64;

    updateCrc();
}

void MainWindow::test()
{
    qDebug() << robot.DataToSend;
}

void MainWindow::updateCrc()
{
    quint16 crc = Crc16(robot.DataToSend,1);
    robot.DataToSend[7] = crc;
    robot.DataToSend[8] = (unsigned char) (crc >> 8);

}
void MainWindow::keyPressEvent(QKeyEvent *event)
    {
        if(event->key() == Qt::Key_Up)
        {
            forward();
            //accelerate();
        }

        if(event->key() == Qt::Key_Q)
        {
            left();
        }

        if(event->key() == Qt::Key_S)
        {
            backward();
        }

        if(event->key() == Qt::Key_D)
        {
            right();
        }

}

short Crc16(QByteArray byteArray, int pos)
{
    byteArray.data();
    unsigned char *data = (unsigned char*)byteArray.constData();
    quint16 crc = 0xFFFF;
    quint16 Polynome = 0xA001;
    quint16 Parity = 0;
    for(; pos < byteArray.length()-2; pos++){
        crc ^= *(data+pos);
        for(unsigned int cptBit = 0; cptBit <= 7; cptBit ++){
            Parity = crc;
            crc >>= 1;
            if (Parity %2 == true) crc ^= Polynome;

        }
    }
        return crc;
    }
