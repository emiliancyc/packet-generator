#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->eth_h = new eth_header();

    //TODO
    // Otwieranie socketu


    //TODO
    //Stworzenie obiektów

    //TODO
    //Serializacja obiektów

    //TODO
    //Wypchniecie obiektów na socket



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_SaveL2Button_clicked()
{
    if (ui->checkBox_eth_vlan->isChecked()) {
        this->vlan_h = new eth_802Q();
        const char *src_mac = ui->lineEdit_eth_src_mac->text().toStdString().c_str();
        const char *dest_mac = ui->lineEdit_eth_src_mac->text().toStdString().c_str();

        this->vlan_h->update_src_mac(this->vlan_h, src_mac);
        this->vlan_h->update_dest_mac(this->vlan_h, dest_mac);

        int PCPbin = (ui->lineEdit_eth_pcp->text().toInt()) << 13;
        int DEIbin = (ui->lineEdit_eth_dei->text().toInt()) << 12;
        int VID = (ui->lineEdit_eth_vid->text().toInt());

        int TCIbin = PCPbin | DEIbin | VID;
        this->vlan_h->update_tci(this->vlan_h, TCIbin);
    }
    else {
    this->eth_h = new eth_header();
    const char *src_mac = ui->lineEdit_eth_src_mac->text().toStdString().c_str();
    const char *dest_mac = ui->lineEdit_eth_src_mac->text().toStdString().c_str();
    this->eth_h->update_src_mac(this->eth_h, src_mac);
    this->eth_h->update_dest_mac(this->eth_h, dest_mac);
    }
}
