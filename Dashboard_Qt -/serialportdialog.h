#ifndef SERIALPORTDIALOG_H
#define SERIALPORTDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtSerialPort/QSerialPortInfo>

class SerialPortDialog : public QDialog {
    Q_OBJECT

public:
    explicit SerialPortDialog(QWidget *parent = nullptr);
    QString selectedPort() const;

private slots:
    void onConnectClicked();

private:
    QComboBox *portComboBox;
    QPushButton *connectButton;
};

#endif // SERIALPORTDIALOG_H
