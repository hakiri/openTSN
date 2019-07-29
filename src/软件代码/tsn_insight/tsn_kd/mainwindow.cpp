#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QTime>
#include <QTimer>
#include "test.h"
#include "qt_info_io.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette palette(this->palette());
    palette.setColor(QPalette::Background,Qt::white);
    this->setPalette(palette);

    //time slot
    for(int i=0;i<10;i++)
    {
        num[i] = 0;
    }
    n=0;
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(Graph_Show()));
    //拓扑图片
    QString StrWidth,StrHeigth;
    QString filename="TuoPu.jpg";
    QImage* img=new QImage,* scaledimg=new QImage;//分别保存原图和缩放之后的图片
    if(! ( img->load(filename) ) ) //加载图像
    {
        QMessageBox::information(this,
        tr("打开图像失败"),
        tr("打开图像失败!"));
        delete img;
        return;
    }
    //int Owidth=img->width(),Oheight=img->height();
    //int Fwidth,Fheight;       //缩放后图片大小¡
    ui->label->setGeometry(0,40,1100,800);
    //            int Mul;            //记录图片与label大小比例
    //            if(Owidth/400>=Oheight/300)
    //                Mul=Owidth/400;
    //           else
    //                Mul=Oheight/300;
    //            Fwidth=Owidth/Mul;
    //            Fheight=Oheight/Mul;
     *scaledimg=img->scaled(950,1000,Qt::KeepAspectRatio);
     //ui->label_text->setText(QString("width: ")+StrWidth.setNum(Fwidth)
     //                       +QString("\nheight: ")+StrHeigth.setNum(Fheight));
     ui->label->setPixmap(QPixmap::fromImage(*scaledimg));
     //表格
     QTableWidgetItem *item = new QTableWidgetItem();
     //QString test=QString::number(c);
     //ui->tableWidget_1->setItem(0,0,new QTableWidgetItem(test));
     //connect(test,SIGNAL(valueChanged(test)),ui->tableWidget_1,SLOT(ui->tableWidget_1->setItem(0,0,new QTableWidgetItem(test))));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Graph_Show()
{

    QTime t;
    t=QTime::currentTime();
    //qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    //n=qrand()%50;
    Graph_Show(ui->widget);
}

