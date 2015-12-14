/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *Main;
    QGroupBox *groupBox_3;
    QPushButton *SendButton;
    QListView *SendList;
    QWidget *Layer2;
    QGroupBox *groupBox_layer2;
    QGridLayout *gridLayout_2;
    QLineEdit *lineEdit_eth_dest_mac;
    QLabel *label_eth_sfd;
    QLineEdit *lineEdit_eth_crc;
    QLineEdit *lineEdit_eth_preamble;
    QFrame *line_2;
    QLabel *label_eth_frame_type;
    QCheckBox *checkBox_eth_rand_dest_mac;
    QLineEdit *lineEdit_eth_dei;
    QLineEdit *lineEdit_eth_pcp;
    QCheckBox *checkBox_eth_rand_dei;
    QLineEdit *lineEdit_eth_src_mac;
    QCheckBox *checkBox_eth_rand_vid;
    QCheckBox *checkBox_eth_rand_src_mac;
    QFrame *line;
    QLineEdit *lineEdit_eth_vid;
    QCheckBox *checkBox_eth_vlan;
    QLabel *label_eth_preamble;
    QLabel *label_eth_crc;
    QLabel *label_eth_vid;
    QLabel *label_eth_dest_mac;
    QLabel *label_eth_src_mac;
    QLineEdit *lineEdit_eth_data;
    QLabel *label_eth_dei;
    QLabel *label_eth_data;
    QLineEdit *lineEdit_eth_frame_type;
    QLabel *label_eth_pcp;
    QCheckBox *checkBox_eth_rand_pcp;
    QSpacerItem *horizontalSpacer_16;
    QSpacerItem *horizontalSpacer_15;
    QLineEdit *lineEdit_eth_sfd;
    QPushButton *SaveL2Button;
    QWidget *Layer3;
    QGroupBox *groupBox_layer3;
    QGridLayout *gridLayout_4;
    QLineEdit *lineEdit_ip_ttl;
    QLabel *label_ip_total_length;
    QLineEdit *lineEdit_ip_total_length;
    QLabel *label_ip_flags;
    QLabel *label_ip_dest_ip;
    QLabel *label_ip_frag_offset;
    QLabel *label_ip_dscp;
    QLabel *label_ip_ihl;
    QLineEdit *lineEdit_ip_offset;
    QLineEdit *lineEdit_ip_ihl;
    QLabel *label_ip_protocol;
    QLabel *label_ip_version;
    QLabel *label_ip_options;
    QLineEdit *lineEdit_ip_options;
    QLabel *label_ip_ttl;
    QLineEdit *lineEdit_ip_protocol;
    QLabel *label_ip_id;
    QLineEdit *lineEdit_ip_version;
    QLabel *label_ip_ecn;
    QCheckBox *checkBox_ip_rand_id;
    QCheckBox *checkBox_ip_rand_src_ip;
    QLineEdit *lineEdit_ip_src_ip;
    QCheckBox *checkBox_ip_rand_ttl;
    QLineEdit *lineEdit_ip_checksum;
    QLineEdit *lineEdit_ip_flags;
    QLineEdit *lineEdit_ip_ecn;
    QLineEdit *lineEdit_ip_dscp;
    QLineEdit *lineEdit_ip_dest_ip;
    QLineEdit *lineEdit_ip_id;
    QCheckBox *checkBox_ip_rand_dest_ip;
    QLabel *label_ip_checksum;
    QLabel *label_ip_src_ip;
    QPushButton *pushButton_4;
    QWidget *Layer4;
    QGroupBox *groupBox_layer4;
    QGridLayout *gridLayout_3;
    QCheckBox *checkBox;
    QGroupBox *groupBox;
    QCheckBox *checkBox_2;
    QGroupBox *groupBox_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1024, 768);
        MainWindow->setMinimumSize(QSize(640, 480));
        MainWindow->setBaseSize(QSize(1280, 1024));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QFont font;
        font.setItalic(false);
        font.setStrikeOut(false);
        tabWidget->setFont(font);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(false);
        Main = new QWidget();
        Main->setObjectName(QStringLiteral("Main"));
        groupBox_3 = new QGroupBox(Main);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(-40, 0, 1001, 651));
        SendButton = new QPushButton(groupBox_3);
        SendButton->setObjectName(QStringLiteral("SendButton"));
        SendButton->setGeometry(QRect(750, 210, 85, 27));
        SendList = new QListView(groupBox_3);
        SendList->setObjectName(QStringLiteral("SendList"));
        SendList->setGeometry(QRect(70, 30, 256, 192));
        tabWidget->addTab(Main, QString());
        Layer2 = new QWidget();
        Layer2->setObjectName(QStringLiteral("Layer2"));
        groupBox_layer2 = new QGroupBox(Layer2);
        groupBox_layer2->setObjectName(QStringLiteral("groupBox_layer2"));
        groupBox_layer2->setGeometry(QRect(0, 0, 1001, 461));
        gridLayout_2 = new QGridLayout(groupBox_layer2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        lineEdit_eth_dest_mac = new QLineEdit(groupBox_layer2);
        lineEdit_eth_dest_mac->setObjectName(QStringLiteral("lineEdit_eth_dest_mac"));
        lineEdit_eth_dest_mac->setClearButtonEnabled(true);

        gridLayout_2->addWidget(lineEdit_eth_dest_mac, 5, 1, 1, 1);

        label_eth_sfd = new QLabel(groupBox_layer2);
        label_eth_sfd->setObjectName(QStringLiteral("label_eth_sfd"));

        gridLayout_2->addWidget(label_eth_sfd, 1, 0, 1, 1);

        lineEdit_eth_crc = new QLineEdit(groupBox_layer2);
        lineEdit_eth_crc->setObjectName(QStringLiteral("lineEdit_eth_crc"));
        lineEdit_eth_crc->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_eth_crc, 14, 1, 1, 1);

        lineEdit_eth_preamble = new QLineEdit(groupBox_layer2);
        lineEdit_eth_preamble->setObjectName(QStringLiteral("lineEdit_eth_preamble"));
        lineEdit_eth_preamble->setEnabled(false);
        lineEdit_eth_preamble->setReadOnly(true);
        lineEdit_eth_preamble->setClearButtonEnabled(false);

        gridLayout_2->addWidget(lineEdit_eth_preamble, 0, 1, 1, 1);

        line_2 = new QFrame(groupBox_layer2);
        line_2->setObjectName(QStringLiteral("line_2"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(line_2->sizePolicy().hasHeightForWidth());
        line_2->setSizePolicy(sizePolicy);
        line_2->setLineWidth(2);
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_2, 11, 0, 1, 3);

        label_eth_frame_type = new QLabel(groupBox_layer2);
        label_eth_frame_type->setObjectName(QStringLiteral("label_eth_frame_type"));

        gridLayout_2->addWidget(label_eth_frame_type, 12, 0, 1, 1);

        checkBox_eth_rand_dest_mac = new QCheckBox(groupBox_layer2);
        checkBox_eth_rand_dest_mac->setObjectName(QStringLiteral("checkBox_eth_rand_dest_mac"));

        gridLayout_2->addWidget(checkBox_eth_rand_dest_mac, 5, 2, 1, 1);

        lineEdit_eth_dei = new QLineEdit(groupBox_layer2);
        lineEdit_eth_dei->setObjectName(QStringLiteral("lineEdit_eth_dei"));
        lineEdit_eth_dei->setEnabled(false);
        lineEdit_eth_dei->setInputMethodHints(Qt::ImhDigitsOnly);
        lineEdit_eth_dei->setClearButtonEnabled(true);

        gridLayout_2->addWidget(lineEdit_eth_dei, 9, 1, 1, 1);

        lineEdit_eth_pcp = new QLineEdit(groupBox_layer2);
        lineEdit_eth_pcp->setObjectName(QStringLiteral("lineEdit_eth_pcp"));
        lineEdit_eth_pcp->setEnabled(false);
        lineEdit_eth_pcp->setInputMethodHints(Qt::ImhDigitsOnly);
        lineEdit_eth_pcp->setClearButtonEnabled(true);

        gridLayout_2->addWidget(lineEdit_eth_pcp, 8, 1, 1, 1);

        checkBox_eth_rand_dei = new QCheckBox(groupBox_layer2);
        checkBox_eth_rand_dei->setObjectName(QStringLiteral("checkBox_eth_rand_dei"));
        checkBox_eth_rand_dei->setEnabled(false);

        gridLayout_2->addWidget(checkBox_eth_rand_dei, 9, 2, 1, 1);

        lineEdit_eth_src_mac = new QLineEdit(groupBox_layer2);
        lineEdit_eth_src_mac->setObjectName(QStringLiteral("lineEdit_eth_src_mac"));
        lineEdit_eth_src_mac->setClearButtonEnabled(true);

        gridLayout_2->addWidget(lineEdit_eth_src_mac, 2, 1, 1, 1);

        checkBox_eth_rand_vid = new QCheckBox(groupBox_layer2);
        checkBox_eth_rand_vid->setObjectName(QStringLiteral("checkBox_eth_rand_vid"));
        checkBox_eth_rand_vid->setEnabled(false);

        gridLayout_2->addWidget(checkBox_eth_rand_vid, 10, 2, 1, 1);

        checkBox_eth_rand_src_mac = new QCheckBox(groupBox_layer2);
        checkBox_eth_rand_src_mac->setObjectName(QStringLiteral("checkBox_eth_rand_src_mac"));

        gridLayout_2->addWidget(checkBox_eth_rand_src_mac, 2, 2, 1, 1);

        line = new QFrame(groupBox_layer2);
        line->setObjectName(QStringLiteral("line"));
        line->setLineWidth(2);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line, 6, 0, 1, 3);

        lineEdit_eth_vid = new QLineEdit(groupBox_layer2);
        lineEdit_eth_vid->setObjectName(QStringLiteral("lineEdit_eth_vid"));
        lineEdit_eth_vid->setEnabled(false);
        lineEdit_eth_vid->setInputMethodHints(Qt::ImhDigitsOnly);
        lineEdit_eth_vid->setClearButtonEnabled(true);

        gridLayout_2->addWidget(lineEdit_eth_vid, 10, 1, 1, 1);

        checkBox_eth_vlan = new QCheckBox(groupBox_layer2);
        checkBox_eth_vlan->setObjectName(QStringLiteral("checkBox_eth_vlan"));

        gridLayout_2->addWidget(checkBox_eth_vlan, 7, 0, 1, 1);

        label_eth_preamble = new QLabel(groupBox_layer2);
        label_eth_preamble->setObjectName(QStringLiteral("label_eth_preamble"));

        gridLayout_2->addWidget(label_eth_preamble, 0, 0, 1, 1);

        label_eth_crc = new QLabel(groupBox_layer2);
        label_eth_crc->setObjectName(QStringLiteral("label_eth_crc"));

        gridLayout_2->addWidget(label_eth_crc, 14, 0, 1, 1);

        label_eth_vid = new QLabel(groupBox_layer2);
        label_eth_vid->setObjectName(QStringLiteral("label_eth_vid"));

        gridLayout_2->addWidget(label_eth_vid, 10, 0, 1, 1);

        label_eth_dest_mac = new QLabel(groupBox_layer2);
        label_eth_dest_mac->setObjectName(QStringLiteral("label_eth_dest_mac"));

        gridLayout_2->addWidget(label_eth_dest_mac, 5, 0, 1, 1);

        label_eth_src_mac = new QLabel(groupBox_layer2);
        label_eth_src_mac->setObjectName(QStringLiteral("label_eth_src_mac"));

        gridLayout_2->addWidget(label_eth_src_mac, 2, 0, 1, 1);

        lineEdit_eth_data = new QLineEdit(groupBox_layer2);
        lineEdit_eth_data->setObjectName(QStringLiteral("lineEdit_eth_data"));
        lineEdit_eth_data->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_eth_data, 13, 1, 1, 1);

        label_eth_dei = new QLabel(groupBox_layer2);
        label_eth_dei->setObjectName(QStringLiteral("label_eth_dei"));

        gridLayout_2->addWidget(label_eth_dei, 9, 0, 1, 1);

        label_eth_data = new QLabel(groupBox_layer2);
        label_eth_data->setObjectName(QStringLiteral("label_eth_data"));

        gridLayout_2->addWidget(label_eth_data, 13, 0, 1, 1);

        lineEdit_eth_frame_type = new QLineEdit(groupBox_layer2);
        lineEdit_eth_frame_type->setObjectName(QStringLiteral("lineEdit_eth_frame_type"));
        lineEdit_eth_frame_type->setEnabled(false);
        lineEdit_eth_frame_type->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_eth_frame_type, 12, 1, 1, 1);

        label_eth_pcp = new QLabel(groupBox_layer2);
        label_eth_pcp->setObjectName(QStringLiteral("label_eth_pcp"));

        gridLayout_2->addWidget(label_eth_pcp, 8, 0, 1, 1);

        checkBox_eth_rand_pcp = new QCheckBox(groupBox_layer2);
        checkBox_eth_rand_pcp->setObjectName(QStringLiteral("checkBox_eth_rand_pcp"));
        checkBox_eth_rand_pcp->setEnabled(false);

        gridLayout_2->addWidget(checkBox_eth_rand_pcp, 8, 2, 1, 1);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_16, 1, 2, 1, 1);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_15, 0, 2, 1, 1);

        lineEdit_eth_sfd = new QLineEdit(groupBox_layer2);
        lineEdit_eth_sfd->setObjectName(QStringLiteral("lineEdit_eth_sfd"));
        lineEdit_eth_sfd->setEnabled(false);
        lineEdit_eth_sfd->setAcceptDrops(true);
        lineEdit_eth_sfd->setAutoFillBackground(false);
        lineEdit_eth_sfd->setReadOnly(true);
        lineEdit_eth_sfd->setClearButtonEnabled(false);

        gridLayout_2->addWidget(lineEdit_eth_sfd, 1, 1, 1, 1);

        SaveL2Button = new QPushButton(Layer2);
        SaveL2Button->setObjectName(QStringLiteral("SaveL2Button"));
        SaveL2Button->setGeometry(QRect(300, 470, 403, 71));
        tabWidget->addTab(Layer2, QString());
        Layer3 = new QWidget();
        Layer3->setObjectName(QStringLiteral("Layer3"));
        groupBox_layer3 = new QGroupBox(Layer3);
        groupBox_layer3->setObjectName(QStringLiteral("groupBox_layer3"));
        groupBox_layer3->setGeometry(QRect(0, 0, 1001, 591));
        gridLayout_4 = new QGridLayout(groupBox_layer3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        lineEdit_ip_ttl = new QLineEdit(groupBox_layer3);
        lineEdit_ip_ttl->setObjectName(QStringLiteral("lineEdit_ip_ttl"));
        lineEdit_ip_ttl->setInputMethodHints(Qt::ImhDigitsOnly);
        lineEdit_ip_ttl->setClearButtonEnabled(true);

        gridLayout_4->addWidget(lineEdit_ip_ttl, 8, 1, 1, 1);

        label_ip_total_length = new QLabel(groupBox_layer3);
        label_ip_total_length->setObjectName(QStringLiteral("label_ip_total_length"));

        gridLayout_4->addWidget(label_ip_total_length, 4, 0, 1, 1);

        lineEdit_ip_total_length = new QLineEdit(groupBox_layer3);
        lineEdit_ip_total_length->setObjectName(QStringLiteral("lineEdit_ip_total_length"));
        lineEdit_ip_total_length->setInputMethodHints(Qt::ImhDigitsOnly);
        lineEdit_ip_total_length->setClearButtonEnabled(true);

        gridLayout_4->addWidget(lineEdit_ip_total_length, 4, 1, 1, 1);

        label_ip_flags = new QLabel(groupBox_layer3);
        label_ip_flags->setObjectName(QStringLiteral("label_ip_flags"));

        gridLayout_4->addWidget(label_ip_flags, 6, 0, 1, 1);

        label_ip_dest_ip = new QLabel(groupBox_layer3);
        label_ip_dest_ip->setObjectName(QStringLiteral("label_ip_dest_ip"));

        gridLayout_4->addWidget(label_ip_dest_ip, 12, 0, 1, 1);

        label_ip_frag_offset = new QLabel(groupBox_layer3);
        label_ip_frag_offset->setObjectName(QStringLiteral("label_ip_frag_offset"));

        gridLayout_4->addWidget(label_ip_frag_offset, 7, 0, 1, 1);

        label_ip_dscp = new QLabel(groupBox_layer3);
        label_ip_dscp->setObjectName(QStringLiteral("label_ip_dscp"));

        gridLayout_4->addWidget(label_ip_dscp, 2, 0, 1, 1);

        label_ip_ihl = new QLabel(groupBox_layer3);
        label_ip_ihl->setObjectName(QStringLiteral("label_ip_ihl"));

        gridLayout_4->addWidget(label_ip_ihl, 1, 0, 1, 1);

        lineEdit_ip_offset = new QLineEdit(groupBox_layer3);
        lineEdit_ip_offset->setObjectName(QStringLiteral("lineEdit_ip_offset"));
        lineEdit_ip_offset->setInputMethodHints(Qt::ImhDigitsOnly);
        lineEdit_ip_offset->setClearButtonEnabled(true);

        gridLayout_4->addWidget(lineEdit_ip_offset, 7, 1, 1, 1);

        lineEdit_ip_ihl = new QLineEdit(groupBox_layer3);
        lineEdit_ip_ihl->setObjectName(QStringLiteral("lineEdit_ip_ihl"));
        lineEdit_ip_ihl->setEnabled(false);

        gridLayout_4->addWidget(lineEdit_ip_ihl, 1, 1, 1, 1);

        label_ip_protocol = new QLabel(groupBox_layer3);
        label_ip_protocol->setObjectName(QStringLiteral("label_ip_protocol"));

        gridLayout_4->addWidget(label_ip_protocol, 9, 0, 1, 1);

        label_ip_version = new QLabel(groupBox_layer3);
        label_ip_version->setObjectName(QStringLiteral("label_ip_version"));

        gridLayout_4->addWidget(label_ip_version, 0, 0, 1, 1);

        label_ip_options = new QLabel(groupBox_layer3);
        label_ip_options->setObjectName(QStringLiteral("label_ip_options"));

        gridLayout_4->addWidget(label_ip_options, 13, 0, 1, 1);

        lineEdit_ip_options = new QLineEdit(groupBox_layer3);
        lineEdit_ip_options->setObjectName(QStringLiteral("lineEdit_ip_options"));
        lineEdit_ip_options->setClearButtonEnabled(true);

        gridLayout_4->addWidget(lineEdit_ip_options, 13, 1, 1, 1);

        label_ip_ttl = new QLabel(groupBox_layer3);
        label_ip_ttl->setObjectName(QStringLiteral("label_ip_ttl"));

        gridLayout_4->addWidget(label_ip_ttl, 8, 0, 1, 1);

        lineEdit_ip_protocol = new QLineEdit(groupBox_layer3);
        lineEdit_ip_protocol->setObjectName(QStringLiteral("lineEdit_ip_protocol"));
        lineEdit_ip_protocol->setClearButtonEnabled(true);

        gridLayout_4->addWidget(lineEdit_ip_protocol, 9, 1, 1, 1);

        label_ip_id = new QLabel(groupBox_layer3);
        label_ip_id->setObjectName(QStringLiteral("label_ip_id"));

        gridLayout_4->addWidget(label_ip_id, 5, 0, 1, 1);

        lineEdit_ip_version = new QLineEdit(groupBox_layer3);
        lineEdit_ip_version->setObjectName(QStringLiteral("lineEdit_ip_version"));
        lineEdit_ip_version->setEnabled(false);
        lineEdit_ip_version->setReadOnly(true);

        gridLayout_4->addWidget(lineEdit_ip_version, 0, 1, 1, 1);

        label_ip_ecn = new QLabel(groupBox_layer3);
        label_ip_ecn->setObjectName(QStringLiteral("label_ip_ecn"));

        gridLayout_4->addWidget(label_ip_ecn, 3, 0, 1, 1);

        checkBox_ip_rand_id = new QCheckBox(groupBox_layer3);
        checkBox_ip_rand_id->setObjectName(QStringLiteral("checkBox_ip_rand_id"));

        gridLayout_4->addWidget(checkBox_ip_rand_id, 5, 2, 1, 1);

        checkBox_ip_rand_src_ip = new QCheckBox(groupBox_layer3);
        checkBox_ip_rand_src_ip->setObjectName(QStringLiteral("checkBox_ip_rand_src_ip"));

        gridLayout_4->addWidget(checkBox_ip_rand_src_ip, 11, 2, 1, 1);

        lineEdit_ip_src_ip = new QLineEdit(groupBox_layer3);
        lineEdit_ip_src_ip->setObjectName(QStringLiteral("lineEdit_ip_src_ip"));
        lineEdit_ip_src_ip->setClearButtonEnabled(true);

        gridLayout_4->addWidget(lineEdit_ip_src_ip, 11, 1, 1, 1);

        checkBox_ip_rand_ttl = new QCheckBox(groupBox_layer3);
        checkBox_ip_rand_ttl->setObjectName(QStringLiteral("checkBox_ip_rand_ttl"));

        gridLayout_4->addWidget(checkBox_ip_rand_ttl, 8, 2, 1, 1);

        lineEdit_ip_checksum = new QLineEdit(groupBox_layer3);
        lineEdit_ip_checksum->setObjectName(QStringLiteral("lineEdit_ip_checksum"));
        lineEdit_ip_checksum->setClearButtonEnabled(true);

        gridLayout_4->addWidget(lineEdit_ip_checksum, 10, 1, 1, 1);

        lineEdit_ip_flags = new QLineEdit(groupBox_layer3);
        lineEdit_ip_flags->setObjectName(QStringLiteral("lineEdit_ip_flags"));
        lineEdit_ip_flags->setClearButtonEnabled(true);

        gridLayout_4->addWidget(lineEdit_ip_flags, 6, 1, 1, 1);

        lineEdit_ip_ecn = new QLineEdit(groupBox_layer3);
        lineEdit_ip_ecn->setObjectName(QStringLiteral("lineEdit_ip_ecn"));
        lineEdit_ip_ecn->setEnabled(true);
        lineEdit_ip_ecn->setInputMethodHints(Qt::ImhDigitsOnly);
        lineEdit_ip_ecn->setClearButtonEnabled(true);

        gridLayout_4->addWidget(lineEdit_ip_ecn, 3, 1, 1, 1);

        lineEdit_ip_dscp = new QLineEdit(groupBox_layer3);
        lineEdit_ip_dscp->setObjectName(QStringLiteral("lineEdit_ip_dscp"));
        lineEdit_ip_dscp->setInputMethodHints(Qt::ImhDigitsOnly);
        lineEdit_ip_dscp->setClearButtonEnabled(true);

        gridLayout_4->addWidget(lineEdit_ip_dscp, 2, 1, 1, 1);

        lineEdit_ip_dest_ip = new QLineEdit(groupBox_layer3);
        lineEdit_ip_dest_ip->setObjectName(QStringLiteral("lineEdit_ip_dest_ip"));
        lineEdit_ip_dest_ip->setClearButtonEnabled(true);

        gridLayout_4->addWidget(lineEdit_ip_dest_ip, 12, 1, 1, 1);

        lineEdit_ip_id = new QLineEdit(groupBox_layer3);
        lineEdit_ip_id->setObjectName(QStringLiteral("lineEdit_ip_id"));
        lineEdit_ip_id->setInputMethodHints(Qt::ImhDigitsOnly);
        lineEdit_ip_id->setClearButtonEnabled(true);

        gridLayout_4->addWidget(lineEdit_ip_id, 5, 1, 1, 1);

        checkBox_ip_rand_dest_ip = new QCheckBox(groupBox_layer3);
        checkBox_ip_rand_dest_ip->setObjectName(QStringLiteral("checkBox_ip_rand_dest_ip"));

        gridLayout_4->addWidget(checkBox_ip_rand_dest_ip, 12, 2, 1, 1);

        label_ip_checksum = new QLabel(groupBox_layer3);
        label_ip_checksum->setObjectName(QStringLiteral("label_ip_checksum"));

        gridLayout_4->addWidget(label_ip_checksum, 10, 0, 1, 1);

        label_ip_src_ip = new QLabel(groupBox_layer3);
        label_ip_src_ip->setObjectName(QStringLiteral("label_ip_src_ip"));

        gridLayout_4->addWidget(label_ip_src_ip, 11, 0, 1, 1);

        pushButton_4 = new QPushButton(groupBox_layer3);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        gridLayout_4->addWidget(pushButton_4, 14, 2, 1, 1);

        tabWidget->addTab(Layer3, QString());
        Layer4 = new QWidget();
        Layer4->setObjectName(QStringLiteral("Layer4"));
        groupBox_layer4 = new QGroupBox(Layer4);
        groupBox_layer4->setObjectName(QStringLiteral("groupBox_layer4"));
        groupBox_layer4->setGeometry(QRect(0, 0, 204, 92));
        gridLayout_3 = new QGridLayout(groupBox_layer4);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        checkBox = new QCheckBox(groupBox_layer4);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        gridLayout_3->addWidget(checkBox, 0, 0, 1, 1);

        groupBox = new QGroupBox(groupBox_layer4);
        groupBox->setObjectName(QStringLiteral("groupBox"));

        gridLayout_3->addWidget(groupBox, 1, 0, 1, 1);

        checkBox_2 = new QCheckBox(groupBox_layer4);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        gridLayout_3->addWidget(checkBox_2, 0, 1, 1, 1);

        groupBox_2 = new QGroupBox(groupBox_layer4);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));

        gridLayout_3->addWidget(groupBox_2, 1, 1, 1, 1);

        tabWidget->addTab(Layer4, QString());

        gridLayout->addWidget(tabWidget, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 27));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(tabWidget, lineEdit_eth_preamble);
        QWidget::setTabOrder(lineEdit_eth_preamble, lineEdit_eth_sfd);
        QWidget::setTabOrder(lineEdit_eth_sfd, lineEdit_eth_src_mac);
        QWidget::setTabOrder(lineEdit_eth_src_mac, lineEdit_eth_dest_mac);
        QWidget::setTabOrder(lineEdit_eth_dest_mac, checkBox_eth_rand_src_mac);
        QWidget::setTabOrder(checkBox_eth_rand_src_mac, checkBox_eth_rand_dest_mac);
        QWidget::setTabOrder(checkBox_eth_rand_dest_mac, checkBox_eth_vlan);
        QWidget::setTabOrder(checkBox_eth_vlan, lineEdit_eth_pcp);
        QWidget::setTabOrder(lineEdit_eth_pcp, checkBox_eth_rand_pcp);
        QWidget::setTabOrder(checkBox_eth_rand_pcp, lineEdit_eth_dei);
        QWidget::setTabOrder(lineEdit_eth_dei, checkBox_eth_rand_dei);
        QWidget::setTabOrder(checkBox_eth_rand_dei, lineEdit_eth_vid);
        QWidget::setTabOrder(lineEdit_eth_vid, checkBox_eth_rand_vid);
        QWidget::setTabOrder(checkBox_eth_rand_vid, lineEdit_eth_frame_type);
        QWidget::setTabOrder(lineEdit_eth_frame_type, lineEdit_eth_data);
        QWidget::setTabOrder(lineEdit_eth_data, lineEdit_eth_crc);
        QWidget::setTabOrder(lineEdit_eth_crc, lineEdit_ip_version);
        QWidget::setTabOrder(lineEdit_ip_version, lineEdit_ip_ihl);
        QWidget::setTabOrder(lineEdit_ip_ihl, lineEdit_ip_dscp);
        QWidget::setTabOrder(lineEdit_ip_dscp, lineEdit_ip_ecn);
        QWidget::setTabOrder(lineEdit_ip_ecn, lineEdit_ip_total_length);
        QWidget::setTabOrder(lineEdit_ip_total_length, lineEdit_ip_id);
        QWidget::setTabOrder(lineEdit_ip_id, checkBox_ip_rand_id);
        QWidget::setTabOrder(checkBox_ip_rand_id, lineEdit_ip_flags);
        QWidget::setTabOrder(lineEdit_ip_flags, lineEdit_ip_offset);
        QWidget::setTabOrder(lineEdit_ip_offset, lineEdit_ip_ttl);
        QWidget::setTabOrder(lineEdit_ip_ttl, checkBox_ip_rand_ttl);
        QWidget::setTabOrder(checkBox_ip_rand_ttl, lineEdit_ip_protocol);
        QWidget::setTabOrder(lineEdit_ip_protocol, lineEdit_ip_checksum);
        QWidget::setTabOrder(lineEdit_ip_checksum, lineEdit_ip_src_ip);
        QWidget::setTabOrder(lineEdit_ip_src_ip, checkBox_ip_rand_src_ip);
        QWidget::setTabOrder(checkBox_ip_rand_src_ip, lineEdit_ip_dest_ip);
        QWidget::setTabOrder(lineEdit_ip_dest_ip, checkBox_ip_rand_dest_ip);
        QWidget::setTabOrder(checkBox_ip_rand_dest_ip, lineEdit_ip_options);
        QWidget::setTabOrder(lineEdit_ip_options, pushButton_4);
        QWidget::setTabOrder(pushButton_4, checkBox_2);
        QWidget::setTabOrder(checkBox_2, checkBox);

        retranslateUi(MainWindow);
        QObject::connect(checkBox_eth_vlan, SIGNAL(toggled(bool)), lineEdit_eth_vid, SLOT(setEnabled(bool)));
        QObject::connect(checkBox_eth_vlan, SIGNAL(toggled(bool)), lineEdit_eth_pcp, SLOT(setEnabled(bool)));
        QObject::connect(checkBox_eth_vlan, SIGNAL(toggled(bool)), lineEdit_eth_dei, SLOT(setEnabled(bool)));
        QObject::connect(checkBox_eth_vlan, SIGNAL(toggled(bool)), checkBox_eth_rand_pcp, SLOT(setEnabled(bool)));
        QObject::connect(checkBox_eth_vlan, SIGNAL(toggled(bool)), checkBox_eth_rand_dei, SLOT(setEnabled(bool)));
        QObject::connect(checkBox_eth_vlan, SIGNAL(toggled(bool)), checkBox_eth_rand_vid, SLOT(setEnabled(bool)));
        QObject::connect(checkBox_eth_rand_src_mac, SIGNAL(toggled(bool)), lineEdit_eth_src_mac, SLOT(setDisabled(bool)));
        QObject::connect(checkBox_eth_rand_dest_mac, SIGNAL(toggled(bool)), lineEdit_eth_dest_mac, SLOT(setDisabled(bool)));
        QObject::connect(checkBox_ip_rand_id, SIGNAL(toggled(bool)), lineEdit_ip_id, SLOT(setDisabled(bool)));
        QObject::connect(checkBox_ip_rand_ttl, SIGNAL(toggled(bool)), lineEdit_ip_ttl, SLOT(setDisabled(bool)));
        QObject::connect(checkBox_ip_rand_src_ip, SIGNAL(toggled(bool)), lineEdit_ip_src_ip, SLOT(setDisabled(bool)));
        QObject::connect(checkBox_ip_rand_dest_ip, SIGNAL(toggled(bool)), lineEdit_ip_dest_ip, SLOT(setDisabled(bool)));
        QObject::connect(checkBox_eth_rand_vid, SIGNAL(toggled(bool)), lineEdit_eth_vid, SLOT(setDisabled(bool)));
        QObject::connect(checkBox_eth_rand_dei, SIGNAL(toggled(bool)), lineEdit_eth_dei, SLOT(setDisabled(bool)));
        QObject::connect(checkBox_eth_rand_pcp, SIGNAL(toggled(bool)), lineEdit_eth_pcp, SLOT(setDisabled(bool)));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Packet-generator", 0));
