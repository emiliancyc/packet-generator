#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <sys/types.h>
#include <ifaddrs.h>
#include <thread>

/*
#include <random>
std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
std::uniform_int_distribution<int> uni(0,7); // guaranteed unbiase
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    unsigned long long int clockCount;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (clockCount));
    srand((unsigned)clockCount);

    ui->lineEdit_eth_pcp->setValidator(new QIntValidator(0, 7));
    ui->lineEdit_eth_dei->setValidator(new QIntValidator(0, 1));
    ui->lineEdit_eth_vid->setValidator(new QIntValidator(0, 4095));

    ui->lineEdit_ip_dscp->setValidator(new QIntValidator(0, 64));
    ui->lineEdit_ip_ecn->setValidator(new QIntValidator(0, 2));
    ui->lineEdit_ip_total_length->setValidator(new QIntValidator(0, 65536));
    ui->lineEdit_ip_id->setValidator(new QIntValidator(0, 65536));
    ui->lineEdit_ip_offset->setValidator(new QIntValidator(0, 8192));
    ui->lineEdit_ip_ttl->setValidator(new QIntValidator(0, 256));
    ui->lineEdit_ip_protocol->setValidator(new QIntValidator(0, 256));

/*  QRegExp rx("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    QRegExpValidator regValidator(rx, 0);
    ui->lineEdit_ip_src_ip->setValidator(&regValidator);
    ui->lineEdit_ip_dest_ip->setValidator(&regValidator);
*/

    ui->TCP_groupBox->setEnabled(true);
    ui->TCP_checkbox->setChecked(true);
    ui->UDP_groupBox->setDisabled(true);
    ui->groupBox_layer3->setDisabled(true);


    this->flags = new bool[15];
    memset(this->flags, 0, sizeof(*(this->flags)));

    //dopisać walidator dla IP

    struct ifaddrs *addrs;
    getifaddrs(&addrs);

    while (addrs) {
       if (addrs->ifa_addr && addrs->ifa_addr->sa_family == AF_PACKET) {
           ui->interface_list_comboBox->addItem((QString) addrs->ifa_name);
       }
       addrs = addrs->ifa_next;
    }
    freeifaddrs(addrs);

    this->threads = std::thread::hardware_concurrency();
    while(this->threads) {
        ui->cores_num_comboBox->addItem((QString::number(this->threads)));
        this->threads--;
    }

    if (this->eth_h == NULL) {
        this->eth_h = new eth_header();
    }
    if (this->ip_h == NULL) {
        this->ip_h = new ip_header();
    }
    if (this->tcp_h == NULL) {
        this->tcp_h = new tcp_header();
    }

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
    if((this->eth_h == NULL) && (this->vlan_h == NULL)) {
       this->eth_h = new eth_header();
    }

    if(this->ip_h == NULL) {
        this->ip_h = new ip_header();
    }

    u_char dscp = (u_char) ui->lineEdit_ip_dscp->text().toInt();
    u_char ecn = (u_char) ui->lineEdit_ip_ecn->text().toInt();
    dscp = dscp << 2;
    u_char ToS = (dscp | ecn );
    u_char flags = (u_char) ui->comboBox_ip_flags->currentIndex();

    this->ip_h->update_src_ip(this->ip_h, ui->lineEdit_ip_src_ip->text().toStdString());
    this->ip_h->update_dest_ip(this->ip_h, ui->lineEdit_ip_dest_ip->text().toStdString());
    this->ip_h->update_values(this->ip_h, ToS, (unsigned short int) (ui->lineEdit_ip_total_length->text().toInt()),
                              (unsigned short int) (ui->lineEdit_ip_id->text().toInt()),
                              flags,
                              (u_char) (ui->lineEdit_ip_offset->text().toInt()),
                              (u_char) (ui->lineEdit_ip_ttl->text().toInt()),
                              (u_char) (ui->lineEdit_ip_protocol->text().toInt()));
}

