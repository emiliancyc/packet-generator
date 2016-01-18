#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit_eth_pcp->setValidator(new QIntValidator(0, 7));
    ui->lineEdit_eth_dei->setValidator(new QIntValidator(0, 1));
    ui->lineEdit_eth_vid->setValidator(new QIntValidator(0, 4096));

    ui->lineEdit_ip_dscp->setValidator(new QIntValidator(0, 64));
    ui->lineEdit_ip_ecn->setValidator(new QIntValidator(0, 2));
    ui->lineEdit_ip_total_length->setValidator(new QIntValidator(0, 65536));
    ui->lineEdit_ip_id->setValidator(new QIntValidator(0, 65536));
    ui->lineEdit_ip_flags->setValidator(new QIntValidator(0, 2));
    ui->lineEdit_ip_offset->setValidator(new QIntValidator(0, 8192));
    ui->lineEdit_ip_ttl->setValidator(new QIntValidator(0, 256));
    ui->lineEdit_ip_protocol->setValidator(new QIntValidator(0, 256));

    QRegExp rx("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    QRegExpValidator regValidator(rx, 0);
    ui->lineEdit_ip_src_ip->setValidator(&regValidator);
    ui->lineEdit_ip_dest_ip->setValidator(&regValidator);

    //dopisać walidator dla IP
    //ui->lineEdit_ip_dscp->setStyleSheet("QToolTip {font-size:8pt; color:black; min-width: 10px;}");
    this->eth_h = new eth_header();
    this->ip_h = new ip_header();
    this->socket = new sendSocket();
    this->socket->buff = new u_char[34]; //size of ethernet frame + IP header

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
        ui->lineEdit_eth_frame_type->setText("0x8100");
        this->vlan_h = new eth_802Q();
        std::string src_mac = ui->lineEdit_eth_src_mac->text().toStdString();
        std::string dest_mac = ui->lineEdit_eth_dest_mac->text().toStdString();

        this->vlan_h->update_src_mac(this->vlan_h, src_mac);
        this->vlan_h->update_dest_mac(this->vlan_h, dest_mac);

        int PCPbin = (ui->lineEdit_eth_pcp->text().toInt()) << 13;
        int DEIbin = (ui->lineEdit_eth_dei->text().toInt()) << 12;
        int VID = (ui->lineEdit_eth_vid->text().toInt());

        int TCIbin = PCPbin | DEIbin | VID;
        this->vlan_h->update_tci(this->vlan_h, TCIbin);
    }
    else {
    ui->lineEdit_eth_frame_type->setText("0x8000");
    this->eth_h = new eth_header();
    std::string src_mac = ui->lineEdit_eth_src_mac->text().toStdString();
    std::string dest_mac = ui->lineEdit_eth_dest_mac->text().toStdString();
    this->eth_h->update_src_mac(this->eth_h, src_mac);
    this->eth_h->update_dest_mac(this->eth_h, dest_mac);
    }
}

void MainWindow::on_SaveL3Button_clicked()
{
    if(this->eth_h == NULL) {
       this->eth_h = new eth_header();
    }

    if(this->ip_h == NULL) {
        this->ip_h = new ip_header();
    }

    u_char dscp = (u_char) ui->lineEdit_ip_dscp->text().toInt();
    u_char ecn = (u_char) ui->lineEdit_ip_ecn->text().toInt();
    dscp = dscp << 2;
    u_char ToS = (dscp | ecn );

    this->ip_h->update_src_ip(this->ip_h, ui->lineEdit_ip_src_ip->text().toStdString());
    this->ip_h->update_dest_ip(this->ip_h, ui->lineEdit_ip_dest_ip->text().toStdString());
    this->ip_h->update_values(this->ip_h, ToS, (unsigned short int) (ui->lineEdit_ip_total_length->text().toInt()),
                              (unsigned short int) (ui->lineEdit_ip_id->text().toInt()),
                              (u_char) (ui->lineEdit_ip_flags->text().toInt()),
                              (u_char) (ui->lineEdit_ip_offset->text().toInt()),
                              (u_char) (ui->lineEdit_ip_ttl->text().toInt()),
                              (u_char) (ui->lineEdit_ip_protocol->text().toInt()));

    //this->ip_h->checksum = checksum(u_char* buff, 10);

//sprawdzic czy flagi będą poprawne w TCPDump!

    //this->QMainWindow::close();
}

void MainWindow::on_checkBox_eth_vlan_toggled(bool checked)
{
    if(checked)
        ui->lineEdit_eth_frame_type->setText("0x8100");
    else
        ui->lineEdit_eth_frame_type->setText("0x8000");
}

void MainWindow::on_SendButton_clicked()
{


}
