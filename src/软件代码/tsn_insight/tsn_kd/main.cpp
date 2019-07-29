#include "mainwindow.h"
#include <QApplication>
#include "test.h"
#include "qt_info_io.h"
#include "Qt_recv.h"
#include "pthread.h"


int c=1;

int main(int argc, char *argv[])
{
    pthread_t tid;
    int ret = 0;


    ret = pthread_create(&tid,NULL,init_socket_tcp_server,NULL);
    if(ret < 0)
    {
        printf("create thread fail!\n");
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();








    return a.exec();

}