#ifndef QT_NO_ACCESSIBILITY
        Main->setAccessibleName(QApplication::translate("MainWindow", "Main", 0));
#endif // QT_NO_ACCESSIBILITY
        groupBox_3->setTitle(QString());
        SendButton->setText(QApplication::translate("MainWindow", "Send", 0));
        tabWidget->setTabText(tabWidget->indexOf(Main), QApplication::translate("MainWindow", "Main", 0));
        groupBox_layer2->setTitle(QApplication::translate("MainWindow", "Ethernet Header", 0));
        lineEdit_eth_dest_mac->setInputMask(QApplication::translate("MainWindow", "HH:HH:HH:HH:HH:HH;_", 0));
        label_eth_sfd->setText(QApplication::translate("MainWindow", "SFD:", 0));
        lineEdit_eth_preamble->setText(QApplication::translate("MainWindow", "AAAAAAAA", 0));
        label_eth_frame_type->setText(QApplication::translate("MainWindow", "Frame type:", 0));
        checkBox_eth_rand_dest_mac->setText(QApplication::translate("MainWindow", "Random", 0));
        checkBox_eth_rand_dei->setText(QApplication::translate("MainWindow", "Random", 0));
        lineEdit_eth_src_mac->setInputMask(QApplication::translate("MainWindow", "HH:HH:HH:HH:HH:HH;_", 0));
        lineEdit_eth_src_mac->setPlaceholderText(QApplication::translate("MainWindow", "Type source MAC address here...", 0));
        checkBox_eth_rand_vid->setText(QApplication::translate("MainWindow", "Random", 0));
        checkBox_eth_rand_src_mac->setText(QApplication::translate("MainWindow", "Random", 0));
        checkBox_eth_vlan->setText(QApplication::translate("MainWindow", "VLAN (802.1Q frame)?", 0));
        label_eth_preamble->setText(QApplication::translate("MainWindow", "Preamble:", 0));
        label_eth_crc->setText(QApplication::translate("MainWindow", "CRC:", 0));
        label_eth_vid->setText(QApplication::translate("MainWindow", "VID:", 0));
        label_eth_dest_mac->setText(QApplication::translate("MainWindow", "Destination MAC:", 0));
        label_eth_src_mac->setText(QApplication::translate("MainWindow", "Source MAC:", 0));
        label_eth_dei->setText(QApplication::translate("MainWindow", "DEI:", 0));
        label_eth_data->setText(QApplication::translate("MainWindow", "Data:", 0));
        lineEdit_eth_frame_type->setText(QApplication::translate("MainWindow", "0x806", 0));
        label_eth_pcp->setText(QApplication::translate("MainWindow", "PCP:", 0));
        checkBox_eth_rand_pcp->setText(QApplication::translate("MainWindow", "Random", 0));
        lineEdit_eth_sfd->setText(QApplication::translate("MainWindow", "AB", 0));
        lineEdit_eth_sfd->setPlaceholderText(QString());
        SaveL2Button->setText(QApplication::translate("MainWindow", "Save", 0));
        tabWidget->setTabText(tabWidget->indexOf(Layer2), QApplication::translate("MainWindow", "Layer 2", 0));
        groupBox_layer3->setTitle(QApplication::translate("MainWindow", "IPv4 Header", 0));
        label_ip_total_length->setText(QApplication::translate("MainWindow", "Total length", 0));
        label_ip_flags->setText(QApplication::translate("MainWindow", "Flags", 0));
        label_ip_dest_ip->setText(QApplication::translate("MainWindow", "Destination IP adress:", 0));
        label_ip_frag_offset->setText(QApplication::translate("MainWindow", "Fragment offset:", 0));
        label_ip_dscp->setText(QApplication::translate("MainWindow", "DSCP:", 0));
        label_ip_ihl->setText(QApplication::translate("MainWindow", "IHL:", 0));
        label_ip_protocol->setText(QApplication::translate("MainWindow", "Protocol:", 0));
        label_ip_version->setText(QApplication::translate("MainWindow", "Version:", 0));
        label_ip_options->setText(QApplication::translate("MainWindow", "Options:", 0));
        label_ip_ttl->setText(QApplication::translate("MainWindow", "Time to live:", 0));
        label_ip_id->setText(QApplication::translate("MainWindow", "Identification:", 0));
        label_ip_ecn->setText(QApplication::translate("MainWindow", "ECN:", 0));
        checkBox_ip_rand_id->setText(QApplication::translate("MainWindow", "Random", 0));
        checkBox_ip_rand_src_ip->setText(QApplication::translate("MainWindow", "Random", 0));
        checkBox_ip_rand_ttl->setText(QApplication::translate("MainWindow", "Random", 0));
        checkBox_ip_rand_dest_ip->setText(QApplication::translate("MainWindow", "Random", 0));
        label_ip_checksum->setText(QApplication::translate("MainWindow", "Header checksum:", 0));
        label_ip_src_ip->setText(QApplication::translate("MainWindow", "Source IP adress:", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "Next step!", 0));
        tabWidget->setTabText(tabWidget->indexOf(Layer3), QApplication::translate("MainWindow", "Layer 3", 0));
        groupBox_layer4->setTitle(QApplication::translate("MainWindow", "TCP/UDP Header", 0));
        checkBox->setText(QApplication::translate("MainWindow", "CheckBox", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "TCP", 0));
        checkBox_2->setText(QApplication::translate("MainWindow", "CheckBox", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "UDP", 0));
        tabWidget->setTabText(tabWidget->indexOf(Layer4), QApplication::translate("MainWindow", "Layer 4", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
