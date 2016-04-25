#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <sys/types.h>
#include <ifaddrs.h>
#include <thread>
#include <QMessageBox>

/*
 #include <random>
 std::random_device rd;     // only used once to initialise (seed) engine
 std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
 std::uniform_int_distribution<int> uni(0,7); // guaranteed unbiase
 */

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);

	//fix for rand working properly in loops
	unsigned long long int clockCount;
	__asm__ volatile (".byte 0x0f, 0x31" : "=A" (clockCount));
	srand((unsigned) clockCount);

	setValidators();

	// UI SETTINGS
	ui->TCP_groupBox->setDisabled(true);
	ui->UDP_groupBox->setDisabled(true);

	ui->sending_progressBar->setValue(0);

	// get network interfaces list and add them to combo box
	getifaddrs (&addrs);
	ipa = addrs;

	while (ipa) {
		if (ipa->ifa_addr && ipa->ifa_addr->sa_family == AF_PACKET) {
			ui->interface_list_comboBox->addItem((QString) ipa->ifa_name);
		}
		ipa = ipa->ifa_next;
	}

	// check how many threads are available in system and add it to drop-down list
	threads = std::thread::hardware_concurrency();
	for (int i = this->threads; i >= 0; i--) {
		ui->cores_num_comboBox->addItem(QString::number(i));
	}

}

MainWindow::~MainWindow() {

    delete valid0to7;
    valid0to7 = NULL;
	delete valid0and1;
    valid0and1 = NULL;
	delete valid0to4095;
    valid0to4095 = NULL;
	delete valid0to63;
    valid0to63 = NULL;
	delete valid0to2;
    valid0to2 = NULL;
	delete valid0to65535;
    valid0to65535 = NULL;
	delete valid0to8191;
    valid0to8191 = NULL;
	delete valid0to255;
    valid0to255 = NULL;
	delete valid0to15;
    valid0to15 = NULL;
	delete valid_double;
    valid_double = NULL;
	freeifaddrs (addrs);
    if (socket != NULL) {
		delete socket;
        socket = NULL;
    }
    if (flags != NULL) {
		delete[] flags;
        flags = NULL;
    }
    if (eth_h != NULL) {
		delete eth_h;
        eth_h = NULL;
    }
    if (vlan_h != NULL) {
		delete vlan_h;
        vlan_h = NULL;
    }
    if (ip_h != NULL) {
		delete ip_h;
        ip_h = NULL;
    }
    if (tcp_h != NULL) {
		delete tcp_h;
        tcp_h = NULL;
    }
    if (udp_h != NULL) {
		delete udp_h;
        udp_h = NULL;
    }

	delete ui;
}

void MainWindow::on_SaveL2Button_clicked() {
	if (ui->checkBox_eth_vlan->isChecked()) {
        if(eth_h != NULL) {
            delete eth_h;
        }
		eth_h = NULL;
		ui->lineEdit_eth_frame_type->setText("0x8100");
		vlan_h = new eth_802Q();
		std::string src_mac = ui->lineEdit_eth_src_mac->text().toStdString();
		std::string dest_mac = ui->lineEdit_eth_dest_mac->text().toStdString();

		vlan_h->update_src_mac(vlan_h, src_mac);
		vlan_h->update_dest_mac(vlan_h, dest_mac);

		int PCPbin = (ui->lineEdit_eth_pcp->text().toInt()) << 13;
		int DEIbin = (ui->lineEdit_eth_dei->text().toInt()) << 12;
		int VID = (ui->lineEdit_eth_vid->text().toInt());

		int TCIbin = PCPbin | DEIbin | VID;
		vlan_h->update_tci(vlan_h, TCIbin);
	} else {
        if (vlan_h != NULL) {
            delete vlan_h;
        }
		vlan_h = NULL;
		ui->lineEdit_eth_frame_type->setText("0x8000");
		eth_h = new eth_header();
		std::string src_mac = ui->lineEdit_eth_src_mac->text().toStdString();
		std::string dest_mac = ui->lineEdit_eth_dest_mac->text().toStdString();
		eth_h->update_src_mac(eth_h, src_mac);
		eth_h->update_dest_mac(eth_h, dest_mac);
	}

    fill_eth_table();

}

void MainWindow::on_SaveL3Button_clicked() {

	if ((eth_h == NULL) && (vlan_h == NULL)) {
		eth_h = new eth_header();
	}

	if (ip_h == NULL) {
		ip_h = new ip_header();
	}

	u_char dscp = (u_char) ui->lineEdit_ip_dscp->text().toInt();
	u_char ecn = (u_char) ui->lineEdit_ip_ecn->text().toInt();
	dscp = dscp << 2;
	u_char ToS = (dscp | ecn);
	u_char flags = (u_char) ui->comboBox_ip_flags->currentIndex();

	ip_h->update_src_ip(ip_h, ui->lineEdit_ip_src_ip->text().toStdString());
	ip_h->update_dest_ip(ip_h, ui->lineEdit_ip_dest_ip->text().toStdString());
	ip_h->update_values(ip_h, ToS,
			(unsigned short int) (ui->lineEdit_ip_total_length->text().toInt()),
			(unsigned short int) (ui->lineEdit_ip_id->text().toInt()), flags,
			(u_char)(ui->lineEdit_ip_offset->text().toInt()),
			(u_char)(ui->lineEdit_ip_ttl->text().toInt()),
			(u_char)(ui->lineEdit_ip_protocol->text().toInt()));

    fill_eth_table();
    fill_ip_table();

}

