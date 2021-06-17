#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myrobot.h"
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QKeyEvent>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:

    double calculVitesse(long odo,bool estGauche);
    Ui::MainWindow *ui;
    //boutons pour la connexion
    QPushButton *buttonConnect;
    QPushButton *buttonDisConnect;
    QPushButton *buttonTest;
    QLabel *textConnected;
    //boutons pour le déplacement
    QPushButton *buttonUp;
    QPushButton *buttonDown;
    QPushButton *buttonLeft;
    QPushButton *buttonRight;
    QPushButton *buttonStop;

    //affichage des données
    QSpinBox *speed;
    QLabel *batterie;
    QLabel *vitesse;
    //odometrie
    QLabel *depgauche;
    QLabel *depdroite;
    QLabel *dep;
    //capteur IR
    QLabel *avg;
    QLabel *arg;
    QLabel *avd;
    QLabel *ard;

    MyRobot robot;
    int keys[4];
    //stockage valeurs IR
    unsigned char iravg=0;
    unsigned char iravd=0;
    unsigned char irarg=0;
    unsigned char irard=0;
    //Stockage valeurs odométrie
     long odoG = 0;
     long odoD = 0;
     long initOdoG = 0;
     long initOdoD = 0;
     long curOdoG=0;
     long curOdoD=0;

private slots:
    void connexion();
    void deconnexion();
    void forward();
    void accelerate();
    void left();
    void leftUp();
    void leftDown();
    void right();
    void rightUp();
    void rightDown();
    void backward();
    void stop();
    void updateCrc();
    void read();
    void test();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};
short Crc16(QByteArray byteArray , int pos);
#endif // MAINWINDOW_H
