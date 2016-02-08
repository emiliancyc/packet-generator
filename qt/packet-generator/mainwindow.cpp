#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <sys/types.h>
#include <ifaddrs.h>
#include <thread>

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
  //  ui->lineEdit_ip_flags->setValidator(new QIntValidator(0, 2));
    ui->lineEdit_ip_offset->setValidator(new QIntValidator(0, 8192));
    ui->lineEdit_ip_ttl->setValidator(new QIntValidator(0, 256));
    ui->lineEdit_ip_protocol->setValidator(new QIntValidator(0, 256));

    /*QRegExp rx("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    QRegExpValidator regValidator(rx, 0);
    ui->lineEdit_ip_src_ip->setValidator(&regValidator);
    ui->lineEdit_ip_dest_ip->setValidator(&regValidator);
*/

    ui->TCP_groupBox->setDisabled(true);
    ui->UDP_groupBox->setDisabled(true);

    srand( time( NULL ) );
    this->flags = new bool[15];
    memset(this->flags, 0, sizeof(this->flags));

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
    u_char flags = (u_char) ui->comboBox_ip_flags->currentIndex();

    this->ip_h->update_src_ip(this->ip_h, ui->lineEdit_ip_src_ip->text().toStdString());
    this->ip_h->update_dest_ip(this->ip_h, ui->lineEdit_ip_dest_ip->text().toStdString());
    this->ip_h->update_values(this->ip_h, ToS, (unsigned short int) (ui->lineEdit_ip_total_length->text().toInt()),
                              (unsigned short int) (ui->lineEdit_ip_id->text().toInt()),
                              flags,
                              (u_char) (ui->lineEdit_ip_offset->text().toInt()),
                              (u_char) (ui->lineEdit_ip_ttl->text().toInt()),
                              (u_char) (ui->lineEdit_ip_protocol->text().toInt()));

    //this->ip_h->checksum = checksum(u_char* buff, 10);

//sprawdzic czy flagi będą poprawne w TCPDump!

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

    std::string interface_name = ui->interface_list_comboBox->currentText().toStdString();
    this->socket = new sendSocket(interface_name.c_str(), "AA:BB:CC:DD:EE:FF");
    strncpy(this->socket->interface_index.ifr_name, interface_name.c_str(), interface_name.length());
    this->socket->buff_begin = new u_char[128];

    //LAYER2 SECTION
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

    //LAYER3 SECTION
    this->socket->buff_layer3 = new u_char[20];
    this->socket->buff_size_layer3 = 20;
    this->ip_h->serialize_ip(this->ip_h, this->socket->buff_layer3);
    this->ip_h->calculate_checksum(this->ip_h, this->socket->buff_layer3, 10);
    this->ip_h->serialize_ip(this->ip_h, this->socket->buff_layer3);



    //LAYER4 SECTION
    //TODO

    //BUFFERS SECTION
    memcpy(this->socket->buff_begin,this->socket->buff_layer2, this->socket->buff_size_layer2);
    memcpy((this->socket->buff_begin + this->socket->buff_size_layer2), this->socket->buff_layer3, this->socket->buff_size_layer3);


    //SENDING SECTION
    bool* rand_flags = this->setFlags();
    this->num_of_packets = ui->packages_to_send_lineEdit->text().toInt();
    for (int i = 0; i < (this->num_of_packets); ++i) {
        this->randomize(rand_flags);
        //przeliczenie checksumy po randomizacji dla L2/L4

        this->socket->send_packet(*(this->socket), this->socket->buff_begin, (this->socket->buff_size_layer2 + this->socket->buff_size_layer3));
    }
}

void MainWindow::on_checkBox_eth_rand_src_mac_toggled(bool checked)
{
    //if (checked)
  //      this->eth_h->random_mac_addr((this->socket->buff_begin), 1, 0);

}

void MainWindow::on_checkBox_eth_rand_dest_mac_toggled(bool checked)
{
  //  if (checked)
 //       this->eth_h->random_mac_addr((this->socket->buff_begin), 0, 1);
}
void MainWindow::on_checkBox_ip_rand_id_toggled(bool checked)
{

}

void MainWindow::on_checkBox_ip_rand_ttl_toggled(bool checked)
{

}

void MainWindow::on_checkBox_ip_rand_src_ip_toggled(bool checked)
{

}

void MainWindow::on_checkBox_ip_rand_dest_ip_toggled(bool checked)
{

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


void MainWindow::on_SaveL4Button_clicked()
{

}


bool* MainWindow::setFlags() {

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

    if (flags[0] != 0) {
       if (ui->checkBox_eth_vlan->isChecked() == true)
          this->vlan_h->random_mac_addr((this->socket->buff_begin), 0, 1);
       else
          this->eth_h->random_mac_addr((this->socket->buff_begin), 0, 1);

    }
    if (flags[1] != 0) {
        if (ui->checkBox_eth_vlan->isChecked() == true)
           this->vlan_h->random_mac_addr((this->socket->buff_begin), 1, 0);
        else
           this->eth_h->random_mac_addr((this->socket->buff_begin), 1, 0);
    }
    if (flags[2] != 0) {
       this->vlan_h->rand_pcp(this->socket->buff_begin);
    }
    if (flags[3] != 0) {
       this->vlan_h->rand_dei(this->socket->buff_begin);
    }
    if (flags[4] != 0) {
       this->vlan_h->rand_vid(this->socket->buff_begin);
    }

    /*for (int i = 0; i < 15; ++i) {
        if()
        switch(flags[i]) {
        case flags[0]:
            this->eth_h->random_mac_addr((this->socket->buff_begin), 0, 1);
            break;
         case flags[1]:
            this->eth_h->random_mac_addr((this->socket->buff_begin), 1, 0);
            break;
        case flags[2]:
            (//pcp
            break;
        case flags[3]:
            //dei
            break;
        case flags[4]:
            //vid
            break;
        case flags[5]:
            //ip ID
            break;
        case flags[6]:
            //ip ttl
            break;
        case flags[7]:
            //ip src ip
            break;
        case flags[8]:
            //ip dest ip
            break;
        case flags[9]:
            break;
        case flags[10]:
            break;
        case flags[11]:
            break;
        case flags[12]:
            break;
        case flags[13]:
            break;
        case flags[14]:
            break;

    }
*/
}
