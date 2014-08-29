#ifndef FILTREPROGRAMI_H
#define FILTREPROGRAMI_H
#include <QListWidgetItem>
#include <QMainWindow>
#include <QDialog>
#include <QMessageBox>
#include <QTimer>
#include <QFormLayout>
#include <QProgressBar>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

namespace Ui {
class FiltreProgrami;
}

class FiltreProgrami : public QMainWindow
{
    Q_OBJECT

public:
    explicit FiltreProgrami(QWidget *parent = 0);
    ~FiltreProgrami();

private slots:
    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_actionTake_Photo_triggered();

    void on_actionReset_triggered();

    void on_actionExport_triggered();

    void on_actionInstagram_triggered();
    void instagramGiris();
    void imageTransfer();

    void on_actionBluetooth_triggered();

    void on_actionAbout_triggered();

    void on_list_itemClicked(QListWidgetItem *item);

private:
    Ui::FiltreProgrami *ui;
    QString filename;
    QPixmap image;
    QImage  *imageObject;
    ////Instagram/////
    QWidget *windowInstagram;
    QPushButton *loginInstagram;
    QPushButton *cancelInstagram;
    QLineEdit *lineUserName;
    QLineEdit *linePassword;
    QFormLayout *layoutInstagram;
    //////InstagramGiris////
    QWidget *windowInstagramGiris;
    QFormLayout *layoutInstagramGiris;
    //////Bluetooth/////
    QWidget *windowBluetooth;
    QListWidget *listBluetooth1;
    QVBoxLayout *layoutBluetooth;
    QPushButton *forward1;
    QPushButton *cancelBluetooth1;
    QFormLayout *butonLayout;
    ///////imageTransfer////
    QWidget *windowImageTransfer;
    QVBoxLayout *layoutImageTransfer1;
    QPushButton *forward2;
    QPushButton *cancelBluetooth2;
    QLineEdit *lineAygit;
    QLineEdit *lineAdi;
    QFormLayout *layoutImageTransfer2;
    QProgressBar *progresBar;



};

#endif // FILTREPROGRAMI_H
