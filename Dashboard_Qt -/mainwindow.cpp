#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialportdialog.h"
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDial>
#include <QTimer>
#include <QPainter>
#include <QtMath>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(800, 600);

    currentSpeed = 0.0;
    acceleration = 0.5;
    deceleration = 0.1;
    brakeForce = 1.0;
    emergencyBrakeForce = 2.0;
    accelerating = false;
    braking = false;
    emergencyBrake = false;
    fuelLevel = 100.0;
    distanceTraveled = 0.0;

    setFocusPolicy(Qt::StrongFocus);
    grabKeyboard();
    this->setFocus();

    SerialPortDialog portDialog(this);
    if (portDialog.exec() == QDialog::Accepted) {
        const QString selectedPort = portDialog.selectedPort();
        openSerialPort(selectedPort);
    } else {
        qDebug() << "No COM port selected. Exiting application.";
        QApplication::quit();
        return;
    }

    // Timer setup
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updatePhysics);
    timer->start(16);
    qDebug() << "Timer started";
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), Qt::gray);

    drawSpeedometerBackground(&painter);

    drawSpeedometerNeedle(&painter);

    drawSpeedLabels(&painter);

    drawPedalStatus(&painter);

    drawAdditionalDashboardItems(&painter);
}

void MainWindow::drawSpeedometerBackground(QPainter *painter)
{
    int centerX = width() / 2;
    int centerY = height() / 2;
    int radius = qMin(width(), height()) / 3;

    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::white);
    painter->drawEllipse(centerX - radius, centerY - radius, radius * 2, radius * 2);

    QRectF rect(centerX - radius, centerY - radius, radius * 2, radius * 2);
    painter->setPen(QPen(Qt::black, 15));
    painter->drawArc(rect, 135 * 16, 270 * 16);
}

void MainWindow::drawSpeedometerNeedle(QPainter *painter)
{
    int centerX = width() / 2;
    int centerY = height() / 2;
    int radius = qMin(width(), height()) / 3;

    double angle = -135 + (currentSpeed * 270.0 / 240.0);
    double radians = qDegreesToRadians(angle);
    int needleX = centerX + radius * qCos(radians);
    int needleY = centerY + radius * qSin(radians);

    painter->setPen(QPen(Qt::red, 2));
    painter->drawLine(centerX, centerY, needleX, needleY);

    QRect speedRect(centerX - 50, centerY + 70, 100, 30);
    painter->drawText(speedRect, Qt::AlignCenter,
                      QString::number(qRound(currentSpeed)) + " km/h");
}

void MainWindow::drawSpeedLabels(QPainter *painter)
{
    int centerX = width() / 2;
    int centerY = height() / 2;
    int radius = qMin(width(), height()) / 3;

    painter->setPen(Qt::black);

    for (int i = 0; i <= 240; i += 20) {
        double angle = -135 + (i * 270.0 / 240.0);
        double radians = qDegreesToRadians(angle);
        int labelX = centerX + (radius - 30) * qCos(radians);
        int labelY = centerY + (radius - 30) * qSin(radians);
        painter->drawText(labelX - 10, labelY + 5, QString::number(i));
    }
}

void MainWindow::drawPedalStatus(QPainter *painter)
{
    int y = height() - 100;

    painter->setPen(Qt::green);
    painter->setBrush(accelerating ? Qt::green : Qt::white);
    painter->drawRect(50, y, 80, 30);
    painter->drawText(QRect(50, y, 80, 30), Qt::AlignCenter, "Accel (W)");

    painter->setPen(Qt::red);
    painter->setBrush(braking ? Qt::red : Qt::white);
    painter->drawRect(150, y, 80, 30);
    painter->drawText(QRect(150, y, 80, 30), Qt::AlignCenter, "Brake (S)");

    painter->setPen(Qt::darkRed);
    painter->setBrush(emergencyBrake ? Qt::darkRed : Qt::white);
    painter->drawRect(250, y, 80, 30);
    painter->drawText(QRect(250, y, 80, 30), Qt::AlignCenter, "E-Brake (Space)");
}

void MainWindow::drawAdditionalDashboardItems(QPainter *painter)
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::yellow);
    painter->drawRect(650, 50, 100, 20);
    painter->drawText(QRect(650, 50, 100, 20), Qt::AlignCenter, QString("Fuel: %1%").arg(fuelLevel, 0, 'f', 1));

    painter->setPen(Qt::black);
    painter->drawText(650, 100, QString("Distance: %1 m").arg(distanceTraveled, 0, 'f', 1));
}

void MainWindow::updatePhysics()
{
    qDebug() << "Timer tick - Speed:" << currentSpeed;
    qDebug() << "Accelerating:" << accelerating << "Braking:" << braking << "Emergency Brake:" << emergencyBrake;

    double previousSpeed = currentSpeed;

    if (emergencyBrake) {
        currentSpeed = qMax(0.0, currentSpeed - emergencyBrakeForce);
    } else if (accelerating) {
        currentSpeed = qMin(240.0, currentSpeed + acceleration);
    } else if (braking) {
        currentSpeed = qMax(0.0, currentSpeed - brakeForce);
    } else {
        currentSpeed = qMax(0.0, currentSpeed - deceleration);
    }

    double avgSpeed = (previousSpeed + currentSpeed) / 2.0;
    distanceTraveled += avgSpeed * (10.0 / 3600.0);

    fuelLevel = qMax(0.0, fuelLevel - (currentSpeed / 20000.0));

    updateMotorSpeed(currentSpeed * 10 / 240);

    qDebug() << "Updated Speed:" << currentSpeed;
    update();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Key pressed:" << event->key();
    if (!event->isAutoRepeat()) {
        switch (event->key()) {
        case Qt::Key_W:
            accelerating = true;
            qDebug() << "Accelerating started";
            break;
        case Qt::Key_S:
            braking = true;
            qDebug() << "Braking started";
            break;
        case Qt::Key_Space:
            emergencyBrake = true;
            qDebug() << "Emergency brake started";
            break;
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        switch (event->key()) {
        case Qt::Key_W:
            accelerating = false;
            qDebug() << "Accelerating stopped";
            break;
        case Qt::Key_S:
            braking = false;
            qDebug() << "Braking stopped";
            break;
        case Qt::Key_Space:
            emergencyBrake = false;
            qDebug() << "Emergency brake stopped";
            break;
        }
    }
}

void MainWindow::updateMotorSpeed(double speedPercentage)
{
    if (serialPort->isOpen()) {
        QByteArray data;
        data.append(QString::number(speedPercentage, 'f', 2).toUtf8());
        serialPort->write(data);
    }
}

void MainWindow::openSerialPort(const QString &portName) {
    serialPort = new QSerialPort(this);

    serialPort->setPortName(portName);
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::WriteOnly)) {
        qDebug() << "Connected to" << serialPort->portName();
    } else {
        qDebug() << "Failed to open port!";
        delete serialPort;
        serialPort = nullptr;
        QApplication::quit();
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
