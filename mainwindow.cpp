#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>

using namespace Tins;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget* widget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    for (const NetworkInterface& iface : NetworkInterface::all()){
        QWidget* ifaceWidget = createNetworkCardWidget(iface, widget);
        layout->addWidget(ifaceWidget);
        ifaceWidget->show();
    };
    widget->setLayout(layout);
    widget->show();

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(widget);

    this->setCentralWidget(scrollArea);
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
