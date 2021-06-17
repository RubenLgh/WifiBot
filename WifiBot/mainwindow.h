#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myrobot.h"
#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QKeyEvent>
#include <QLabel>
#include <QUrl>
#include <QWebEngineView>
#include <QNetworkAccessManager>

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
    Ui::MainWindow *ui;
    double calculVitesse(long odo,bool estGauche);
    QPushButton *buttonConnect; //Bouton pour se conencter
    QPushButton *buttonDisConnect; //Bouton pour se déconnecter
    QPushButton *buttonTest;
    QPushButton *buttonUp;  //Déplacement vers le haut
    QPushButton *buttonDown; //Déplacement vers le bas
    QPushButton *buttonLeft; //Déplacement vers la gauche
    QPushButton *buttonRight; //Déplacement vers la droite
    QPushButton *buttonStop;  //Bouton pour arréter le robot
    QPushButton *camUp; //Déplacer la caméra vers le haut
    QPushButton *camDown; //Déplacer la caméra vers le bas
    QPushButton *camLeft ;//Déplacer la caméra vers la gauche
    QPushButton *camRight; //Déplacer la caméra vers la droite
    QSpinBox *speed;    //Pour mettre une vitesse au robot
    QLabel *textConnected; //Affichage à l'écran de l'état de la connexion
    QLabel *batterie;   //Affichage de la batterie
    QLabel *vitesse;
    QLabel *depgauche;
    QLabel *depdroite;
    QLabel *dep;
    QLabel *avg;
    QLabel *arg;
    QLabel *avd;
    QLabel *ard;
    QWebEngineView *camera; //Affichage de la caméra
    QNetworkAccessManager *manager;//permet d'envoyer les requêtes pour bouger la caméra
    MyRobot robot; // permet de gérer le robot
    int keys[4]; //Pour lire plusieurs touches


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
    void connexion(); //Lance la connexion au robot
    void deconnexion(); //Se déconnecte du robot
    void forward(); //Fait avancer
    void accelerate(); //(Non utilisé pour le moment) Fait avancer de plus en plus vite
    void left(); // tourne à gauche à l'arrêt
    void leftUp(); // tourne à gauche en avançant
    void leftDown(); //tourne à gauche en reculant
    void right(); // tourne à droite à l'arrêt
    void rightUp();//tourne à droite en avançant
    void rightDown(); //tourne à droite en reculant
    void backward();    //recule
    void moveCamUp();//Déplace la caméra vers le haut
    void moveCamDown(); //Déplace la caméra vers le bas
    void moveCamLeft();//Déplace la caméra vers la gauche
    void moveCamRight();//Déplace la caméra vers la droite
    void stop(); //Arrête le robot
    void updateCrc();//Met à jour le crc
    void read();
    void test();

protected:
    void keyPressEvent(QKeyEvent *event);   //Permet de lire les touches du clavier
    void keyReleaseEvent(QKeyEvent *event);
};
short Crc16(QByteArray byteArray , int pos);
#endif // MAINWINDOW_H