void MainWindow::on_SaveL4Button_clicked() {

	if (ui->checkbox_TCP_create->isChecked()) {

		if ((eth_h == NULL) && (vlan_h == NULL)) {
			fill_eth_table();
			eth_h = new eth_header();
		}

		if (ip_h == NULL) {
            fill_ip_table();
			ip_h = new ip_header();
            ui->checkBox_ip_create->setChecked(true);
            ui->groupBox_layer3->setEnabled(true);
		}

        if(udp_h != NULL) {
            delete udp_h;
            udp_h = NULL;
        }

		tcp_h = new tcp_header();

		int data_length = ui->lineEdit_tcp_data->text().size();
		if (!(ui->lineEdit_tcp_data->text().isEmpty())) {
			tcp_h->fill_data(tcp_h, ui->lineEdit_tcp_data->text());
		}

		unsigned short int src_port =
				(unsigned short int) ui->lineEdit_tcp_src_port->text().toInt();
		unsigned short int dest_port =
				(unsigned short int) ui->lineEdit_tcp_dest_port->text().toInt();
		unsigned long int seq_num =
				(unsigned long int) ui->lineEdit_tcp_seq_num->text().toULong();
		unsigned long int ack_num =
				(unsigned long int) ui->lineEdit_tcp_ack_num->text().toULong();
		u_char data_offset =
				(u_char) ui->lineEdit_tcp_data_offset->text().toUShort();
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

		unsigned short int window =
                (unsigned short int) ui->lineEdit_tcp_window_size->text().toInt();
		unsigned short int urgent_pointer =
				(unsigned short int) ui->lineEdit_tcp_urgent_pointer->text().toInt();

		tcp_h->update_values(tcp_h, src_port, dest_port, seq_num, ack_num,
				data_offset, ecn, control_bits, window, urgent_pointer);

        int full_length = (20 + 20 + ui->lineEdit_tcp_data->text().length()); //IP header length = 20 and TCP header length = 20;
        std::string length_string = std::to_string(full_length);
        QString final_string = QString::fromStdString(length_string);
        ui->lineEdit_ip_total_length->setText(final_string);

        fill_eth_table();
        fill_ip_table();
        update_table_ip_length(20, data_length);

        clean_table(ui->layer4_tableWidget);
        fill_tcp_table();

	} else if (ui->checkbox_UDP_create->isChecked()) {

		if ((eth_h == NULL) && (vlan_h == NULL)) {
			fill_eth_table();
			eth_h = new eth_header();
		}

		if (ip_h == NULL) {
            fill_ip_table();
			ip_h = new ip_header();
		}

        if(tcp_h != NULL) {
            delete tcp_h;
            tcp_h = NULL;
        }

		udp_h = new udp_header();

		int data_length = ui->lineEdit_udp_data->text().size();
		if (!(ui->lineEdit_udp_data->text().isEmpty())) {
			udp_h->fill_data(udp_h, ui->lineEdit_udp_data->text());
		}

		unsigned short int src_port =
				(unsigned short int) ui->lineEdit_udp_src_port->text().toInt();
		unsigned short int dest_port =
				(unsigned short int) ui->lineEdit_udp_dest_port->text().toInt();
		unsigned short int length = ui->lineEdit_udp_length->text().toInt();

		udp_h->update_values(udp_h, src_port, dest_port,
				(length + data_length));
        update_table_ip_length(8, data_length);

        int full_length = (20 + 8 + ui->lineEdit_udp_data->text().length()); //IP header length = 20 and UDP header length = 8;
        std::string length_string = std::to_string(full_length);
        QString final_string = QString::fromStdString(length_string);
        ui->lineEdit_ip_total_length->setText(final_string);

	} else {
        ui->lineEdit_ip_total_length->setText("20");
        if (ip_h != NULL) {
            ip_h->setLength(20);
        }

        QMessageBox* box = new QMessageBox();
        box->setText("You must choose between TCP or UDP packet form in order to save Layer 4");
        box->exec();
	}

}