void MainWindow::on_SaveL4Button_clicked()
{
    if((this->eth_h == NULL) && (this->vlan_h == NULL)) {
       this->eth_h = new eth_header();
    }

    if(this->ip_h == NULL) {
        this->ip_h = new ip_header();
    }

    this->tcp_h = new tcp_header();
    unsigned short int src_port = (unsigned short int) ui->tcp_src_port_lineEdit->text().toInt();
    unsigned short int dest_port = (unsigned short int) ui->tcp_dest_port_lineEdit->text().toInt();
    unsigned long int seq_num = (unsigned long int) ui->tcp_seq_num_lineEdit->text().toULong();
    unsigned long int ack_num = (unsigned long int) ui->tcp_ack_num_lineEdit->text().toULong();
    u_char data_offset = (u_char) ui->tcp_data_offset_lineEdit->text().toUShort();
    u_char control_bits = 0;
    u_char ecn = 0;
    if (ui->checkBox_tcp_ecn_ns->isChecked() == true) {
        ecn = 1;
    }
    if (ui->checkBox_tcp_ecn_cwr->isChecked() == true) {
        control_bits = control_bits | 0x80;
    }
    if (ui->checkBox_tcp_ecn_ece->isChecked() == true) {
        control_bits = control_bits | 0x40;
    }
    if (ui->checkBox_tcp_control_bits_urg->isChecked() == true) {
        control_bits = control_bits | 0x20;
    }
    if (ui->checkBox_tcp_control_bits_ack->isChecked() == true) {
        control_bits = control_bits | 0x10;
    }
    if (ui->checkBox_tcp_control_bits_psh->isChecked() == true) {
        control_bits = control_bits | 0x8;
    }
    if (ui->checkBox_tcp_control_bits_rst->isChecked() == true) {
        control_bits = control_bits | 0x4;
    }
    if (ui->checkBox_tcp_control_bits_syn->isChecked() == true) {
        control_bits = control_bits | 0x2;
    }
    if (ui->checkBox_tcp_control_bits_fin->isChecked() == true) {
        control_bits = control_bits | 0x1;
    }
    unsigned short int window = (unsigned short int) ui->tcp_window_lineEdit->text().toInt();
    unsigned short int checksum = 0; //(unsigned short int) ui->tcp_checksum_lineEdit->text().toInt();
    unsigned short int urgent_pointer = (unsigned short int) ui->tcp_urgent_pointer_lineEdit->text().toInt();

    this->tcp_h->update_values(this->tcp_h, src_port, dest_port, seq_num, ack_num,
                               data_offset, ecn, control_bits, window, urgent_pointer);

}

