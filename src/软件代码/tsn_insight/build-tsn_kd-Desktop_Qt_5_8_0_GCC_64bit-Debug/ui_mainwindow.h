/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QCustomPlot *widget;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_text;
    QLabel *label_3;
    QTableWidget *tableWidget_1;
    QTableWidget *tableWidget_2;
    QLabel *label_5;
    QLabel *label_6;
    QTableWidget *tableWidget_3;
    QLabel *label_7;
    QLabel *label_30;
    QLabel *label_3_2;
    QLabel *label_2_1;
    QLabel *label_3_1;
    QLabel *label_2_2;
    QLabel *label_45;
    QLabel *label_48;
    QLabel *label_1_2;
    QLabel *label_1_1;
    QLabel *label_55;
    QLabel *label_0_2;
    QLabel *label_0_1;
    QLabel *label_64;
    QLabel *label_5_2;
    QLabel *label_5_1;
    QLabel *label_71;
    QLabel *label_4_2;
    QLabel *label_4_1;
    QFrame *line;
    QFrame *line_2;
    QLabel *label_4;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1760, 939);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        widget = new QCustomPlot(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(1010, 350, 731, 331));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 40, 1011, 781));
        label->setLineWidth(1);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 50, 241, 41));
        QFont font;
        font.setFamily(QStringLiteral("Abyssinica SIL"));
        font.setPointSize(22);
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        label_2->setFont(font);
        label_text = new QLabel(centralWidget);
        label_text->setObjectName(QStringLiteral("label_text"));
        label_text->setGeometry(QRect(860, 0, 261, 41));
        QFont font1;
        font1.setFamily(QStringLiteral("URW Gothic L"));
        font1.setPointSize(33);
        font1.setBold(true);
        font1.setWeight(75);
        label_text->setFont(font1);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(1130, 120, 171, 31));
        QFont font2;
        font2.setFamily(QStringLiteral("Abyssinica SIL"));
        font2.setPointSize(18);
        font2.setBold(true);
        font2.setWeight(75);
        label_3->setFont(font2);
        tableWidget_1 = new QTableWidget(centralWidget);
        if (tableWidget_1->columnCount() < 4)
            tableWidget_1->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setText(QString::fromUtf8("\346\272\220mac"));
        tableWidget_1->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_1->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_1->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_1->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        if (tableWidget_1->rowCount() < 3)
            tableWidget_1->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_1->setVerticalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_1->setVerticalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_1->setVerticalHeaderItem(2, __qtablewidgetitem6);
        QFont font3;
        font3.setBold(false);
        font3.setWeight(50);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        __qtablewidgetitem7->setFont(font3);
        __qtablewidgetitem7->setFlags(Qt::NoItemFlags);
        tableWidget_1->setItem(0, 0, __qtablewidgetitem7);
        tableWidget_1->setObjectName(QStringLiteral("tableWidget_1"));
        tableWidget_1->setGeometry(QRect(1000, 160, 421, 121));
        tableWidget_2 = new QTableWidget(centralWidget);
        if (tableWidget_2->columnCount() < 3)
            tableWidget_2->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem10);
        if (tableWidget_2->rowCount() < 3)
            tableWidget_2->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(0, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(1, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget_2->setVerticalHeaderItem(2, __qtablewidgetitem13);
        tableWidget_2->setObjectName(QStringLiteral("tableWidget_2"));
        tableWidget_2->setGeometry(QRect(1430, 160, 321, 121));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(1480, 120, 221, 31));
        label_5->setFont(font2);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(1260, 300, 241, 31));
        label_6->setFont(font2);
        tableWidget_3 = new QTableWidget(centralWidget);
        if (tableWidget_3->columnCount() < 6)
            tableWidget_3->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(0, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(1, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(2, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(3, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(4, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(5, __qtablewidgetitem19);
        if (tableWidget_3->rowCount() < 4)
            tableWidget_3->setRowCount(4);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tableWidget_3->setVerticalHeaderItem(0, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tableWidget_3->setVerticalHeaderItem(1, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        tableWidget_3->setVerticalHeaderItem(2, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        tableWidget_3->setVerticalHeaderItem(3, __qtablewidgetitem23);
        tableWidget_3->setObjectName(QStringLiteral("tableWidget_3"));
        tableWidget_3->setGeometry(QRect(1000, 730, 741, 151));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(1290, 690, 171, 31));
        label_7->setFont(font2);
        label_30 = new QLabel(centralWidget);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setGeometry(QRect(540, 360, 61, 21));
        label_30->setLineWidth(1);
        label_3_2 = new QLabel(centralWidget);
        label_3_2->setObjectName(QStringLiteral("label_3_2"));
        label_3_2->setGeometry(QRect(610, 580, 201, 111));
        label_2_1 = new QLabel(centralWidget);
        label_2_1->setObjectName(QStringLiteral("label_2_1"));
        label_2_1->setGeometry(QRect(810, 520, 201, 111));
        label_3_1 = new QLabel(centralWidget);
        label_3_1->setObjectName(QStringLiteral("label_3_1"));
        label_3_1->setGeometry(QRect(610, 730, 181, 91));
        label_2_2 = new QLabel(centralWidget);
        label_2_2->setObjectName(QStringLiteral("label_2_2"));
        label_2_2->setGeometry(QRect(670, 350, 171, 111));
        label_45 = new QLabel(centralWidget);
        label_45->setObjectName(QStringLiteral("label_45"));
        label_45->setGeometry(QRect(590, 440, 61, 21));
        label_45->setLineWidth(1);
        label_48 = new QLabel(centralWidget);
        label_48->setObjectName(QStringLiteral("label_48"));
        label_48->setGeometry(QRect(510, 500, 61, 21));
        label_48->setLineWidth(1);
        label_1_2 = new QLabel(centralWidget);
        label_1_2->setObjectName(QStringLiteral("label_1_2"));
        label_1_2->setGeometry(QRect(570, 230, 201, 111));
        label_1_1 = new QLabel(centralWidget);
        label_1_1->setObjectName(QStringLiteral("label_1_1"));
        label_1_1->setGeometry(QRect(610, 70, 191, 121));
        label_55 = new QLabel(centralWidget);
        label_55->setObjectName(QStringLiteral("label_55"));
        label_55->setGeometry(QRect(400, 510, 61, 21));
        label_55->setLineWidth(1);
        label_0_2 = new QLabel(centralWidget);
        label_0_2->setObjectName(QStringLiteral("label_0_2"));
        label_0_2->setGeometry(QRect(230, 210, 191, 101));
        label_0_1 = new QLabel(centralWidget);
        label_0_1->setObjectName(QStringLiteral("label_0_1"));
        label_0_1->setGeometry(QRect(210, 80, 201, 111));
        label_64 = new QLabel(centralWidget);
        label_64->setObjectName(QStringLiteral("label_64"));
        label_64->setGeometry(QRect(350, 430, 61, 20));
        label_64->setLineWidth(1);
        label_5_2 = new QLabel(centralWidget);
        label_5_2->setObjectName(QStringLiteral("label_5_2"));
        label_5_2->setGeometry(QRect(120, 320, 171, 121));
        label_5_1 = new QLabel(centralWidget);
        label_5_1->setObjectName(QStringLiteral("label_5_1"));
        label_5_1->setGeometry(QRect(20, 470, 191, 121));
        label_71 = new QLabel(centralWidget);
        label_71->setObjectName(QStringLiteral("label_71"));
        label_71->setGeometry(QRect(380, 360, 61, 20));
        label_71->setLineWidth(1);
        label_4_2 = new QLabel(centralWidget);
        label_4_2->setObjectName(QStringLiteral("label_4_2"));
        label_4_2->setGeometry(QRect(260, 560, 181, 111));
        label_4_1 = new QLabel(centralWidget);
        label_4_1->setObjectName(QStringLiteral("label_4_1"));
        label_4_1->setGeometry(QRect(230, 730, 151, 91));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(10, 29, 1741, 41));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(960, 50, 20, 851));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(980, 50, 231, 41));
        label_4->setFont(font);
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Source Insight", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\345\205\250\345\261\200\346\213\223\346\211\221\350\265\204\346\272\220\344\277\241\346\201\257", Q_NULLPTR));
        label_text->setText(QApplication::translate("MainWindow", "TSN INSIGHT", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "TSN\346\265\201\347\211\271\345\276\201\344\277\241\346\201\257", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_1->horizontalHeaderItem(1);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "\347\233\256\347\232\204mac", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_1->horizontalHeaderItem(2);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "\346\227\266\351\227\264\345\221\250\346\234\237(us)", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_1->horizontalHeaderItem(3);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "\346\234\200\345\244\247\345\273\266\350\277\237(us)", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_1->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_1->verticalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "2", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_1->verticalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "3", Q_NULLPTR));

        const bool __sortingEnabled = tableWidget_1->isSortingEnabled();
        tableWidget_1->setSortingEnabled(false);
        tableWidget_1->setSortingEnabled(__sortingEnabled);

        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "\346\272\220mac", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "\347\233\256\347\232\204mac", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_2->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QApplication::translate("MainWindow", "\351\242\204\347\272\246\345\270\246\345\256\275(Mb/s)", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_2->verticalHeaderItem(0);
        ___qtablewidgetitem9->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_2->verticalHeaderItem(1);
        ___qtablewidgetitem10->setText(QApplication::translate("MainWindow", "2", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_2->verticalHeaderItem(2);
        ___qtablewidgetitem11->setText(QApplication::translate("MainWindow", "3", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "\345\270\246\345\256\275\351\242\204\347\272\246\346\265\201\347\211\271\345\276\201\344\277\241\346\201\257", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "\345\205\250\345\261\200\346\227\266\351\222\237\345\220\214\346\255\245\347\212\266\346\200\201\345\261\225\347\244\272", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget_3->horizontalHeaderItem(0);
        ___qtablewidgetitem12->setText(QApplication::translate("MainWindow", "\344\272\244\346\215\242\346\234\2720", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget_3->horizontalHeaderItem(1);
        ___qtablewidgetitem13->setText(QApplication::translate("MainWindow", "\344\272\244\346\215\242\346\234\2721", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget_3->horizontalHeaderItem(2);
        ___qtablewidgetitem14->setText(QApplication::translate("MainWindow", "\344\272\244\346\215\242\346\234\2722", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget_3->horizontalHeaderItem(3);
        ___qtablewidgetitem15->setText(QApplication::translate("MainWindow", "\344\272\244\346\215\242\346\234\2723", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidget_3->horizontalHeaderItem(4);
        ___qtablewidgetitem16->setText(QApplication::translate("MainWindow", "\344\272\244\346\215\242\346\234\2724", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem17 = tableWidget_3->horizontalHeaderItem(5);
        ___qtablewidgetitem17->setText(QApplication::translate("MainWindow", "\344\272\244\346\215\242\346\234\2725", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem18 = tableWidget_3->verticalHeaderItem(0);
        ___qtablewidgetitem18->setText(QApplication::translate("MainWindow", "TSN\351\230\237\345\210\2270", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem19 = tableWidget_3->verticalHeaderItem(1);
        ___qtablewidgetitem19->setText(QApplication::translate("MainWindow", "TSN\351\230\237\345\210\2271", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem20 = tableWidget_3->verticalHeaderItem(2);
        ___qtablewidgetitem20->setText(QApplication::translate("MainWindow", "\345\270\246\345\256\275\351\242\204\347\272\246\351\230\237\345\210\227", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem21 = tableWidget_3->verticalHeaderItem(3);
        ___qtablewidgetitem21->setText(QApplication::translate("MainWindow", "BEST EFFORT\351\230\237\345\210\227", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "\344\272\244\346\215\242\346\234\272\350\260\203\345\272\246\344\277\241\346\201\257", Q_NULLPTR));
        label_30->setText(QApplication::translate("MainWindow", "1\345\217\267", Q_NULLPTR));
        label_3_2->setText(QApplication::translate("MainWindow", "\344\272\244\346\215\242\346\234\2723", Q_NULLPTR));
        label_2_1->setText(QApplication::translate("MainWindow", "\344\270\273\346\234\2722", Q_NULLPTR));
        label_3_1->setText(QApplication::translate("MainWindow", "\344\270\273\346\234\2723", Q_NULLPTR));
        label_2_2->setText(QApplication::translate("MainWindow", "\344\272\244\346\215\242\346\234\2722", Q_NULLPTR));
        label_45->setText(QApplication::translate("MainWindow", "2\345\217\267", Q_NULLPTR));
        label_48->setText(QApplication::translate("MainWindow", "3\345\217\267", Q_NULLPTR));
        label_1_2->setText(QApplication::translate("MainWindow", "\344\272\244\346\215\242\346\234\2721", Q_NULLPTR));
        label_1_1->setText(QApplication::translate("MainWindow", "\344\270\273\346\234\2721", Q_NULLPTR));
        label_55->setText(QApplication::translate("MainWindow", "4\345\217\267", Q_NULLPTR));
        label_0_2->setText(QApplication::translate("MainWindow", "\344\272\244\346\215\242\346\234\2720", Q_NULLPTR));
        label_0_1->setText(QApplication::translate("MainWindow", "\344\270\273\346\234\2720", Q_NULLPTR));
        label_64->setText(QApplication::translate("MainWindow", "5\345\217\267", Q_NULLPTR));
        label_5_2->setText(QApplication::translate("MainWindow", "\344\272\244\346\215\242\346\234\2725", Q_NULLPTR));
        label_5_1->setText(QApplication::translate("MainWindow", "\344\270\273\346\234\2725", Q_NULLPTR));
        label_71->setText(QApplication::translate("MainWindow", "0\345\217\267", Q_NULLPTR));
        label_4_2->setText(QApplication::translate("MainWindow", "\344\272\244\346\215\242\346\234\2724", Q_NULLPTR));
        label_4_1->setText(QApplication::translate("MainWindow", "\344\270\273\346\234\2724", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "TSN\350\277\220\350\241\214\347\212\266\346\200\201\345\261\225\347\244\272", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
