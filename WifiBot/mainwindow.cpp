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

    camUp = new QPushButton("^", this);
    camUp->setGeometry(QRect(QPoint(630, 250), QSize(50, 50)));

    camDown = new QPushButton("v", this);
    camDown->setGeometry(QRect(QPoint(630, 350), QSize(50, 50)));

    camLeft = new QPushButton("<", this);
    camLeft->setGeometry(QRect(QPoint(580, 300), QSize(50, 50)));

    camRight = new QPushButton(">", this);
    camRight->setGeometry(QRect(QPoint(680, 300), QSize(50, 50)));

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
    connect(camUp, &QPushButton::released, this, &MainWindow::moveCamUp);
    connect(camDown, &QPushButton::released, this, &MainWindow::moveCamDown);
    connect(camLeft, &QPushButton::released, this, &MainWindow::moveCamLeft);
    connect(camRight, &QPushButton::released, this, &MainWindow::moveCamRight);

    speed = new QSpinBox(this);
    speed->setGeometry(QRect(QPoint(10, 500), QSize(50, 30)));
    speed->setMaximum(240);
    speed->setValue(160);

    QFont f("Arial", 15, QFont::Bold);
    depgauche = new QLabel("",this);
    depgauche->setGeometry(340,40,50,50);
    depdroite = new QLabel("",this);
    depdroite->setGeometry(400,40,50,50);
    dep = new QLabel(depgauche->text()+depdroite->text(),this);
    dep->setGeometry(360,60,100,50);


    QFont ir("Arial", 8, QFont::Bold);
    avg = new QLabel("avg",this);
    avg->setGeometry(600,80,20,20);
    avg->setFont(ir);

    avd = new QLabel("avd",this);
    avd->setGeometry(720,80,20,20);
    avd->setFont(ir);

    arg = new QLabel("arg",this);
    arg->setGeometry(600,160,20,20);
    arg->setFont(ir);

    ard = new QLabel("ard",this);
    ard->setGeometry(720,160,20,20);
    ard->setFont(ir);


    textConnected = new QLabel("Non Connecté",this);
    textConnected->setGeometry(330,10,500,50);
    textConnected->setFont(f);

    keys[0] = 0;
    keys[1] = 0;
    keys[2] = 0;
    keys[3] = 0;

    camera = new QWebEngineView(this);
    camera->load(QUrl("http://192.168.1.11:8080/?action=stream"));
    camera->setGeometry(260,200,320,240);
    camera->show();

    manager = new QNetworkAccessManager(this);

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
    if(batteryLvl>150)//le robot est branché
        this->ui->batterie->setText("Charging...");
    else{//la val de la batterie va de 101 a 125
        this->ui->batterie->setText(QString::number(qRound((batteryLvl-102)/1.25))+" %");
    }


    //Gestion des infrarouges

    iravg = robot.DataReceived[3];

    //le capteur est cassé, il affiche toujours 255
    irarg= robot.DataReceived[12];

    iravd= robot.DataReceived[11];
    irard = robot.DataReceived[4];

    avg->setText(QString::number(iravg));
    avd->setText(QString::number(iravd));
    arg->setText(QString::number(irarg));
    ard->setText(QString::number(irard));
    ard->setStyleSheet( "color : rgb("+QString::number(irard)+",0,0);");
    avd->setStyleSheet( "color : rgb("+QString::number(iravd)+",0,0);");
    arg->setStyleSheet( "color : rgb("+QString::number(irarg)+",0,0);");
    avg->setStyleSheet( "color : rgb("+QString::number(iravg)+",0,0);");



    //odometrie
    //les vals initiales envoyées par le robot
    if(odol==0){
        odol = (((( long)robot.DataReceived[8]<<24)) + (((long)robot.DataReceived[7]<<16 ))+ (((long)robot.DataReceived[6]<<8 ))+ ((long)robot.DataReceived[5]));
        odor = (((( long)robot.DataReceived[16]<<24)) + (((long)robot.DataReceived[15]<<16 ))+ (((long)robot.DataReceived[14]<<8 ))+ ((long)robot.DataReceived[13]));
    }

    odoG= ((((long)robot.DataReceived[8]<<24)) + (((long)robot.DataReceived[7]<<16 ))+ (((long)robot.DataReceived[6]<<8 ))+ ((long)robot.DataReceived[5])) -odol;
    odoD= ((((long)robot.DataReceived[16]<<24)) + (((long)robot.DataReceived[15]<<16 ))+ (((long)robot.DataReceived[14]<<8 ))+ ((long)robot.DataReceived[13])) -odor;

    depgauche->setText(QString::number(odoG));
    depdroite->setText( QString::number(odoD));
    //moyenne du deplacement des deux roues
    dep->setText(QString::number((odoG+odoD)));

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
   // robot.DataToSend[2] += 10;
    //robot.DataToSend[4] += 10;
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

void MainWindow::moveCamUp()
{
    manager->get(QNetworkRequest(QUrl("http://192.168.1.11:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200")));
}

void MainWindow::moveCamDown()
{
    manager->get(QNetworkRequest(QUrl("http://192.168.1.11:8080/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200")));
}

void MainWindow::moveCamLeft()
{
    manager->get(QNetworkRequest(QUrl("http://192.168.1.11:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200")));
}

void MainWindow::moveCamRight()
{
    manager->get(QNetworkRequest(QUrl("http://192.168.1.11:8080/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200")));
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
    robot.DataToSend[2] = speed->value()-(iravg/2);
    robot.DataToSend[4] = (speed->value()-(iravg/2))/2;
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
    robot.DataToSend[2] = speed->value()-(irarg/2);
    robot.DataToSend[4] = (speed->value()-(irarg/2))/2;
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
    robot.DataToSend[2] = (speed->value()-(iravd/2))/2;
    robot.DataToSend[4] = speed->value()-(iravd/2);
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
    robot.DataToSend[2] = (speed->value()-(irard/2))/2;
    robot.DataToSend[4] = speed->value()-(irard/2);

    buttonLeft->setStyleSheet("");
    buttonUp->setStyleSheet("");
    buttonRight->setStyleSheet("background-color : rgb(255,255,0);");
    buttonDown->setStyleSheet("background-color : rgb(255,255,0);");

    updateCrc();
}
void MainWindow::test()
{

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
