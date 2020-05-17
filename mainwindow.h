#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <tins/tins.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // void getRoutingTable();

    // void getArpTable();

    // void ScanPorts();

    // void getMacByIp();

    // void getIpByMac();

    // void getHostname();

    // void getOS();

    // void getSpeedtest();

    // void getWhoIs();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
