#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "myrobot.h"
#include <QMainWindow>
#include <QPushButton>
#include <QKeyEvent>


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
    QPushButton *buttonRead;
    MyRobot robot;

private slots:
    void connexion();
    void deconnexion();
    void forward();
    void accelerate();
    void left();
    void right();
    void backward();
    void stop();
    void updateCrc();
    void test();
    void read();

protected:
    void keyPressEvent(QKeyEvent *event);
};
short Crc16(QByteArray byteArray , int pos);
#endif // MAINWINDOW_H
