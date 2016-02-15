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
    bool* setFlags();

private slots:
    void on_SaveL2Button_clicked();
    void on_SaveL3Button_clicked();
    void on_SaveL4Button_clicked();

    void on_checkBox_eth_vlan_toggled(bool checked);

    void on_SendButton_clicked();

    void on_TCP_checkbox_toggled(bool checked);

    void on_UDP_checkbox_toggled(bool checked);

    void randomize(bool* flags);

    void on_checkBox_ip_create_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    eth_header *eth_h = NULL;
    eth_802Q *vlan_h = NULL;
    ip_header *ip_h = NULL;
    tcp_header *tcp_h = NULL;
    udp_header *udp_h = NULL;
    sendSocket *socket = NULL;
    unsigned threads = 0;
    int num_of_packets = 0;
    bool *flags = NULL;

};

#endif // MAINWINDOW_H
