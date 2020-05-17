#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace Tins;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    for (const NetworkInterface& iface : NetworkInterface::all()){
        if(iface.info().is_up && iface.info().hw_addr.to_string() != "00:00:00:00:00:00" && iface.info().ip_addr.to_string() != "0.0.0.0"){
            ui->mac->setText(QString::fromStdString(iface.info().hw_addr.to_string()));
            ui->ipv4->setText(QString::fromStdString(iface.info().ip_addr.to_string()));
            ui->netmask->setText(QString::fromStdString(iface.info().netmask.to_string()));
            ui->broadcast->setText(QString::fromStdString(iface.info().bcast_addr.to_string()));
            ui->status->setText(QString::fromStdString(std::to_string(iface.info().is_up)));
        }
    }

}
