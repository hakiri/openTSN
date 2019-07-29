#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    double num[10];
    int n;
    void Graph_Show(QCustomPlot *customPlot);

private slots:
    void Graph_Show();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
