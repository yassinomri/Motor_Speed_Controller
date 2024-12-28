#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QComboBox>
#include <QKeyEvent>
#include <QPaintEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void updatePhysics();

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;

    double currentSpeed;
    double acceleration;
    double deceleration;
    double brakeForce;
    double emergencyBrakeForce;

    bool accelerating;
    bool braking;
    bool emergencyBrake;

    double fuelLevel;
    double distanceTraveled;

    void openSerialPort(const QString &portName);
    void updateMotorSpeed(double speed);

    void drawSpeedometerBackground(QPainter *painter);
    void drawSpeedometerNeedle(QPainter *painter);
    void drawSpeedLabels(QPainter *painter);
    void drawPedalStatus(QPainter *painter);
    void drawAdditionalDashboardItems(QPainter *painter);
};

#endif // MAINWINDOW_H