void MainWindow::on_SendButton_clicked() {

	// Get interface name and set it to socket
	std::string interface_name =
			ui->interface_list_comboBox->currentText().toStdString();
	socket = new sendSocket(interface_name.c_str(), "AA:BB:CC:DD:EE:FF");
	strncpy(socket->interface_index.ifr_name, interface_name.c_str(),
			interface_name.length());

	// Check amount of packages to send and set proper progress bar values
	num_of_packets = ui->packages_to_send_lineEdit->text().toInt();
	ui->sending_progressBar->setMinimum(0);
	ui->sending_progressBar->setMaximum(num_of_packets);
	ui->sending_progressBar->setTextVisible(true);

	//  Start the timer
	timer.start();

	//LAYER4 SECTION
	if ((ui->checkbox_TCP_create->isChecked() == true)
			|| (ui->checkbox_UDP_create->isChecked() == true)) {
		if (ui->checkbox_TCP_create->isChecked() == true) {
			socket->buff_size_layer4 = 20
					+ ui->lineEdit_tcp_data->text().length();
			socket->buff_layer4 = new u_char[socket->buff_size_layer4];
			tcp_h->serialize_tcp(tcp_h, socket->buff_layer4);

			if (ui->checkBox_ip_create->isChecked() == true) {
				socket->buff_layer3 = new u_char[20];
				socket->buff_size_layer3 = 20;
				ip_h->update_length(
						(unsigned short int) socket->buff_size_layer4);
				ip_h->serialize_ip(ip_h, socket->buff_layer3);

                if (ui->comboBox_ip_checksum->currentIndex() == 0) {
					ip_h->calculate_checksum(ip_h, socket->buff_layer3, 10);
					ip_h->serialize_ip(ip_h, socket->buff_layer3);
				}
			} else {
				ip_h = new ip_header();
                ip_h->update_protocol(ip_h, 6);
				ip_h->update_length(
						(unsigned short int) socket->buff_size_layer4);
				socket->buff_layer3 = new u_char[20];
				socket->buff_size_layer3 = 20;
				ip_h->serialize_ip(ip_h, socket->buff_layer3);
				ip_h->calculate_checksum(ip_h, socket->buff_layer3, 10);
				ip_h->serialize_ip(ip_h, socket->buff_layer3);
			}

			if (ui->checkBox_eth_vlan->isChecked() == true) {
				socket->buff_layer2 = new u_char[18];
				socket->buff_size_layer2 = 18;
				vlan_h->serialize_eth_802Q(vlan_h, socket->buff_layer2);
			} else {
				socket->buff_layer2 = new u_char[14];
				socket->buff_size_layer2 = 14;
				eth_h->serialize_eth(eth_h, socket->buff_layer2);
			}

		} else if (ui->checkbox_UDP_create->isChecked() == true) {
			socket->buff_size_layer4 = 8
					+ ui->lineEdit_udp_data->text().length();

			socket->buff_layer4 = new u_char[socket->buff_size_layer4];
			udp_h->serialize_udp(udp_h, socket->buff_layer4);

			if (ui->checkBox_ip_create->isChecked() == true) {
				socket->buff_layer3 = new u_char[20];
				socket->buff_size_layer3 = 20;
				ip_h->update_length(
						(unsigned short int) socket->buff_size_layer4);
				ip_h->serialize_ip(ip_h, socket->buff_layer3);

                if (ui->comboBox_ip_checksum->currentIndex() == 0) {
					ip_h->calculate_checksum(ip_h, socket->buff_layer3, 10);
					ip_h->serialize_ip(ip_h, socket->buff_layer3);
				}
			} else {
				ip_h = new ip_header();
                ip_h->update_protocol(ip_h, 17);
				ip_h->update_length(
						(unsigned short int) socket->buff_size_layer4);
				socket->buff_layer3 = new u_char[20];
				socket->buff_size_layer3 = 20;
				ip_h->serialize_ip(ip_h, socket->buff_layer3);
				ip_h->calculate_checksum(ip_h, socket->buff_layer3, 10);
				ip_h->serialize_ip(ip_h, socket->buff_layer3);
			}

			if (ui->checkBox_eth_vlan->isChecked() == true) {
				socket->buff_layer2 = new u_char[18];
				socket->buff_size_layer2 = 18;
				vlan_h->serialize_eth_802Q(vlan_h, socket->buff_layer2);
			} else {
				socket->buff_layer2 = new u_char[14];
				socket->buff_size_layer2 = 14;
				eth_h->serialize_eth(eth_h, socket->buff_layer2);
			}
		}
	}

	//LAYER3 SECTION
	else {
		if (ui->checkBox_ip_create->isChecked() == true) {
			socket->buff_layer3 = new u_char[20];
			socket->buff_size_layer3 = 20;
			ip_h->update_length((unsigned short int) socket->buff_size_layer4);
			ip_h->serialize_ip(ip_h, socket->buff_layer3);
			ip_h->calculate_checksum(ip_h, socket->buff_layer3, 10);
			ip_h->serialize_ip(ip_h, socket->buff_layer3);

            if (ui->comboBox_ip_checksum->currentIndex() == 0) {
				ip_h->calculate_checksum(ip_h, socket->buff_layer3, 10);
				ip_h->serialize_ip(ip_h, socket->buff_layer3);
			}

			if (ui->checkBox_eth_vlan->isChecked() == true) {
				socket->buff_layer2 = new u_char[18];
				socket->buff_size_layer2 = 18;
				vlan_h->serialize_eth_802Q(vlan_h, socket->buff_layer2);
			} else {
				socket->buff_layer2 = new u_char[14];
				socket->buff_size_layer2 = 14;
				eth_h->serialize_eth(eth_h, socket->buff_layer2);
			}

		} else {
			if (ui->checkBox_eth_vlan->isChecked() == true) {
				socket->buff_layer2 = new u_char[20];
				socket->buff_size_layer2 = 20;
				vlan_h->serialize_eth_802Q(vlan_h, socket->buff_layer2);
			} else if (this->eth_h != NULL) {
				socket->buff_layer2 = new u_char[16];
				socket->buff_size_layer2 = 16;
				eth_h->serialize_eth(eth_h, socket->buff_layer2);
			} else {
				eth_h = new eth_header();
				//MEMORY LEAK
				socket->buff_layer2 = new u_char[16];
				socket->buff_size_layer2 = 16;
				eth_h->serialize_eth(eth_h, socket->buff_layer2);
                fill_eth_table();
			}
		}

	}

	//MEMORY LEAK
	socket->buff_begin = new u_char[socket->buff_size_layer2
			+ socket->buff_size_layer3 + socket->buff_size_layer4];

	//BUFFERS SECTION
	memcpy(socket->buff_begin, socket->buff_layer2, socket->buff_size_layer2);
	memcpy((socket->buff_begin + socket->buff_size_layer2), socket->buff_layer3,
			socket->buff_size_layer3);
	memcpy(
			(socket->buff_begin + socket->buff_size_layer2
					+ socket->buff_size_layer3), socket->buff_layer4,
			socket->buff_size_layer4);

//    delete[] socket->buff_layer2;
//    socket->buff_layer2 = NULL;
//    delete[] socket->buff_layer3;
//    socket->buff_layer3 = NULL;
//    delete[] socket->buff_layer4;l
//    socket->buff_layer4 = NULL;

	//SENDING SECTION
	bool* rand_flags = setFlags();
	unsigned short int* to_send_ip = (unsigned short int*) (socket->buff_begin);
    unsigned short int* to_send_tcp = (unsigned short int*) (socket->buff_begin);
    unsigned short int* to_send_udp = (unsigned short int*) (socket->buff_begin);

	if (ui->checkBox_eth_vlan->isChecked() == true) {
		to_send_ip += 14;
        to_send_tcp += 27;
        to_send_udp += 22;
	} else {
		to_send_ip += 12;
        to_send_tcp += 25;
        to_send_udp += 20;
	}

	//SENDING LOOP
    for (double i = 1; i <= (num_of_packets); ++i) {

		//check if flags are set and randomize values
		randomize(rand_flags);

        if (ui->comboBox_tcp_checksum->currentIndex() == 0) {
            (*to_send_tcp) = 0;
            (*to_send_tcp) = tcp_h->calculate_checksum(tcp_h, ip_h,
					((socket->buff_begin) + socket->buff_size_layer2
							+ socket->buff_size_layer3),
					(socket->buff_size_layer4));
		}

        if (ui->comboBox_udp_checksum->currentIndex() == 0) {
            (*to_send_udp) = 0;
            (*to_send_udp) = udp_h->calculate_checksum(udp_h, ip_h,
					((socket->buff_begin) + socket->buff_size_layer2
							+ socket->buff_size_layer3),
					(socket->buff_size_layer4));
		}

		//check if user has chosen to calculate IP header checksum
        if (ui->comboBox_ip_checksum->currentIndex() == 0) {
			(*to_send_ip) = 0;
			(*to_send_ip) = ip_h->calculate_checksum(ip_h,
					((socket->buff_begin) + socket->buff_size_layer2), 10);
		}

		//TODO
		//check if user has chosen to calculate TCP/UDP header checksum

		//sending section
		socket->send_packet(*(socket), socket->buff_begin,
				(socket->buff_size_layer2 + socket->buff_size_layer3
						+ socket->buff_size_layer4));

		// display progress sections
        ui->sending_progressBar->setValue(i);
		ui->sending_time_lcdNumber->display((double) timer.elapsed());
	}
    to_send_ip = NULL;
    to_send_tcp = NULL;
    to_send_udp = NULL;

	timer.restart();

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

	flags = new bool[15];
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
			vlan_h->random_mac_addr((socket->buff_begin), 0, 1);
		else
			eth_h->random_mac_addr((socket->buff_begin), 0, 1);

	}
	if (flags[1] != 0) {
		if (vlan)
			vlan_h->random_mac_addr((socket->buff_begin), 1, 0);
		else
			eth_h->random_mac_addr((socket->buff_begin), 1, 0);
	}
	if (flags[2] != 0) {
		if (vlan)
			vlan_h->rand_pcp(socket->buff_begin);
	}
	if (flags[3] != 0) {
		if (vlan)
			vlan_h->rand_dei(socket->buff_begin);
	}
	if (flags[4] != 0) {
		if (vlan)
			vlan_h->rand_vid(socket->buff_begin);
	}
	if (flags[5] != 0) {
		if (vlan)
			ip_h->rand_id(ip_h, socket->buff_begin, true);
		else
			ip_h->rand_id(ip_h, socket->buff_begin, false);
	}
	if (flags[6] != 0) {
		if (vlan)
			ip_h->rand_ttl(ip_h, socket->buff_begin, 1);
		else
			ip_h->rand_ttl(ip_h, socket->buff_begin, 0);
	}
	if (flags[7] != 0) {
		if (vlan)
			ip_h->rand_ip(ip_h, socket->buff_begin, 1, 1, 0);
		else
			ip_h->rand_ip(ip_h, socket->buff_begin, 0, 1, 0);
	}
	if (flags[8] != 0) {
		if (vlan)
			ip_h->rand_ip(ip_h, socket->buff_begin, 1, 0, 1);
		else
			ip_h->rand_ip(ip_h, socket->buff_begin, 0, 0, 1);
	}
	if (flags[9] != 0) {
		if (vlan)
			tcp_h->rand_port(tcp_h, socket->buff_begin, 1, 1, 0);
		else
			tcp_h->rand_port(tcp_h, socket->buff_begin, 0, 1, 0);
	}
	if (flags[10] != 0) {
		if (vlan)
			tcp_h->rand_port(tcp_h, socket->buff_begin, 1, 0, 1);
		else
			tcp_h->rand_port(tcp_h, socket->buff_begin, 0, 0, 1);
	}
	if (flags[11] != 0) {
		if (vlan)
			tcp_h->rand_seq_num(tcp_h, socket->buff_begin, 1);
		else
			tcp_h->rand_seq_num(tcp_h, socket->buff_begin, 0);
	}
	if (flags[12] != 0) {
		if (vlan)
			tcp_h->rand_ack_num(tcp_h, socket->buff_begin, 1);
		else
			tcp_h->rand_ack_num(tcp_h, socket->buff_begin, 0);
	}
	if (flags[13] != 0) {
		if (vlan)
			udp_h->rand_port(udp_h, socket->buff_begin, 1, 1, 0);
		else
			udp_h->rand_port(udp_h, socket->buff_begin, 0, 1, 0);
	}
	if (flags[14] != 0) {
		if (vlan)
			udp_h->rand_port(udp_h, socket->buff_begin, 1, 0, 1);
		else
			udp_h->rand_port(udp_h, socket->buff_begin, 0, 0, 1);
	}

}

