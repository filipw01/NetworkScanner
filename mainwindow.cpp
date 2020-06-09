#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Host.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QTextEdit>
#include <QPushButton>

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
    button->setText("Przeskanuj otwarte porty");
            connect(button, SIGNAL (clicked()),this, SLOT (handleClick()));
    networkScanLayout->addWidget(field);
    networkScanLayout->addWidget(button);
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
        host->scan_ports({});
        std::vector<int> ports = host->getPorts();
        std::string labelText = "Adres hosta: ";
        labelText += host->getIp().to_string();
        labelText += "\nSystem operacyjny: ";
        labelText += host->getOs();
        labelText += "\nAdres MAC: ";
        labelText += host->getMac().to_string();
        labelText += "\nAktywny w sieci: ";
        if(host->getActive()){
            labelText += "tak";
        }else{
            labelText += "nie";
        }
        labelText += "\nOtwarte porty:";
        for (const int port : ports) {
            labelText+=std::to_string(port);
            labelText+=", ";
        }
        label->setText(QString::fromStdString(labelText));
        networkScanLayout->addWidget(label);
        networkScanWidget->setLayout(networkScanLayout);
        networkScanWidget->show();
    } catch (Tins::invalid_address) {
        field->setText("Niepoprawny adres");
    }
}
