#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "includes.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_3_clicked();

    void on_SaveL2Button_clicked();

private:
    Ui::MainWindow *ui;
    eth_header *eth_h = NULL;
    eth_802Q *vlan_h = NULL;
    ip_header *ip_h = NULL;
    tcp_header *tcp_h = NULL;
    udp_header *upd_h = NULL;
};

#endif // MAINWINDOW_H
