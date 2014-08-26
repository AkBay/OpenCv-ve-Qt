#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/qbluetooth.h>
#include <QTimer>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionReset_triggered();

    void on_actionExit_triggered();

    void on_list_itemClicked(QListWidgetItem *item);

    void on_actionExport_triggered();

    void on_actionAbout_triggered();

    void on_actionInstagram_triggered();

    void on_actionBack_triggered();
    void instagramGiris();

    void on_actionBluetooth_triggered();

    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void on_pushButton_clicked();

    void on_actionTake_Photo_triggered();

private:
    Ui::MainWindow *ui;
    QString filename;
    QPixmap image;
    QWidget *window;
    QWidget *window2;
    QWidget *w2;
    QPushButton *giris;
    QPushButton *cikis;
    QPushButton *giris2;
    QPushButton *cikis2;
    QLineEdit *lk;
    QLineEdit *ls;
    QListWidget *liste;

    QImage  *imageObject;

    QGraphicsScene *scene;

    QBluetoothDeviceDiscoveryAgent *discoveryAgent;

};

#endif // MAINWINDOW_H
