#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), robot()
{
    ui->setupUi(this);

    buttonConnect = new QPushButton("Connect", this);
    buttonConnect->setGeometry(QRect(QPoint(10, 10), QSize(200, 50)));

    buttonDisConnect = new QPushButton("Disconnect", this);
    buttonDisConnect->setGeometry(QRect(QPoint(10, 80), QSize(200, 50)));


    buttonUp = new QPushButton("^", this);
    buttonUp->setGeometry(QRect(QPoint(110, 200), QSize(50, 50)));

    buttonDown = new QPushButton("v", this);
    buttonDown->setGeometry(QRect(QPoint(110, 400), QSize(50, 50)));

    buttonRight = new QPushButton(">", this);
    buttonRight->setGeometry(QRect(QPoint(210, 300), QSize(50, 50)));

    buttonLeft = new QPushButton("<", this);
    buttonLeft->setGeometry(QRect(QPoint(10, 300), QSize(50, 50)));

    buttonStop = new QPushButton("Stop", this);
    buttonStop->setGeometry(QRect(QPoint(110, 300), QSize(50, 50)));

    connect(&robot,&MyRobot::updateUI,this,&MainWindow::read);
    connect(buttonConnect, &QPushButton::released, this, &MainWindow::connexion);
    connect(buttonDisConnect, &QPushButton::released, this, &MainWindow::deconnexion);

    connect(buttonUp, &QPushButton::released, this, &MainWindow::forward);
    connect(buttonDown, &QPushButton::released, this, &MainWindow::backward);

    connect(buttonLeft, &QPushButton::released, this, &MainWindow::left);
    connect(buttonRight, &QPushButton::released, this, &MainWindow::right);
    connect(buttonStop, &QPushButton::released, this, &MainWindow::stop);

    speed = new QSpinBox(this);
    speed->setGeometry(QRect(QPoint(10, 500), QSize(50, 30)));
    speed->setMaximum(240);
    speed->setValue(160);

    QFont f("Arial", 15, QFont::Bold);
    depgauche = new QLabel("debug",this);
    depgauche->setGeometry(340,40,50,50);
    depdroite = new QLabel("debug",this);
    depdroite->setGeometry(400,40,50,50);
    dep = new QLabel(depgauche->text()+depdroite->text(),this);
    dep->setGeometry(360,60,100,50);


    textConnected = new QLabel("Non Connecté",this);
    textConnected->setGeometry(330,10,500,50);
    textConnected->setFont(f);

    keys[0] = 0;
    keys[1] = 0;
    keys[2] = 0;
    keys[3] = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connexion()
{
   robot.doConnect();
   if(robot.isConnected())textConnected->setText("Connecté");
}

void MainWindow::read(){
//qDebug() << robot.DataReceived;
    //Affichage de la batterie
    unsigned char batteryLvl = robot.DataReceived[2];
    if(batteryLvl>150)//le robot est chargé
        this->ui->batterie->setText("Charging...");
    else{//la val de la batterie va de 101 a 125
        this->ui->batterie->setText(QString::number(qRound((batteryLvl-102)/1.25))+" %");
    }
    //Gestion des infrarouges
    unsigned char irAvG = robot.DataReceived[3];
    unsigned char irArrG = robot.DataReceived[4];
    unsigned char irAvD = robot.DataReceived[11];
    //le capteur affiche toujours 255
    //unsigned char irArrD = robot.DataReceived[12];
     qDebug() << QString::number(irAvG) << QString::number(irAvD) <<QString::number(irArrG); //<< QString::number(irArrD);




    //odometrie
    //les vals initiales envoyées par le robot
    if(odol==0){
        odol = ((((long)robot.DataReceived[8]<<24)) + (((long)robot.DataReceived[7]<<16 ))+ (((long)robot.DataReceived[6]<<8 ))+ ((long)robot.DataReceived[5]));
        odor = ((((long)robot.DataReceived[16]<<24)) + (((long)robot.DataReceived[15]<<16 ))+ (((long)robot.DataReceived[14]<<8 ))+ ((long)robot.DataReceived[13]));
    }

    odoG= ((((long)robot.DataReceived[8]<<24)) + (((long)robot.DataReceived[7]<<16 ))+ (((long)robot.DataReceived[6]<<8 ))+ ((long)robot.DataReceived[5])) -odol;
    odoD= ((((long)robot.DataReceived[16]<<24)) + (((long)robot.DataReceived[15]<<16 ))+ (((long)robot.DataReceived[14]<<8 ))+ ((long)robot.DataReceived[13])) -odor;

    depgauche->setText(QString::number((long)odoG));
    depdroite->setText( QString::number((long)odoD));
    //moyenne du deplacement des deux roues
    dep->setText(QString::number((long)(odoG+odoD)));

}

void MainWindow::deconnexion()
{
    if(robot.isConnected()){
        robot.disConnect();
        if(!robot.isConnected())textConnected->setText("Non connecté");
    }
}

void MainWindow::forward()
{
    robot.DataToSend.resize(9);
    robot.DataToSend[2] = speed->value();
    robot.DataToSend[4] = speed->value();
    robot.DataToSend[6] = 0x50;

    buttonUp->setStyleSheet("background-color : rgb(255,255,0);");
    buttonLeft->setStyleSheet("");
    buttonRight->setStyleSheet("");
    buttonDown->setStyleSheet("");
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
    robot.DataToSend[2] = speed->value();
    robot.DataToSend[4] = speed->value();
    robot.DataToSend[6] = 0x00;

    buttonDown->setStyleSheet("background-color : rgb(255,255,0);");
    buttonLeft->setStyleSheet("");
    buttonRight->setStyleSheet("");
    buttonUp->setStyleSheet("");

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

    buttonDown->setStyleSheet("");
    buttonLeft->setStyleSheet("");
    buttonRight->setStyleSheet("");
    buttonUp->setStyleSheet("");

    updateCrc();
}

void MainWindow::left()
{
    robot.DataToSend.resize(9);
    robot.DataToSend[2] = speed->value();
    robot.DataToSend[4] = speed->value();
    robot.DataToSend[6] = 16;

    buttonLeft->setStyleSheet("background-color : rgb(255,255,0);");
    buttonUp->setStyleSheet("");
    buttonRight->setStyleSheet("");
    buttonDown->setStyleSheet("");

    updateCrc();
}

void MainWindow::leftUp()
{
    robot.DataToSend.resize(9);
    robot.DataToSend[2] = speed->value();
    robot.DataToSend[4] = speed->value()/2;
    robot.DataToSend[6] = 0x50;

    buttonLeft->setStyleSheet("background-color : rgb(255,255,0);");
    buttonUp->setStyleSheet("background-color : rgb(255,255,0);");
    buttonRight->setStyleSheet("");
    buttonDown->setStyleSheet("");

    updateCrc();
}

void MainWindow::leftDown()
{
    robot.DataToSend.resize(9);
    robot.DataToSend[2] = speed->value();
    robot.DataToSend[4] = speed->value()/2;
    robot.DataToSend[6] = 0x00;

    buttonLeft->setStyleSheet("background-color : rgb(255,255,0);");
    buttonUp->setStyleSheet("");
    buttonRight->setStyleSheet("");
    buttonDown->setStyleSheet("background-color : rgb(255,255,0);");

    updateCrc();
}
void MainWindow::right()
{
    robot.DataToSend.resize(9);
    robot.DataToSend[2] = speed->value();
    robot.DataToSend[4] = speed->value();
    robot.DataToSend[6] = 64;

    buttonRight->setStyleSheet("background-color : rgb(255,255,0);");
    buttonLeft->setStyleSheet("");
    buttonUp->setStyleSheet("");
    buttonDown->setStyleSheet("");

    updateCrc();
}

void MainWindow::rightUp()
{
    robot.DataToSend.resize(9);
    robot.DataToSend[2] = speed->value()/2;
    robot.DataToSend[4] = speed->value();
    robot.DataToSend[6] = 0x50;

    buttonLeft->setStyleSheet("");
    buttonUp->setStyleSheet("background-color : rgb(255,255,0);");
    buttonRight->setStyleSheet("background-color : rgb(255,255,0);");
    buttonDown->setStyleSheet("");

    updateCrc();
}

void MainWindow::rightDown()
{
    robot.DataToSend.resize(9);
    robot.DataToSend[2] = speed->value()/2;
    robot.DataToSend[4] = speed->value();
    robot.DataToSend[6] = 0x00;

    buttonLeft->setStyleSheet("");
    buttonUp->setStyleSheet("");
    buttonRight->setStyleSheet("background-color : rgb(255,255,0);");
    buttonDown->setStyleSheet("background-color : rgb(255,255,0);");

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

        switch( event->key())
        {
        case Qt::Key_Z :
            keys[0] = 1;
            if(keys[1] == 1)leftUp();
            else if(keys[3] == 1)rightUp();
            else
            forward();
            break;

        case Qt::Key_Q :
            keys[1] = 1;
            if(keys[0] == 1)leftUp();
            else if(keys[2] == 1)leftDown();
            else
            left();
            break;

        case Qt::Key_S :
            keys[2] = 1;
            if(keys[1] == 1)leftDown();
            else if(keys[3] == 1)rightDown();
            else
            backward();
            break;

        case Qt::Key_D :
            keys[3] = 1;
            if(keys[0] == 1)rightUp();
            else if(keys[2] == 1)rightDown();
            else
            right();
            break;

        case Qt::Key_M :
            stop();
            break;
        }



}

void MainWindow::keyReleaseEvent(QKeyEvent *event){

    stop();

    switch( event->key())
    {
    case Qt::Key_Z :
        keys[0] = 0;
        break;

    case Qt::Key_Q :
        keys[1] = 0;
        break;

    case Qt::Key_S :
        keys[2] = 0;
        break;

    case Qt::Key_D :
        keys[3] = 0;
        break;
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