void MainWindow::on_SendButton_clicked()
{

    std::string interface_name = ui->interface_list_comboBox->currentText().toStdString();
    this->socket = new sendSocket(interface_name.c_str(), "AA:BB:CC:DD:EE:FF");
    strncpy(this->socket->interface_index.ifr_name, interface_name.c_str(), interface_name.length());

    //TODO
    // Fix buff_begin size, should be matching packet size, not constant value
    this->socket->buff_begin = new u_char[128]; //



    //LAYER2 SECTION
    if (ui->checkBox_ip_create->isChecked() == false) {
       if (ui->checkBox_eth_vlan->isChecked() == true) {
          this->socket->buff_layer2 = new u_char[20];
          this->socket->buff_size_layer2 = 20;
          this->vlan_h->serialize_eth_802Q(this->vlan_h, this->socket->buff_layer2);
       }
       else {
          this->socket->buff_layer2 = new u_char[16];
          this->socket->buff_size_layer2 = 16;
          this->eth_h->serialize_eth(this->eth_h, this->socket->buff_layer2);
       }
    }
    else if (ui->checkBox_ip_create->isChecked() == true) {
       if (ui->checkBox_eth_vlan->isChecked() == true) {
          this->socket->buff_layer2 = new u_char[18];
          this->socket->buff_size_layer2 = 18;
          this->vlan_h->serialize_eth_802Q(this->vlan_h, this->socket->buff_layer2);
       }
       else {
          this->socket->buff_layer2 = new u_char[14];
          this->socket->buff_size_layer2 = 14;
          this->eth_h->serialize_eth(this->eth_h, this->socket->buff_layer2);
       }
    }

    //LAYER3 SECTION
    this->socket->buff_layer3 = new u_char[20];
    this->socket->buff_size_layer3 = 20;
    this->ip_h->serialize_ip(this->ip_h, this->socket->buff_layer3);

    if (ui->comboBox_ip_checksum->currentIndex() == 1) {
       this->ip_h->calculate_checksum(this->ip_h, this->socket->buff_layer3, 10);
       this->ip_h->serialize_ip(this->ip_h, this->socket->buff_layer3);
    }

    //LAYER4 SECTION
    if (ui->TCP_checkbox->isChecked() == true) {
       this->socket->buff_layer4 = new u_char[20];
       this->socket->buff_size_layer4 = 20;
       this->tcp_h->serialize_tcp(this->tcp_h, this->socket->buff_layer4);
    }
    else if (ui->UDP_checkbox->isChecked() == true) {
       this->socket->buff_layer4 = new u_char[8];
       this->socket->buff_size_layer4 = 8;
       this->udp_h->serialize_udp(this->udp_h, this->socket->buff_layer4);
    }

    //BUFFERS SECTION
    memcpy(this->socket->buff_begin,this->socket->buff_layer2, this->socket->buff_size_layer2);
    memcpy((this->socket->buff_begin + this->socket->buff_size_layer2), this->socket->buff_layer3, this->socket->buff_size_layer3);
    memcpy((this->socket->buff_begin + this->socket->buff_size_layer2 + this->socket->buff_size_layer3), this->socket->buff_layer4, this->socket->buff_size_layer4);


    //SENDING SECTION
    bool* rand_flags = this->setFlags();
    unsigned short int* temp = (unsigned short int*) (this->socket->buff_begin);

    if (ui->checkBox_eth_vlan->isChecked() == true) {
       temp += 14;
    }
    else {
       temp += 12;
    }

    this->num_of_packets = ui->packages_to_send_lineEdit->text().toInt();
    for (int i = 0; i < (this->num_of_packets); ++i) {
        this->randomize(rand_flags);
        if (ui->comboBox_ip_checksum->currentIndex() == 1) {
           (*temp) = 0;
           (*temp) = this->ip_h->calculate_checksum(this->ip_h, ((this->socket->buff_begin) + this->socket->buff_size_layer2), 10);
//           temp -= 5;
//           (*temp) = 0;
        }

        this->socket->send_packet(*(this->socket), this->socket->buff_begin, (this->socket->buff_size_layer2 + this->socket->buff_size_layer3 + this->socket->buff_size_layer4));
    }
}




void MainWindow::on_TCP_checkbox_toggled(bool checked)
{
    if(checked) {
        ui->TCP_groupBox->setEnabled(true);
        ui->UDP_groupBox->setChecked(false);
        ui->UDP_checkbox->setDisabled(true);
    }
    else {
        ui->UDP_checkbox->setEnabled(true);
        ui->TCP_groupBox->setDisabled(true);
    }

}

void MainWindow::on_UDP_checkbox_toggled(bool checked)
{
    if(checked) {
        ui->UDP_groupBox->setEnabled(true);
        ui->TCP_groupBox->setChecked(false);
        ui->TCP_checkbox->setDisabled(true);
    }
    else {
        ui->TCP_checkbox->setEnabled(true);
        ui->UDP_groupBox->setDisabled(true);
    }
}

