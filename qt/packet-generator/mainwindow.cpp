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

    //fix for rand working properly in loops
    unsigned long long int clockCount;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (clockCount));
    srand((unsigned) clockCount);

    // VALIDATORS
    valid0to7 = new QIntValidator(0, 7);
    valid0and1 = new QIntValidator(0, 1);
    valid0to4095 = new QIntValidator(0, 4095);
    valid0to63 = new QIntValidator(0, 63);
    valid0to2 = new QIntValidator(0, 2);
    valid0to65535 = new QIntValidator(0, 65535);
    valid0to8191 = new QIntValidator(0, 8191);
    valid0to255 = new QIntValidator(0, 255);
    valid0to15 = new QIntValidator(0, 15);
    valid_double = new QDoubleValidator(0, 4294967295, 0, this);

    ui->lineEdit_eth_pcp->setValidator(valid0to7);
    ui->lineEdit_eth_dei->setValidator(valid0and1);
    ui->lineEdit_eth_vid->setValidator(valid0to4095);

    ui->lineEdit_ip_dscp->setValidator(valid0to63);
    ui->lineEdit_ip_ecn->setValidator(valid0to2);
    ui->lineEdit_ip_total_length->setValidator(valid0to65535);
    ui->lineEdit_ip_id->setValidator(valid0to65535);
    ui->lineEdit_ip_offset->setValidator(valid0to8191);
    ui->lineEdit_ip_ttl->setValidator(valid0to255);
    ui->lineEdit_ip_protocol->setValidator(valid0to255);


    ui->lineEdit_tcp_src_port->setValidator(valid0to65535);
    ui->lineEdit_tcp_dest_port->setValidator(valid0to65535);
    ui->lineEdit_tcp_ack_num->setValidator(valid_double);
    ui->lineEdit_tcp_seq_num->setValidator(valid_double);
    ui->lineEdit_tcp_data_offset->setValidator(valid0to15);
    ui->lineEdit_tcp_window->setValidator(valid0to65535);
    ui->lineEdit_tcp_urgent_pointer->setValidator(valid0to65535);


    //TODO VALIDATORS!
    //dopisać walidator dla IP
/*  QRegExp rx("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    QRegExpValidator regValidator(rx, 0);
    ui->lineEdit_ip_src_ip->setValidator(&regValidator);
    ui->lineEdit_ip_dest_ip->setValidator(&regValidator);
*/

    // UI SETTINGS
    ui->TCP_groupBox->setDisabled(true);
    ui->UDP_groupBox->setDisabled(true);

    ui->sending_progressBar->setValue(0);

    // get network interfaces list and add them to combo box
    getifaddrs(&addrs);
    ipa = addrs;

    while (ipa) {
       if (ipa->ifa_addr && ipa->ifa_addr->sa_family == AF_PACKET) {
           ui->interface_list_comboBox->addItem((QString) ipa->ifa_name);
       }
       ipa = ipa->ifa_next;
    }

    // check how many threads are available in system and add it to drop-down list
    this->threads = std::thread::hardware_concurrency();
    for (int i = this->threads; i >=0; i--) {
        ui->cores_num_comboBox->addItem(QString::number(i));
    }

}

MainWindow::~MainWindow()
{

    delete valid0to7;
    delete valid0and1;
    delete valid0to4095;
    delete valid0to63;
    delete valid0to2;
    delete valid0to65535;
    delete valid0to8191;
    delete valid0to255;
    delete valid0to15;
    delete valid_double;
    freeifaddrs(addrs);
    if (socket) delete socket;
    if (flags) delete [] flags;
    if (eth_h) delete eth_h;
    if (vlan_h) delete vlan_h;
    if (ip_h) delete ip_h;
    if (tcp_h) delete tcp_h;
    if (udp_h) delete udp_h;

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

    QTableWidgetItem* text1 = new QTableWidgetItem("AAAAAAAB");
    QTableWidgetItem* text2 = new QTableWidgetItem(ui->lineEdit_eth_dest_mac->text().toStdString().c_str());
    QTableWidgetItem* text3 = new QTableWidgetItem(ui->lineEdit_eth_src_mac->text().toStdString().c_str());
    QTableWidgetItem* text4 = new QTableWidgetItem(ui->lineEdit_eth_pcp->text().toStdString().c_str());
    QTableWidgetItem* text5 = new QTableWidgetItem(ui->lineEdit_eth_dei->text().toStdString().c_str());
    QTableWidgetItem* text6 = new QTableWidgetItem(ui->lineEdit_eth_vid->text().toStdString().c_str());
    QTableWidgetItem* text7 = new QTableWidgetItem(ui->lineEdit_eth_frame_type->text().toStdString().c_str());

    ui->layer2_tableWidget->setItem(0,0, text1);
    ui->layer2_tableWidget->setItem(0,1, text2);
    ui->layer2_tableWidget->setItem(0,2, text3);
    ui->layer2_tableWidget->setItem(0,3, text4);
    ui->layer2_tableWidget->setItem(0,4, text5);
    ui->layer2_tableWidget->setItem(0,5, text6);
    ui->layer2_tableWidget->setItem(0,6, text7);

}

