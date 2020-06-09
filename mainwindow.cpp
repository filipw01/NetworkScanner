#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Host.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QTextEdit>
#include <QPushButton>
#include <QCheckBox>

using namespace Tins;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget* localhostWidget = new QWidget;
    QVBoxLayout* localhostLayout = new QVBoxLayout;
    for (const NetworkInterface& iface : NetworkInterface::all()){
        QWidget* ifaceWidget = createNetworkCardWidget(iface, localhostWidget);
        localhostLayout->addWidget(ifaceWidget);
        ifaceWidget->show();
    };
    localhostWidget->setLayout(localhostLayout);
    localhostWidget->show();
    QScrollArea* localhostScrollArea = new QScrollArea;
    localhostScrollArea->setWidget(localhostWidget);

    networkScanWidget = new QWidget;
    networkScanLayout = new QVBoxLayout;

    field = new QTextEdit;
    field->setFixedHeight(26);
    QPushButton* button = new QPushButton;
    button->setText("Przeskanuj hosta");
    connect(button, SIGNAL (clicked()),this, SLOT (handleClick()));

    checkboxOs = new QCheckBox;
    checkboxMac = new QCheckBox;
    checkboxActive = new QCheckBox;
    checkboxPorts= new QCheckBox;
    checkboxOs->setChecked(true);
    checkboxMac->setChecked(true);
    checkboxActive->setChecked(true);
    checkboxPorts->setChecked(true);
    checkboxOs->setText("Pokaż system operacyjny");
    checkboxMac->setText("Pokaż adres MAC");
    checkboxActive->setText("Pokaż czy aktywny w sieci");
    checkboxPorts->setText("Pokaż otwarte porty");

    networkScanLayout->addWidget(field);
    networkScanLayout->addWidget(button);
    networkScanLayout->addWidget(checkboxOs);
    networkScanLayout->addWidget(checkboxMac);
    networkScanLayout->addWidget(checkboxActive);
    networkScanLayout->addWidget(checkboxPorts);
    networkScanWidget->setLayout(networkScanLayout);
    networkScanWidget->setFixedHeight(500);
    networkScanWidget->show();
    networkScanScrollArea = new QScrollArea;
    networkScanScrollArea->setWidget(networkScanWidget);
    tabWidget = new QTabWidget(this);
    tabWidget->addTab(localhostScrollArea, "Lokalne interfejsy sieciowe");
    tabWidget->addTab(networkScanScrollArea, "Skan hosta");
    tabWidget->show();

    this->setCentralWidget(tabWidget);
}


MainWindow::~MainWindow()
{
    delete ui;
}

QWidget* MainWindow::createNetworkCardWidget(NetworkInterface iface, QWidget* parent){
    QWidget* widget = new QWidget(parent);

    QLabel* name = new QLabel;
    name->setText(QString::fromStdString("Nazwa: " + iface.name()));

    QLabel* mac = new QLabel;
    mac->setText(QString::fromStdString("Adres MAC: " + iface.info().hw_addr.to_string()));

    QLabel* ipv4 = new QLabel;
    ipv4->setText(QString::fromStdString("Adres IP: " + iface.info().ip_addr.to_string()));

    QLabel* netmask = new QLabel;
    netmask->setText(QString::fromStdString("Maska: " + iface.info().netmask.to_string()));

    QLabel* broadcast = new QLabel;
    broadcast->setText(QString::fromStdString("Adres rozgłoszeniowy: " + iface.info().bcast_addr.to_string()));

    QLabel* status = new QLabel;
    std::string statusText = iface.info().is_up ? "Włączona" : "Wyłączona";
    status->setText(QString::fromStdString("Stan karty: " + statusText));

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(name);
    layout->addWidget(mac);
    layout->addWidget(ipv4);
    layout->addWidget(netmask);
    layout->addWidget(broadcast);
    layout->addWidget(status);

    widget->setLayout(layout);
    return widget;
}

void MainWindow::handleClick(){
    std::string fieldValue = field->toPlainText().toStdString();
    QLabel* label = new QLabel;
    try {
        Host* host = new Host(IPv4Address(fieldValue));
        std::string labelText = "Adres hosta: ";
        labelText += host->getIp().to_string();
        if(checkboxOs->isChecked()){
            labelText += "\nSystem operacyjny: ";
            host->check_availability();
            labelText += host->getOs();
        }
        if(checkboxMac->isChecked()){
            labelText += "\nAdres MAC: ";
            host->check_mac();
            labelText += host->getMac().to_string();
        }
        if(checkboxActive->isChecked()){
            labelText += "\nAktywny w sieci: ";
            host->check_availability();
            if(host->getActive()){
                labelText += "tak";
            }else{
                labelText += "nie";
            }
        }
        if(checkboxPorts->isChecked()){
            host->scan_ports({});
            std::vector<int> ports = host->getPorts();
            labelText += "\nOtwarte porty:";
            for (const int port : ports) {
                labelText+=std::to_string(port);
                labelText+=", ";
            }
        }
        label->setText(QString::fromStdString(labelText));
        networkScanLayout->addWidget(label);
        networkScanWidget->setLayout(networkScanLayout);
        networkScanWidget->show();
    } catch (Tins::invalid_address) {
        field->setText("Niepoprawny adres");
    }
}