bool* MainWindow::setFlags() {

    /* LEGEND:
    flag[0] = eth_rand_src_mac
    flag[1] = eth_rand_dest_mac
    flag[2] = eth_rand_pcp
    flag[3] = eth_rand_dei
    flag[4] = eth_rand_vid
    flag[5] = ip_rand_id
    flag[6] = ip_rand_ttl
    flag[7] = ip_rand_src_ip
    flag[8] = ip_rand_dest_ip
    flag[9] = tcp_rand_src_port
    flag[10] = tcp_rand_dest_port
    flag[11] = tcp_rand_seq_number
    flag[12] = tcp_rand_ack_num
    flag[13] = udp_rand_src_port
    flag[14] = udp_rand_dest_port
    */

    bool *flags = new bool[15];
    memset(flags, 0, 15*sizeof(flags));
    if (ui->checkBox_eth_rand_src_mac->isChecked() == true) {
        flags[0]= 1;
    }

    if (ui->checkBox_eth_rand_dest_mac->isChecked() == true) {
        flags[1]= 1;
    }

    if (ui->checkBox_eth_rand_pcp->isChecked() == true) {
        flags[2]= 1;
    }

    if (ui->checkBox_eth_rand_dei->isChecked() == true) {
        flags[3]= 1;
    }

    if (ui->checkBox_eth_rand_vid->isChecked() == true) {
        flags[4]= 1;
    }

    if (ui->checkBox_ip_rand_id->isChecked() == true) {
        flags[5]= 1;
    }

    if (ui->checkBox_ip_rand_ttl->isChecked() == true) {
        flags[6]= 1;
    }

    if (ui->checkBox_ip_rand_src_ip->isChecked() == true) {
        flags[7]= 1;
    }

    if (ui->checkBox_ip_rand_dest_ip->isChecked() == true) {
        flags[8]= 1;
    }

    if (ui->checkBox_tcp_rand_src_port->isChecked() == true) {
        flags[9]= 1;
    }

    if (ui->checkBox_tcp_rand_dest_port->isChecked() == true) {
        flags[10]= 1;
    }

    if (ui->checkBox_tcp_rand_seq_number->isChecked() == true) {
        flags[11]= 1;
    }

    if (ui->checkBox_tcp_rand_ack_num->isChecked() == true) {
        flags[12]= 1;
    }

    if (ui->checkBox_udp_rand_src_port->isChecked() == true) {
        flags[13]= 1;
    }

    if (ui->checkBox_udp_rand_dest_port->isChecked() == true) {
        flags[14]= 1;
    }

    return flags;
}

void MainWindow::randomize(bool* flags) {
    bool vlan = 0;
    if (ui->checkBox_eth_vlan->isChecked() == true)
        vlan = 1;

    if (flags[0] != 0) {
       if (vlan)
          this->vlan_h->random_mac_addr((this->socket->buff_begin), 0, 1);
       else
          this->eth_h->random_mac_addr((this->socket->buff_begin), 0, 1);

    }
    if (flags[1] != 0) {
       if (vlan)
          this->vlan_h->random_mac_addr((this->socket->buff_begin), 1, 0);
       else
          this->eth_h->random_mac_addr((this->socket->buff_begin), 1, 0);
    }
    if (flags[2] != 0) {
       if (vlan)
          this->vlan_h->rand_pcp(this->socket->buff_begin);
    }
    if (flags[3] != 0) {
       if (vlan)
          this->vlan_h->rand_dei(this->socket->buff_begin);
    }
    if (flags[4] != 0) {
       if (vlan)
          this->vlan_h->rand_vid(this->socket->buff_begin);
    }
    if (flags[5] != 0) {
       if (vlan)
          this->ip_h->rand_id(this->socket->buff_begin, true);
       else
          this->ip_h->rand_id(this->socket->buff_begin, false);
    }
    if (flags[6] != 0) {
       if (vlan)
          this->ip_h->rand_ttl(this->socket->buff_begin, 1);
       else
          this->ip_h->rand_ttl(this->socket->buff_begin, 0);
    }
    if (flags[7] != 0) {
       if (vlan)
          this->ip_h->rand_ip(this->socket->buff_begin, 1, 1, 0);
       else
          this->ip_h->rand_ip(this->socket->buff_begin, 0, 1, 0);
    }
    if (flags[8] != 0) {
       if (vlan)
          this->ip_h->rand_ip(this->socket->buff_begin, 1, 0, 1);
       else
          this->ip_h->rand_ip(this->socket->buff_begin, 0, 0, 1);
    }

}

void MainWindow::on_checkBox_ip_create_toggled(bool checked)
{
    if (checked) {
       ui->groupBox_layer3->setEnabled(true);
    }
    else {
       ui->groupBox_layer3->setDisabled(true);
    }

}

void MainWindow::on_checkBox_eth_vlan_toggled(bool checked)
{
    if(checked)
        ui->lineEdit_eth_frame_type->setText("0x8100");
    else
        ui->lineEdit_eth_frame_type->setText("0x8000");
}
