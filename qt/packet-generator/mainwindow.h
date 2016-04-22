#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include <QValidator>
#include <QTableWidget>
#include "includes.h"

namespace Ui {
class MainWindow;
}

class MainWindow: public QMainWindow {
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	bool* setFlags();
	void clean_table(QTableWidget *table);

private slots:
	void on_SaveL2Button_clicked();
	void on_SaveL3Button_clicked();
	void on_SaveL4Button_clicked();
	void on_checkBox_eth_vlan_toggled(bool checked);
	void on_SendButton_clicked();
	void randomize(bool* flags);
	void on_checkBox_ip_create_toggled(bool checked);
	void on_checkbox_TCP_create_toggled(bool checked);
	void on_checkbox_UDP_create_toggled(bool checked);
	void on_packages_to_send_lineEdit_textEdited(const QString &arg1);
	void fill_eth_table();
	void fill_ip_table(int data_length);
	void table_update_ip_length(int l4_length, int data_length);
	void setValidators();

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
	QElapsedTimer timer;

	struct ifaddrs *addrs = NULL;
	struct ifaddrs *ipa = NULL;
	QIntValidator *valid0to7;
	QIntValidator *valid0and1;
	QIntValidator *valid0to4095;
	QIntValidator *valid0to63;
	QIntValidator *valid0to2;
	QIntValidator *valid0to65535;
	QIntValidator *valid0to8191;
	QIntValidator *valid0to255;
	QIntValidator *valid0to15;
	QDoubleValidator *valid_double;
};

#endif // MAINWINDOW_H
