#include "serialportdialog.h"

SerialPortDialog::SerialPortDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Select COM Port");

    portComboBox = new QComboBox(this);
    connectButton = new QPushButton("Connect", this);

    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : ports) {
        portComboBox->addItem(port.portName());
    }

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(portComboBox);
    layout->addWidget(connectButton);

    connect(connectButton, &QPushButton::clicked, this, &SerialPortDialog::onConnectClicked);
}

QString SerialPortDialog::selectedPort() const {
    return portComboBox->currentText();
}

void SerialPortDialog::onConnectClicked() {
    accept();
}