void MainWindow::on_checkBox_ip_create_toggled(bool checked) {
	if (checked) {
		ui->groupBox_layer3->setEnabled(true);
	} else {
		clean_table(ui->layer3_tableWidget);
		ui->groupBox_layer3->setDisabled(true);
		if (ip_h != NULL) {
			delete ip_h;
			ip_h = NULL;
		}
	}

}

void MainWindow::on_checkBox_eth_vlan_toggled(bool checked) {
	if (checked)
		ui->lineEdit_eth_frame_type->setText("0x8100");
	else
		ui->lineEdit_eth_frame_type->setText("0x8000");
}

void MainWindow::on_checkbox_TCP_create_toggled(bool checked) {
	if (checked) {
		ui->TCP_groupBox->setEnabled(true);
		ui->UDP_groupBox->setDisabled(true);
		ui->checkbox_UDP_create->setDisabled(true);
        ui->lineEdit_ip_protocol->setText("6");
        if (ip_h != NULL) {
            ip_h->setProtocol(6);
        }
	} else {
		clean_table(ui->layer4_tableWidget);
		ui->checkbox_UDP_create->setEnabled(true);
		ui->TCP_groupBox->setDisabled(true);
        ui->checkBox_tcp_control_bits_ack->setChecked(false);
        ui->checkBox_tcp_control_bits_fin->setChecked(false);
        ui->checkBox_tcp_control_bits_psh->setChecked(false);
        ui->checkBox_tcp_control_bits_rst->setChecked(false);
        ui->checkBox_tcp_control_bits_syn->setChecked(false);
        ui->checkBox_tcp_control_bits_urg->setChecked(false);
        ui->checkBox_tcp_ecn_cwr->setChecked(false);
        ui->checkBox_tcp_ecn_ece->setChecked(false);
        ui->checkBox_tcp_ecn_ns->setChecked(false);
        ui->checkBox_tcp_rand_ack_num->setChecked(false);
        ui->checkBox_tcp_rand_dest_port->setChecked(false);
        ui->checkBox_tcp_rand_seq_number->setChecked(false);
        ui->checkBox_tcp_rand_src_port->setChecked(false);
        ui->comboBox_tcp_checksum->setCurrentIndex(0);
        if (ip_h != NULL) {
            ip_h->setProtocol(6);
        }

		if (tcp_h != NULL) {
			delete tcp_h;
			tcp_h = NULL;
		}
	}

}

