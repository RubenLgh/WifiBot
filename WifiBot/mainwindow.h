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
    QPushButton *buttonConnect;
    QPushButton *buttonDisConnect;
    QPushButton *buttonTest;
    QPushButton *buttonUp;
    QPushButton *buttonDown;
    QPushButton *buttonLeft;
    QPushButton *buttonRight;
    QPushButton *buttonStop;
    QPushButton *camUp;
    QPushButton *camDown;
    QPushButton *camLeft;
    QPushButton *camRight;
    QSpinBox *speed;
    QLabel *textConnected;
    QLabel *batterie;
    QWebEngineView *camera;
    QWebEngineView *controlCamera;
    QNetworkAccessManager *manager;
    MyRobot robot;
    int keys[4];

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
    void moveCamUp();
    void moveCamDown();
    void moveCamLeft();
    void moveCamRight();
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