void MainWindow::on_SaveL3Button_clicked()
{
    if ((this->eth_h == NULL) && (this->vlan_h == NULL)) {
       this->eth_h = new eth_header();
    }

    if (this->ip_h == NULL) {
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


    QTableWidgetItem* text1 = new QTableWidgetItem(ui->lineEdit_ip_version->text().toStdString().c_str());
    QTableWidgetItem* text2 = new QTableWidgetItem(ui->lineEdit_ip_ihl->text().toStdString().c_str());
    QTableWidgetItem* text3 = new QTableWidgetItem(ui->lineEdit_ip_dscp->text().toStdString().c_str());
    QTableWidgetItem* text4 = new QTableWidgetItem(ui->lineEdit_ip_ecn->text().toStdString().c_str());
    QTableWidgetItem* text5 = new QTableWidgetItem(ui->lineEdit_ip_total_length->text().toStdString().c_str());
    QTableWidgetItem* text6 = new QTableWidgetItem(ui->lineEdit_ip_id->text().toStdString().c_str());
    QTableWidgetItem* text7 = new QTableWidgetItem(ui->comboBox_ip_flags->itemText(ui->comboBox_ip_flags->currentIndex()).toStdString().c_str());
    QTableWidgetItem* text8 = new QTableWidgetItem(ui->lineEdit_ip_offset->text().toStdString().c_str());
    QTableWidgetItem* text9 = new QTableWidgetItem(ui->lineEdit_ip_ttl->text().toStdString().c_str());
    QTableWidgetItem* text10 = new QTableWidgetItem(ui->lineEdit_ip_protocol->text().toStdString().c_str());
    char* cksm = "0";
    QTableWidgetItem* text11 = new QTableWidgetItem(cksm);
    QTableWidgetItem* text12 = new QTableWidgetItem(ui->lineEdit_ip_src_ip->text().toStdString().c_str());
    QTableWidgetItem* text13 = new QTableWidgetItem(ui->lineEdit_ip_dest_ip->text().toStdString().c_str());
    QTableWidgetItem* text14 = new QTableWidgetItem(ui->lineEdit_ip_options->text().toStdString().c_str());

    ui->layer3_tableWidget->setItem(0,0, text1);
    ui->layer3_tableWidget->setItem(0,1, text2);
    ui->layer3_tableWidget->setItem(0,2, text3);
    ui->layer3_tableWidget->setItem(0,3, text4);
    ui->layer3_tableWidget->setItem(0,4, text5);
    ui->layer3_tableWidget->setItem(0,5, text6);
    ui->layer3_tableWidget->setItem(0,6, text7);
    ui->layer3_tableWidget->setItem(2,0, text8);
    ui->layer3_tableWidget->setItem(2,1, text9);
    ui->layer3_tableWidget->setItem(2,2, text10);
    ui->layer3_tableWidget->setItem(2,3, text11);
    ui->layer3_tableWidget->setItem(2,4, text12);
    ui->layer3_tableWidget->setItem(2,5, text13);
    ui->layer3_tableWidget->setItem(2,6, text14);
}

void MainWindow::on_SaveL4Button_clicked()
{
    if ((this->eth_h == NULL) && (this->vlan_h == NULL)) {
       this->eth_h = new eth_header();
    }

    if (this->ip_h == NULL) {
       this->ip_h = new ip_header();
    }

    this->tcp_h = new tcp_header();

    unsigned short int src_port = (unsigned short int) ui->lineEdit_tcp_src_port->text().toInt();
    unsigned short int dest_port = (unsigned short int) ui->lineEdit_tcp_dest_port->text().toInt();
    unsigned long int seq_num = (unsigned long int) ui->lineEdit_tcp_seq_num->text().toULong();
    unsigned long int ack_num = (unsigned long int) ui->lineEdit_tcp_ack_num->text().toULong();
    //ui->lineEdit_tcp_data_offset->setText((QString) (ui->lineEdit_tcp_data_offset->text().toInt() + 0));
    u_char data_offset = (u_char) ui->lineEdit_tcp_data_offset->text().toUShort();
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

    unsigned short int window = (unsigned short int) ui->lineEdit_tcp_window->text().toInt();
    // unsigned short int checksum = 0; //(unsigned short int) ui->tcp_checksum_lineEdit->text().toInt();
    unsigned short int urgent_pointer = (unsigned short int) ui->lineEdit_tcp_urgent_pointer->text().toInt();

    this->tcp_h->update_values(this->tcp_h, src_port, dest_port, seq_num, ack_num,
                               data_offset, ecn, control_bits, window, urgent_pointer);

    if (!(ui->lineEdit_tcp_data->text().isEmpty())) {
       this->tcp_h->fill_data(this->tcp_h, ui->lineEdit_tcp_data->text()); //?????????????
    }
}

void MainWindow::on_SendButton_clicked()
{

    // Get interface name and set it to socket
    std::string interface_name = ui->interface_list_comboBox->currentText().toStdString();
    this->socket = new sendSocket(interface_name.c_str(), "AA:BB:CC:DD:EE:FF");
    strncpy(this->socket->interface_index.ifr_name, interface_name.c_str(), interface_name.length());

    // Check amount of packages to send and set proper progress bar values
    this->num_of_packets = ui->packages_to_send_lineEdit->text().toInt();
    ui->sending_progressBar->setMinimum(0);
    ui->sending_progressBar->setMaximum(this->num_of_packets);
    ui->sending_progressBar->setTextVisible(true);

    //  Start the timer
    this->timer.start();

    //LAYER4 SECTION
    if ((ui->checkbox_TCP_create->isChecked() == true) || (ui->checkbox_UDP_create->isChecked() == true)) {
        if (ui->checkbox_TCP_create->isChecked() == true) {
            this->socket->buff_size_layer4 += 20 + ui->lineEdit_tcp_data->text().length();
            this->socket->buff_layer4 = new u_char[this->socket->buff_size_layer4];
            this->tcp_h->serialize_tcp(this->tcp_h, this->socket->buff_layer4);

            if (ui->checkBox_ip_create->isChecked() == true) {
                this->socket->buff_layer3 = new u_char[20];
                this->socket->buff_size_layer3 = 20;
                this->ip_h->update_length((unsigned short int)this->socket->buff_size_layer4);
                this->ip_h->serialize_ip(this->ip_h, this->socket->buff_layer3);

                if (ui->comboBox_ip_checksum->currentIndex() == 1) {
                   this->ip_h->calculate_checksum(this->ip_h, this->socket->buff_layer3, 10);
                   this->ip_h->serialize_ip(this->ip_h, this->socket->buff_layer3);
                }
            }
            else {
                this->ip_h = new ip_header();
                this->ip_h->update_length((unsigned short int)this->socket->buff_size_layer4);
                this->socket->buff_layer3 = new u_char[20];
                this->socket->buff_size_layer3 = 20;
                this->ip_h->serialize_ip(this->ip_h, this->socket->buff_layer3);
                this->ip_h->calculate_checksum(this->ip_h, this->socket->buff_layer3, 10);
                this->ip_h->serialize_ip(this->ip_h, this->socket->buff_layer3);
            }

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
        else if (ui->checkbox_UDP_create->isChecked() == true) {
            //TODO
        }
    }

    //LAYER3 SECTION
    else {
        if (ui->checkBox_ip_create->isChecked() == true) {
            this->socket->buff_layer3 = new u_char[20];
            this->socket->buff_size_layer3 = 20;
            this->ip_h->update_length((unsigned short int)this->socket->buff_size_layer4);
            this->ip_h->serialize_ip(this->ip_h, this->socket->buff_layer3);
            this->ip_h->calculate_checksum(this->ip_h, this->socket->buff_layer3, 10);
            this->ip_h->serialize_ip(this->ip_h, this->socket->buff_layer3);

            if (ui->comboBox_ip_checksum->currentIndex() == 1) {
               this->ip_h->calculate_checksum(this->ip_h, this->socket->buff_layer3, 10);
               this->ip_h->serialize_ip(this->ip_h, this->socket->buff_layer3);
            }

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
        else {
            if (ui->checkBox_eth_vlan->isChecked() == true) {
               this->socket->buff_layer2 = new u_char[20];
               this->socket->buff_size_layer2 = 20;
               this->vlan_h->serialize_eth_802Q(this->vlan_h, this->socket->buff_layer2);
            }
            else if (this->eth_h != NULL) {
               this->socket->buff_layer2 = new u_char[16];
               this->socket->buff_size_layer2 = 16;
               this->eth_h->serialize_eth(this->eth_h, this->socket->buff_layer2);
            }
            else {
                this->eth_h = new eth_header();
                //MEMORY LEAK
                this->socket->buff_layer2 = new u_char[16];
                this->socket->buff_size_layer2 = 16;
                this->eth_h->serialize_eth(this->eth_h, this->socket->buff_layer2);
            }
        }

   }

    //MEMORY LEAK
    this->socket->buff_begin = new u_char[this->socket->buff_size_layer2 + this->socket->buff_size_layer3 + this->socket->buff_size_layer4];

    //BUFFERS SECTION
    memcpy(this->socket->buff_begin, this->socket->buff_layer2, this->socket->buff_size_layer2);
    memcpy((this->socket->buff_begin + this->socket->buff_size_layer2), this->socket->buff_layer3, this->socket->buff_size_layer3);
    memcpy((this->socket->buff_begin + this->socket->buff_size_layer2 + this->socket->buff_size_layer3), this->socket->buff_layer4, this->socket->buff_size_layer4);

    delete [] this->socket->buff_layer2;
    delete [] this->socket->buff_layer3;
    delete [] this->socket->buff_layer4;

    //SENDING SECTION
    bool* rand_flags = this->setFlags();
    unsigned short int* to_send_ip = (unsigned short int*) (this->socket->buff_begin);
    unsigned short int* to_send_tcp = (unsigned short int*) (this->socket->buff_begin);

    if (ui->checkBox_eth_vlan->isChecked() == true) {
       to_send_ip += 14;
       to_send_tcp += 27;
    }
    else {
       to_send_ip += 12;
       to_send_tcp += 25;
    }

    //SENDING LOOP
    for (int i = 1; i <= (this->num_of_packets); ++i) {

        //check if flags are set and randomize values
        this->randomize(rand_flags);

        if (ui->comboBox_tcp_checksum->currentIndex() == 1) {
            (*to_send_tcp) = 0;
            (*to_send_tcp) = this->tcp_h->calculate_checksum(this->tcp_h, this->ip_h, ((this->socket->buff_begin) + this->socket->buff_size_layer2 + this->socket->buff_size_layer3), (this->socket->buff_size_layer4));
        }

        //check if user has chosen to calculate IP header checksum
        if (ui->comboBox_ip_checksum->currentIndex() == 1) {
           (*to_send_ip) = 0;
           (*to_send_ip) = this->ip_h->calculate_checksum(this->ip_h, ((this->socket->buff_begin) + this->socket->buff_size_layer2), 10);
        }


        //TODO
        //check if user has chosen to calculate TCP/UDP header checksum

        //sending section
        this->socket->send_packet(*(this->socket), this->socket->buff_begin, (this->socket->buff_size_layer2 + this->socket->buff_size_layer3 + this->socket->buff_size_layer4));

        // display progress sections
        ui->sending_progressBar->setValue(i);
        ui->sending_time_lcdNumber->display((double) this->timer.elapsed());
    }
    this->timer.restart();

    //delete [] this->socket->buff_begin;
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

    this->flags = new bool[15];
    memset(flags, 0, 15);

    if (ui->checkBox_eth_rand_src_mac->isChecked() == true) {
        flags[0] = 1;
    }

    if (ui->checkBox_eth_rand_dest_mac->isChecked() == true) {
        flags[1] = 1;
    }

    if (ui->checkBox_eth_rand_pcp->isChecked() == true) {
        flags[2] = 1;
    }

    if (ui->checkBox_eth_rand_dei->isChecked() == true) {
        flags[3] = 1;
    }

    if (ui->checkBox_eth_rand_vid->isChecked() == true) {
        flags[4] = 1;
    }

    if (ui->checkBox_ip_rand_id->isChecked() == true) {
        flags[5] = 1;
    }

    if (ui->checkBox_ip_rand_ttl->isChecked() == true) {
        flags[6] = 1;
    }

    if (ui->checkBox_ip_rand_src_ip->isChecked() == true) {
        flags[7] = 1;
    }

    if (ui->checkBox_ip_rand_dest_ip->isChecked() == true) {
        flags[8] = 1;
    }

    if (ui->checkBox_tcp_rand_src_port->isChecked() == true) {
        flags[9] = 1;
    }

    if (ui->checkBox_tcp_rand_dest_port->isChecked() == true) {
        flags[10] = 1;
    }

    if (ui->checkBox_tcp_rand_seq_number->isChecked() == true) {
        flags[11] = 1;
    }

    if (ui->checkBox_tcp_rand_ack_num->isChecked() == true) {
        flags[12] = 1;
    }

    if (ui->checkBox_udp_rand_src_port->isChecked() == true) {
        flags[13] = 1;
    }

    if (ui->checkBox_udp_rand_dest_port->isChecked() == true) {
        flags[14] = 1;
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
       clean_table(ui->layer3_tableWidget);
       ui->groupBox_layer3->setDisabled(true);
       if (this->ip_h != NULL) {
           delete this->ip_h;
           this->ip_h = NULL;
       }
    }

}

void MainWindow::on_checkBox_eth_vlan_toggled(bool checked)
{
    if (checked)
        ui->lineEdit_eth_frame_type->setText("0x8100");
    else
        ui->lineEdit_eth_frame_type->setText("0x8000");
}

void MainWindow::on_checkbox_TCP_create_toggled(bool checked)
{
    if(checked) {
        ui->TCP_groupBox->setEnabled(true);
        ui->UDP_groupBox->setDisabled(true);
        ui->checkbox_UDP_create->setDisabled(true);
    }
    else {
        clean_table(ui->layer4_tableWidget);
        ui->checkbox_UDP_create->setEnabled(true);
        ui->TCP_groupBox->setDisabled(true);
        if (this->tcp_h != NULL) {
            delete this->tcp_h;
            this->tcp_h = NULL;
        }
    }

}

void MainWindow::on_checkbox_UDP_create_toggled(bool checked)
{
    if(checked) {
        ui->UDP_groupBox->setEnabled(true);
        ui->TCP_groupBox->setDisabled(true);
        ui->checkbox_TCP_create->setDisabled(true);
    }
    else {
        clean_table(ui->layer4_tableWidget);
        ui->checkbox_TCP_create->setEnabled(true);
        ui->UDP_groupBox->setDisabled(true);
        if (this->udp_h != NULL) {
            delete this->udp_h;
            this->udp_h = NULL;
        }
    }
}

void MainWindow::clean_table(QTableWidget *table) {
     int columns = table->columnCount();
     int rows = table->rowCount();
     int i = 0;
     while (i < rows) {
         for (int j = 0; j < columns; j++) {
             QTableWidgetItem* text = new QTableWidgetItem("---");
             table->setItem(i, j, text);
         }
         i+=2;
     }
}


void MainWindow::on_packages_to_send_lineEdit_textEdited(const QString &arg1)
{
    if(arg1.toLong() >= threads) {
        ui->cores_num_comboBox->setEnabled(true);
    }
    else if (arg1.toLong() < threads){\
        ui->cores_num_comboBox->setDisabled(true);
    }

}