void MainWindow::on_checkbox_UDP_create_toggled(bool checked) {
	if (checked) {
        delete tcp_h;
        tcp_h = NULL;
		ui->UDP_groupBox->setEnabled(true);
		ui->TCP_groupBox->setDisabled(true);
		ui->checkbox_TCP_create->setDisabled(true);
        ui->lineEdit_ip_protocol->setText("17");
        if (ip_h != NULL) {
            ip_h->setProtocol(17);
        }
	} else {
		clean_table(ui->layer4_tableWidget);
		ui->checkbox_TCP_create->setEnabled(true);
		ui->UDP_groupBox->setDisabled(true);
        ui->checkBox_udp_rand_src_port->setChecked(false);
        ui->checkBox_udp_rand_dest_port->setChecked(false);
        ui->comboBox_udp_checksum->setCurrentIndex(0);
        ui->lineEdit_ip_protocol->setText("6");
        if (ip_h != NULL) {
            ip_h->setProtocol(6);
        }

        if (udp_h != NULL) {
			delete udp_h;
			udp_h = NULL;
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
		i += 2;
	}
}

void MainWindow::on_packages_to_send_lineEdit_textEdited(const QString &arg1) {
	if (arg1.toLong() >= threads) {
		ui->cores_num_comboBox->setEnabled(true);
	} else if (arg1.toLong() < threads) {
		ui->cores_num_comboBox->setDisabled(true);
	}

}

void MainWindow::fill_eth_table() {

    QTableWidgetItem* preamble = new QTableWidgetItem("AAAAAAAB");
    QTableWidgetItem* dest_mac = NULL;
    if (ui->checkBox_eth_rand_dest_mac->isChecked() == true) {
        dest_mac = new QTableWidgetItem("Random");
    } else {
        dest_mac = new QTableWidgetItem(ui->lineEdit_eth_dest_mac->text().toStdString().c_str());
    }
    QTableWidgetItem* src_mac =  NULL;
    if (ui->checkBox_eth_rand_src_mac->isChecked() == true) {
        src_mac = new QTableWidgetItem("Random");
    } else {
        src_mac = new QTableWidgetItem(ui->lineEdit_eth_src_mac->text().toStdString().c_str());
    }
    QTableWidgetItem* pcp = new QTableWidgetItem("Not used");
    QTableWidgetItem* dei = new QTableWidgetItem("Not used");
    QTableWidgetItem* vlan_id = new QTableWidgetItem("Not used");
    QTableWidgetItem* ethertype = new QTableWidgetItem("0x8000");

    if (vlan_h != NULL) {
        delete pcp;
        pcp = new QTableWidgetItem(
                ui->lineEdit_eth_pcp->text().toStdString().c_str());
        delete dei;
        dei = new QTableWidgetItem(
                ui->lineEdit_eth_dei->text().toStdString().c_str());
        delete vlan_id;
        vlan_id = new QTableWidgetItem(
                ui->lineEdit_eth_vid->text().toStdString().c_str());
        delete ethertype;
        ethertype = new QTableWidgetItem("0x8100");
    }

    ui->layer2_tableWidget->setItem(0, 0, preamble);
    ui->layer2_tableWidget->item(0, 0)->setTextAlignment( Qt::AlignCenter );
    ui->layer2_tableWidget->setItem(0, 1, dest_mac);
    ui->layer2_tableWidget->item(0, 1)->setTextAlignment( Qt::AlignCenter );
    ui->layer2_tableWidget->setItem(0, 2, src_mac);
    ui->layer2_tableWidget->item(0, 2)->setTextAlignment( Qt::AlignCenter );
    ui->layer2_tableWidget->setItem(0, 3, pcp);
    ui->layer2_tableWidget->item(0, 3)->setTextAlignment( Qt::AlignCenter );
    ui->layer2_tableWidget->setItem(0, 4, dei);
    ui->layer2_tableWidget->item(0, 4)->setTextAlignment( Qt::AlignCenter );
    ui->layer2_tableWidget->setItem(0, 5, vlan_id);
    ui->layer2_tableWidget->item(0, 5)->setTextAlignment( Qt::AlignCenter );
    ui->layer2_tableWidget->setItem(0, 6, ethertype);
    ui->layer2_tableWidget->item(0, 6)->setTextAlignment( Qt::AlignCenter );

}

void MainWindow::fill_ip_table() {

    QTableWidgetItem* ver = new QTableWidgetItem("4");
    QTableWidgetItem* ihl = new QTableWidgetItem("5");
    QTableWidgetItem* dscp = new QTableWidgetItem("0");
    QTableWidgetItem* ecn = new QTableWidgetItem("0");
    int len = 0;
    if (vlan_h != 0) {
        len = 18;
    }
    else {
        len = 14;
    }
    len += 20; //IP Header length

	std::string len_str = std::to_string(len);
    QTableWidgetItem* length = new QTableWidgetItem(len_str.c_str());

    QTableWidgetItem* id = NULL;
    if (ui->checkBox_ip_rand_id->isChecked() == true) {
        id = new QTableWidgetItem("Random");
    } else {
        id = new QTableWidgetItem(ui->lineEdit_ip_id->text().toStdString().c_str());
    }

    QTableWidgetItem* flags = NULL;
    if (ui->comboBox_ip_flags->currentIndex() == 0) {
        flags = new QTableWidgetItem("Reserved");
    }
    else if (ui->comboBox_ip_flags->currentIndex() == 1) {
        flags = new QTableWidgetItem("DF");
    }
    else if (ui->comboBox_ip_flags->currentIndex() == 2) {
        flags = new QTableWidgetItem("MF");
    }


    QTableWidgetItem* offset = new QTableWidgetItem(ui->lineEdit_ip_offset->text().toStdString().c_str());
    QTableWidgetItem* ttl = new QTableWidgetItem(ui->lineEdit_ip_ttl->text().toStdString().c_str());

    QTableWidgetItem* protocol = new QTableWidgetItem(ui->lineEdit_ip_protocol->text().toStdString().c_str());

    QTableWidgetItem* checksum = NULL;
    if (ui->comboBox_ip_checksum->currentIndex() == 0) {
       checksum = new QTableWidgetItem("Calculated");
    }
    else {
       checksum = new QTableWidgetItem("0");
    }

    QTableWidgetItem* src_ip = NULL;
    if (ui->checkBox_ip_rand_src_ip->isChecked() == true) {
        src_ip = new QTableWidgetItem("Random");
    }
    else {
        src_ip =new QTableWidgetItem(ui->lineEdit_ip_src_ip->text().toStdString().c_str());
    }

    QTableWidgetItem* dest_ip = NULL;
    if (ui->checkBox_ip_rand_dest_ip->isChecked() == true) {
        dest_ip = new QTableWidgetItem("Random");
    }
    else {
        dest_ip =new QTableWidgetItem(ui->lineEdit_ip_dest_ip->text().toStdString().c_str());
    }

    QTableWidgetItem* options = new QTableWidgetItem("0");

    ui->layer3_tableWidget->setItem(0, 0, ver);
    ui->layer3_tableWidget->item(0, 0)->setTextAlignment( Qt::AlignCenter );
    ui->layer3_tableWidget->setItem(0, 1, ihl);
    ui->layer3_tableWidget->item(0, 1)->setTextAlignment( Qt::AlignCenter );
    ui->layer3_tableWidget->setItem(0, 2, dscp);
    ui->layer3_tableWidget->item(0, 2)->setTextAlignment( Qt::AlignCenter );
    ui->layer3_tableWidget->setItem(0, 3, ecn);
    ui->layer3_tableWidget->item(0, 3)->setTextAlignment( Qt::AlignCenter );
    ui->layer3_tableWidget->setItem(0, 4, length);
    ui->layer3_tableWidget->item(0, 4)->setTextAlignment( Qt::AlignCenter );
    ui->layer3_tableWidget->setItem(0, 5, id);
    ui->layer3_tableWidget->item(0, 5)->setTextAlignment( Qt::AlignCenter );
    ui->layer3_tableWidget->setItem(0, 6, flags);
    ui->layer3_tableWidget->item(0, 6)->setTextAlignment( Qt::AlignCenter );
    ui->layer3_tableWidget->setItem(2, 0, offset);
    ui->layer3_tableWidget->item(2, 0)->setTextAlignment( Qt::AlignCenter );
    ui->layer3_tableWidget->setItem(2, 1, ttl);
    ui->layer3_tableWidget->item(2, 1)->setTextAlignment( Qt::AlignCenter );
    ui->layer3_tableWidget->setItem(2, 2, protocol);
    ui->layer3_tableWidget->item(2, 2)->setTextAlignment( Qt::AlignCenter );
    ui->layer3_tableWidget->setItem(2, 3, checksum);
    ui->layer3_tableWidget->item(2, 3)->setTextAlignment( Qt::AlignCenter );
    ui->layer3_tableWidget->setItem(2, 4, src_ip);
    ui->layer3_tableWidget->item(2, 4)->setTextAlignment( Qt::AlignCenter );
    ui->layer3_tableWidget->setItem(2, 5, dest_ip);
    ui->layer3_tableWidget->item(2, 5)->setTextAlignment( Qt::AlignCenter );
    ui->layer3_tableWidget->setItem(2, 6, options);
    ui->layer3_tableWidget->item(2, 6)->setTextAlignment( Qt::AlignCenter );

}

void MainWindow::update_table_ip_length(int l4_length, int data_length) {

	int l2_len = 0;

	if (vlan_h != NULL) {
        l2_len = 18;
	} else if (eth_h != NULL) {
        l2_len = 14;
	}

	int len = l2_len + 20 + l4_length + data_length;
	std::string len_str = std::to_string(len);
    QTableWidgetItem* length = new QTableWidgetItem(len_str.c_str());
    length->setTextAlignment( Qt::AlignLeft );
    ui->layer3_tableWidget->setItem(0, 4, length);
    ui->layer3_tableWidget->item(0, 4)->setTextAlignment( Qt::AlignCenter );
}

void MainWindow::fill_tcp_table() {



    QTableWidgetItem* source_port = NULL;
    if (ui->checkBox_tcp_rand_src_port->isChecked() == true) {
        source_port = new QTableWidgetItem("Random");
    }
    else {
            source_port = new QTableWidgetItem(ui->lineEdit_tcp_src_port->text().toStdString().c_str());
    }

    QTableWidgetItem* destination_port = NULL;
    if (ui->checkBox_tcp_rand_dest_port->isChecked() == true) {
        destination_port = new QTableWidgetItem("Random");
    }
    else {
        destination_port = new QTableWidgetItem(ui->lineEdit_tcp_dest_port->text().toStdString().c_str());
    }

    QTableWidgetItem* seq_num = NULL;
    if (ui->checkBox_tcp_rand_seq_number->isChecked() == true) {
        seq_num = new QTableWidgetItem("Random");
    }
    else {
        seq_num = new QTableWidgetItem(ui->lineEdit_tcp_seq_num->text().toStdString().c_str());
    }

    QTableWidgetItem* ack_num = NULL;
    if (ui->checkBox_tcp_rand_ack_num->isChecked() == true) {
        ack_num = new QTableWidgetItem("Random");
    }
    else {
        ack_num = new QTableWidgetItem(ui->lineEdit_tcp_ack_num->text().toStdString().c_str());
    }

    QTableWidgetItem* data_offset = new QTableWidgetItem(ui->lineEdit_tcp_data_offset->text().toStdString().c_str());
    QTableWidgetItem* reserved = new QTableWidgetItem("000");

    QTableWidgetItem* ns = NULL;
    if (ui->checkBox_tcp_ecn_ns->isChecked() == true) {
        ns = new QTableWidgetItem("1");
    }
    else {
        ns = new QTableWidgetItem("0");
    }

    QTableWidgetItem* cwr = NULL;
    if (ui->checkBox_tcp_ecn_cwr->isChecked() == true) {
        cwr = new QTableWidgetItem("1");
    }
    else {
        cwr = new QTableWidgetItem("0");
    }

    QTableWidgetItem* ece = NULL;
    if (ui->checkBox_tcp_ecn_ece->isChecked() == true) {
        ece = new QTableWidgetItem("1");
    }
    else {
        ece = new QTableWidgetItem("0");
    }

    QTableWidgetItem* urg = NULL;
    if (ui->checkBox_tcp_control_bits_urg->isChecked() == true) {
        urg = new QTableWidgetItem("1");
    }
    else {
        urg = new QTableWidgetItem("0");
    }

    QTableWidgetItem* ack = NULL;
    if (ui->checkBox_tcp_control_bits_ack->isChecked() == true) {
        ack = new QTableWidgetItem("1");
    }
    else {
        ack = new QTableWidgetItem("0");
    }

    QTableWidgetItem* psh = NULL;
    if (ui->checkBox_tcp_control_bits_psh->isChecked() == true) {
        psh = new QTableWidgetItem("1");
    }
    else {
        psh = new QTableWidgetItem("0");
    }

    QTableWidgetItem* rst = NULL;
    if (ui->checkBox_tcp_control_bits_rst->isChecked() == true) {
        rst = new QTableWidgetItem("1");
    }
    else {
        rst = new QTableWidgetItem("0");
    }

    QTableWidgetItem* syn = NULL;
    if (ui->checkBox_tcp_control_bits_syn->isChecked() == true) {
        syn = new QTableWidgetItem("1");
    }
    else {
        syn = new QTableWidgetItem("0");
    }

    QTableWidgetItem* fin = NULL;
    if (ui->checkBox_tcp_control_bits_fin->isChecked() == true) {
        fin = new QTableWidgetItem("1");
    }
    else {
        fin = new QTableWidgetItem("0");
    }

    QTableWidgetItem* window_size = new QTableWidgetItem(ui->lineEdit_tcp_window_size->text().toStdString().c_str());

    QTableWidgetItem* checksum = NULL;

    if (ui->comboBox_tcp_checksum->currentIndex() == 0) {
       checksum = new QTableWidgetItem("Calculated");
    }
    else {
       checksum = new QTableWidgetItem("0");
    }

    QTableWidgetItem* urg_pointer = new QTableWidgetItem(ui->lineEdit_tcp_urgent_pointer->text().toStdString().c_str());
    QTableWidgetItem* options = new QTableWidgetItem("0");
    QTableWidgetItem* data = new QTableWidgetItem(ui->lineEdit_tcp_data->text().toStdString().c_str());


    ui->layer4_tableWidget->setItem(0, 0, source_port);
    ui->layer4_tableWidget->item(0, 0)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(0, 1, destination_port);
    ui->layer4_tableWidget->item(0, 1)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(0, 2, seq_num);
    ui->layer4_tableWidget->item(0, 2)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(0, 3, ack_num);
    ui->layer4_tableWidget->item(0, 3)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(0, 4, data_offset);
    ui->layer4_tableWidget->item(0, 4)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(0, 5, reserved);
    ui->layer4_tableWidget->item(0, 5)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(2, 0, ns);
    ui->layer4_tableWidget->item(2, 0)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(2, 1, cwr);
    ui->layer4_tableWidget->item(2, 1)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(2, 2, ece);
    ui->layer4_tableWidget->item(2, 2)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->item(2, 3)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->item(2, 4)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->item(2, 5)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(4, 0, urg);
    ui->layer4_tableWidget->item(4, 0)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(4, 1, ack);
    ui->layer4_tableWidget->item(4, 1)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(4, 2, psh);
    ui->layer4_tableWidget->item(4, 2)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(4, 3, rst);
    ui->layer4_tableWidget->item(4, 3)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(4, 4, syn);
    ui->layer4_tableWidget->item(4, 4)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(4, 5, fin);
    ui->layer4_tableWidget->item(4, 5)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(6, 0, window_size);
    ui->layer4_tableWidget->item(6, 0)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(6, 1, checksum);
    ui->layer4_tableWidget->item(6, 1)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(6, 2, urg_pointer);
    ui->layer4_tableWidget->item(6, 2)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(6, 3, options);
    ui->layer4_tableWidget->item(6, 3)->setTextAlignment( Qt::AlignCenter );
    ui->layer4_tableWidget->setItem(6, 4, data);
    ui->layer4_tableWidget->item(6, 4)->setTextAlignment( Qt::AlignCenter );
    //ui->layer4_tableWidget->item(6, 5)->setTextAlignment( Qt::AlignCenter );

}
void MainWindow::setValidators() {

	//TODO VALIDATORS!
	//dopisać walidator dla IP
	/*  QRegExp rx("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
	 QRegExpValidator regValidator(rx, 0);
	 ui->lineEdit_ip_src_ip->setValidator(&regValidator);
	 ui->lineEdit_ip_dest_ip->setValidator(&regValidator);
	 */

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
    ui->lineEdit_tcp_window_size->setValidator(valid0to65535);
	ui->lineEdit_tcp_urgent_pointer->setValidator(valid0to65535);
}