void MainWindow::Graph_Show(QCustomPlot *CustomPlot)
{
    QVector<double> temp(10);   //横坐标
    QVector<double> temp1(10);  //纵坐标

    //向左移一格数据
    for(int i=0; i<9; i++)
    {
        num[i]=num[i+1];
    }
    num[9]=global.global_time_offset;
    qDebug()<<global.global_time_offset;

    //坐标赋值
    for(int i=0;i<10;i++)
    {
        temp[i] = i;
        temp1[i] =num[i];
    }
    //拓扑信息显示
    ui->label_0_1->setText(QString("0号主机")
                          +QString("\nmac地址:")+QString::number((tp_info[0].host_mac[0]),16)+QString(":")+QString::number((tp_info[0].host_mac[1]),16)+QString(":")+QString::number((tp_info[0].host_mac[2]),16)+QString(":")+QString::number((tp_info[0].host_mac[3]),16)+QString(":")+QString::number((tp_info[0].host_mac[4]),16)+QString(":")+QString::number((tp_info[0].host_mac[5]),16)
                          +QString("\n类型:CNC")
            );
    ui->label_0_2->setText(QString("MASTER")
                          +QString("\nmac地址:")+QString::number((tp_info[0].sw_mac[0]),16)+QString(":")+QString::number((tp_info[0].sw_mac[1]),16)+QString(":")+QString::number((tp_info[0].sw_mac[2]),16)+QString(":")+QString::number((tp_info[0].sw_mac[3]),16)+QString(":")+QString::number((tp_info[0].sw_mac[4]),16)+QString(":")+QString::number((tp_info[0].sw_mac[5]),16)
                          +QString("\n已用带宽资源:")+QString::number(global.sw_info[0].res_info.used_bd)+QString("Mb/s")
                          +QString("\n时间同步偏差:")+QString::number(global.sw_info[0].time_offset)+QString("ns")
            );
    ui->label_1_1->setText(QString("1号主机")
                          +QString("\nmac地址:")+QString::number((tp_info[1].host_mac[0]),16)+QString(":")+QString::number((tp_info[1].host_mac[1]),16)+QString(":")+QString::number((tp_info[1].host_mac[2]),16)+QString(":")+QString::number((tp_info[1].host_mac[3]),16)+QString(":")+QString::number((tp_info[1].host_mac[4]),16)+QString(":")+QString::number((tp_info[1].host_mac[5]),16)
                          +QString("\n类型:MASTER")
            );
    ui->label_1_2->setText(QString("SLAVE")
                          +QString("\nmac地址:")+QString::number((tp_info[1].sw_mac[0]),16)+QString(":")+QString::number((tp_info[1].sw_mac[1]),16)+QString(":")+QString::number((tp_info[1].sw_mac[2]),16)+QString(":")+QString::number((tp_info[1].sw_mac[3]),16)+QString(":")+QString::number((tp_info[1].sw_mac[4]),16)+QString(":")+QString::number((tp_info[1].sw_mac[5]),16)
                          +QString("\n已用带宽资源:")+QString::number(global.sw_info[1].res_info.used_bd)+QString("Mb/s")
                          +QString("\n时间同步偏差:")+QString::number(global.sw_info[1].time_offset)+QString("ns")
            );
    ui->label_2_1->setText(QString("2号主机")
                          +QString("\nmac地址:")+QString::number((tp_info[2].host_mac[0]),16)+QString(":")+QString::number((tp_info[2].host_mac[1]),16)+QString(":")+QString::number((tp_info[2].host_mac[2]),16)+QString(":")+QString::number((tp_info[2].host_mac[3]),16)+QString(":")+QString::number((tp_info[2].host_mac[4]),16)+QString(":")+QString::number((tp_info[2].host_mac[5]),16)
                          +QString("\n类型:MASTER")
            );
    ui->label_2_2->setText(QString("SLAVE")
                          +QString("\nmac地址:")+QString::number((tp_info[2].sw_mac[0]),16)+QString(":")+QString::number((tp_info[2].sw_mac[1]),16)+QString(":")+QString::number((tp_info[2].sw_mac[2]),16)+QString(":")+QString::number((tp_info[2].sw_mac[3]),16)+QString(":")+QString::number((tp_info[2].sw_mac[4]),16)+QString(":")+QString::number((tp_info[2].sw_mac[5]),16)
                          +QString("\n已用带宽资源:")+QString::number(global.sw_info[2].res_info.used_bd)+QString("Mb/s")
                          +QString("\n时间同步偏差:")+QString::number(global.sw_info[2].time_offset)+QString("ns")
            );
    ui->label_3_1->setText(QString("3号主机")
                          +QString("\nmac地址:")+QString::number((tp_info[3].host_mac[0]),16)+QString(":")+QString::number((tp_info[3].host_mac[1]),16)+QString(":")+QString::number((tp_info[3].host_mac[2]),16)+QString(":")+QString::number((tp_info[3].host_mac[3]),16)+QString(":")+QString::number((tp_info[3].host_mac[4]),16)+QString(":")+QString::number((tp_info[3].host_mac[5]),16)
                          +QString("\n类型:MASTER")
            );
    ui->label_3_2->setText(QString("SLAVE")
                          +QString("\nmac地址:")+QString::number((tp_info[3].sw_mac[0]),16)+QString(":")+QString::number((tp_info[3].sw_mac[1]),16)+QString(":")+QString::number((tp_info[3].sw_mac[2]),16)+QString(":")+QString::number((tp_info[3].sw_mac[3]),16)+QString(":")+QString::number((tp_info[3].sw_mac[4]),16)+QString(":")+QString::number((tp_info[5].sw_mac[3]),16)
                          +QString("\n已用带宽资源:")+QString::number(global.sw_info[3].res_info.used_bd)+QString("Mb/s")
                          +QString("\n时间同步偏差:")+QString::number(global.sw_info[3].time_offset)+QString("ns")
            );
    ui->label_4_1->setText(QString("4号主机")
                          +QString("\nmac地址:")+QString::number((tp_info[4].host_mac[0]),16)+QString(":")+QString::number((tp_info[4].host_mac[1]),16)+QString(":")+QString::number((tp_info[4].host_mac[2]),16)+QString(":")+QString::number((tp_info[4].host_mac[3]),16)+QString(":")+QString::number((tp_info[4].host_mac[4]),16)+QString(":")+QString::number((tp_info[4].host_mac[5]),16)
                          +QString("\n类型:CAMERA")
            );
    ui->label_4_2->setText(QString("SLAVE")
                          +QString("\nmac地址:")+QString::number((tp_info[4].sw_mac[0]),16)+QString(":")+QString::number((tp_info[4].sw_mac[1]),16)+QString(":")+QString::number((tp_info[4].sw_mac[2]),16)+QString(":")+QString::number((tp_info[4].sw_mac[3]),16)+QString(":")+QString::number((tp_info[4].sw_mac[4]),16)+QString(":")+QString::number((tp_info[4].sw_mac[5]),16)
                          +QString("\n已用带宽资源:")+QString::number(global.sw_info[4].res_info.used_bd)+QString("Mb/s")
                          +QString("\n时间同步偏差:")+QString::number(global.sw_info[4].time_offset)+QString("ns")
            );
    ui->label_5_1->setText(QString("5号主机")
                          +QString("\nmac地址:")+QString::number((tp_info[5].host_mac[0]),16)+QString(":")+QString::number((tp_info[5].host_mac[1]),16)+QString(":")+QString::number((tp_info[5].host_mac[2]),16)+QString(":")+QString::number((tp_info[5].host_mac[3]),16)+QString(":")+QString::number((tp_info[5].host_mac[4]),16)+QString(":")+QString::number((tp_info[5].host_mac[5]),16)
                          +QString("\n类型:MASTER")
            );
    ui->label_5_2->setText(QString("SLAVE")
                          +QString("\nmac地址:")+QString::number((tp_info[5].sw_mac[0]),16)+QString(":")+QString::number((tp_info[5].sw_mac[1]),16)+QString(":")+QString::number((tp_info[5].sw_mac[2]),16)+QString(":")+QString::number((tp_info[5].sw_mac[3]),16)+QString(":")+QString::number((tp_info[5].sw_mac[4]),16)+QString(":")+QString::number((tp_info[5].sw_mac[5]))
                          +QString("\n已用带宽资源:")+QString::number(global.sw_info[5].res_info.used_bd)+QString("Mb/s")
                          +QString("\n时间同步偏差:")+QString::number(global.sw_info[5].time_offset)+QString("ns")
            );
    //TSN流特征信息
    for(int i=0;i<flow.tsn_num;i++){
        ui->tableWidget_1->setItem(i,0,new QTableWidgetItem(QString::number((flow.tsn_flow[i].src_mac[0]),16)+QString(":")+QString::number((flow.tsn_flow[i].src_mac[1]),16)+QString(":")+QString::number((flow.tsn_flow[i].src_mac[2]),16)+QString(":")+QString::number((flow.tsn_flow[i].src_mac[3]),16)+QString(":")+QString::number((flow.tsn_flow[i].src_mac[4]),16)+QString(":")+QString::number((flow.tsn_flow[i].src_mac[5]),16)));
        ui->tableWidget_1->setItem(i,1,new QTableWidgetItem(QString::number((flow.tsn_flow[i].dst_mac[0]),16)+QString(":")+QString::number((flow.tsn_flow[i].dst_mac[1]),16)+QString(":")+QString::number((flow.tsn_flow[i].dst_mac[2]),16)+QString(":")+QString::number((flow.tsn_flow[i].dst_mac[3]),16)+QString(":")+QString::number((flow.tsn_flow[i].dst_mac[4]),16)+QString(":")+QString::number((flow.tsn_flow[i].dst_mac[5]),16)));
        ui->tableWidget_1->setItem(i,2,new QTableWidgetItem(QString::number((flow.tsn_flow[i].interval))));
        ui->tableWidget_1->setItem(i,3,new QTableWidgetItem(QString::number(flow.tsn_flow[i].latency)));
    }
    //带宽预约流特征信息
    for(int i=0;i<flow.bd_num;i++){
        ui->tableWidget_2->setItem(i,0,new QTableWidgetItem(QString::number((flow.bd_flow[i].src_mac[0]),16)+QString(":")+QString::number((flow.bd_flow[i].src_mac[1]),16)+QString(":")+QString::number((flow.bd_flow[i].src_mac[2]),16)+QString(":")+QString::number((flow.bd_flow[i].src_mac[3]),16)+QString(":")+QString::number((flow.bd_flow[i].src_mac[4]),16)+QString(":")+QString::number((flow.bd_flow[i].src_mac[5]),16)));
        ui->tableWidget_2->setItem(i,1,new QTableWidgetItem(QString::number((flow.bd_flow[i].dst_mac[0]),16)+QString(":")+QString::number((flow.bd_flow[i].dst_mac[1]),16)+QString(":")+QString::number((flow.bd_flow[i].dst_mac[2]),16)+QString(":")+QString::number((flow.bd_flow[i].dst_mac[3]),16)+QString(":")+QString::number((flow.bd_flow[i].dst_mac[4]),16)+QString(":")+QString::number((flow.bd_flow[i].dst_mac[5]),16)));
        ui->tableWidget_2->setItem(i,2,new QTableWidgetItem(QString::number(flow.bd_flow[i].bandwidth)));
    }
    //交换机信息
    for(int i=0;i<6;i++){
        ui->tableWidget_3->setItem(0,i,new QTableWidgetItem((QString::number((global.sw_info[i].cnt_info.eos_q0rest_cnt),16))));
        ui->tableWidget_3->setItem(1,i,new QTableWidgetItem(QString::number((global.sw_info[i].cnt_info.eos_q1rest_cnt),16)));
        ui->tableWidget_3->setItem(2,i,new QTableWidgetItem(QString::number((global.sw_info[i].cnt_info.eos_q2rest_cnt),16)));
        ui->tableWidget_3->setItem(3,i,new QTableWidgetItem(QString::number((global.sw_info[i].cnt_info.eos_q3rest_cnt),16)));
    }

    ui->tableWidget_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //显示折线
    CustomPlot->addGraph();
    CustomPlot->graph(0)->setPen(QPen(Qt::red));
    CustomPlot->graph(0)->setData(temp,temp1);

    CustomPlot->xAxis->setLabel("时间/s");
    CustomPlot->yAxis->setLabel("同步精度(ns)");

    CustomPlot->xAxis->setRange(0,10);
    CustomPlot->yAxis->setRange(-1,100);
    CustomPlot->replot();
}
