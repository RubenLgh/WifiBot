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
    Ui::MainWindow *ui;
    QPushButton *buttonConnect;
    QPushButton *buttonDisConnect;
    QPushButton *buttonTest;
    QPushButton *buttonUp;
    QPushButton *buttonDown;
    QPushButton *buttonLeft;
    QPushButton *buttonRight;
    QPushButton *buttonStop;
    QSpinBox *speed;
    QLabel *textConnected;
    QLabel *batterie;
    QLabel *depgauche;
    QLabel *depdroite;
    QLabel *dep;
    QLabel *avg;
    QLabel *arg;
    QLabel *avd;
    QLabel *ard;
    MyRobot robot;
    int keys[4];

    unsigned char iravg=0;
    unsigned char iravd=0;
    unsigned char irarg=0;
    unsigned char irard=0;

     long odoG = 0;
     long odoD = 0;
     long odol=0;
     long odor=0;

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
