#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QGraphicsScene>
#include <QApplication>
#include <QListWidgetItem>
#include <QPushButton>
#include <qlineedit.h>
#include <qstring.h>
#include <QtBluetooth/qbluetooth.h>
#include <QtBluetooth/qbluetoothdevicediscoveryagent.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qlist.h>
#include <qdebug.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothdevicediscoveryagent.h>
using namespace std;
using namespace cv;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)),
            this, SLOT(deviceDiscovered(const QBluetoothDeviceInfo&)));

          // Start a discovery
      // QList->QBlueothDeviceDiscoveryAgent
          discoveryAgent->start();



          QBluetoothLocalDevice localDevice;
          QString localDeviceName;

          // Check if Bluetooth is available on this device
          if (localDevice.isValid()) {

              // Turn Bluetooth on
              localDevice.powerOn();

              // Read local device name
              localDeviceName = localDevice.name();

              // Make it visible to others
              localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
          }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    filename=QFileDialog::getOpenFileName(
                this,
                tr("open file"),
                "C://",
                "All Files (*.*);;Image file (*.png *.xpm *.jpg)"
                );

    if(filename.isNull())
    {
        QMessageBox::about(this, tr("Uyari"),
                           tr("Dosya yuklenemedi. "
                              " "
                              ));
    }
    else
    {
  // QMessageBox::information(this,tr("File Name"),filename);
  //QDesktopServices::openUrl(QUrl("file:///"+filename,QUrl::TolerantMode));
   //resmi ekrana getirmek için
        imageObject = new QImage();
        imageObject->load(filename);

        image = QPixmap::fromImage(*imageObject);
        ui->label->setPixmap(image);


   /* scene = new QGraphicsScene(this);

    scene->addPixmap(image);

    scene->setSceneRect(image.rect());

    ui->graphicsView->setScene(scene);*/

  //pasif olan kısımları aktif yap
        ui->actionReset->setEnabled(true);
        ui->actionExport->setEnabled(true);
        ui->actionExit->setEnabled(true);
        ui->menuShare->setEnabled(true);
        ui->list->setEnabled(true);
    }
}

void MainWindow::on_actionReset_triggered()
{
    ui->label->setPixmap(image);
}

void MainWindow::on_actionExit_triggered()
{
    close();//exit ile çıkış
}

void MainWindow::on_list_itemClicked(QListWidgetItem *item)
{
    if(ui->list->item(0)==item)
    {
        int width=image.toImage().width();
        int height=image.toImage().height();
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ////////
        ///////////
        ///
        Mat im;
        im=img;
         //cvNot(img,img);
        cvtColor(im, im,cv::COLOR_RGB2HSV);
       ///////////

        cvtColor(im, im,cv::COLOR_BGR2RGB);
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
          // cvNot(img, img);
           //  cvNamedWindow("MyWindow");
           //  cvShowImage("MyWindow", img);
    //çevirme
        ui->label->setPixmap(QPixmap::fromImage(ima));
    }


    ////////////////////////////////////////////////////////////////////////////////7777
    if(ui->list->item(1)==item)
    {
        int width=image.toImage().width();
        int height=image.toImage().height();
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ////////
        ////////
        Mat im;
        im=img;

             //cvNot(img,img);
               //cvtColor(im,im,cv::COLOR_RGB2GRAY);
        blur(im,im,Size(9,9));
        ////////////////
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
              // cvNot(img, img);
               //  cvNamedWindow("MyWindow");
               //  cvShowImage("MyWindow", img);
        //çevirme
        ui->label->setPixmap(QPixmap::fromImage(ima));
    }


    ////////////////////////////////////////////////////////////////////////////////7777
    if(ui->list->item(2)==item)
        {
            int width=image.toImage().width();
            int height=image.toImage().height();
            IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

            for(int y=0;y<height;y++)
                {
                    for(int x=0;x<width;x++ )
                    {
                        QRgb color = image.toImage().pixel(x,y);
                        cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
                    }
                }
           ////////
            Mat im;
            im=img;

             //cvNot(img,img);
               cvtColor(im,im,cv::COLOR_HLS2RGB_FULL);


           ///////////

            cvtColor(im, im,cv::COLOR_BGR2RGB);
            QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
              // cvNot(img, img);
               //  cvNamedWindow("MyWindow");
               //  cvShowImage("MyWindow", img);
        //çevirme
    ui->label->setPixmap(QPixmap::fromImage(ima));
        }





//////////////////////////////////////////////////////////////////////////7
    if(ui->list->item(3)==item)
    {
        int width=image.toImage().width();
        int height=image.toImage().height();
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
            {
                for(int x=0;x<width;x++ )
                {
                    QRgb color = image.toImage().pixel(x,y);
                    cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
                }
            }
       /////////
        Mat im;
        im=img;
         //cvNot(img,img);
           cvtColor(im, im,cv::COLOR_RGB2HLS);
       ///////////
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
          // cvNot(img, img);
           //  cvNamedWindow("MyWindow");
           //  cvShowImage("MyWindow", img);
    //çevirme
ui->label->setPixmap(QPixmap::fromImage(ima));
    }


    ////////////////////////////////////////////////////////////////////////
    if(ui->list->item(4)==item)
        {
            int width=image.toImage().width();
            int height=image.toImage().height();
            IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

            for(int y=0;y<height;y++)
                {
                    for(int x=0;x<width;x++ )
                    {
                        QRgb color = image.toImage().pixel(x,y);
                        cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
                    }
                }
           /////////////
            Mat im;
            im=img;

             //cvNot(img,img);
               //cvtColor(im,im,cv::COLOR_RGB2GRAY);
            GaussianBlur(im,im,Size(11,11),5);//Resmi Bulan�kla�t�rd�k.
           //////////
            cvtColor(im, im,cv::COLOR_BGR2RGB);
            QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
              // cvNot(img, img);
               //  cvNamedWindow("MyWindow");
               //  cvShowImage("MyWindow", img);
        //çevirme
    ui->label->setPixmap(QPixmap::fromImage(ima));
        }




    //////////////////////////////////////////////////////////////////////////////////////////////
    if(ui->list->item(5)==item)
        {
            int width=image.toImage().width();
            int height=image.toImage().height();
            IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

            for(int y=0;y<height;y++)
                {
                    for(int x=0;x<width;x++ )
                    {
                        QRgb color = image.toImage().pixel(x,y);
                        cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
                    }
                }
           ////////////////////
            Mat im;
            im=img;
             //cvNot(img,img);
               cvtColor(im, im,cv::COLOR_YCrCb2BGR);
           ////////////////////////////////////

            cvtColor(im, im,cv::COLOR_BGR2RGB);
            QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
              // cvNot(img, img);
               //  cvNamedWindow("MyWindow");
               //  cvShowImage("MyWindow", img);
        //çevirme
    ui->label->setPixmap(QPixmap::fromImage(ima));
        }

    //////////////////////////////////////////////////////////////////////////////////////////////
    if(ui->list->item(6)==item)
        {
            int width=image.toImage().width();
            int height=image.toImage().height();
            IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

            for(int y=0;y<height;y++)
                {
                    for(int x=0;x<width;x++ )
                    {
                        QRgb color = image.toImage().pixel(x,y);
                        cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
                    }
                }
           ////////////////////
            Mat im;
            im=img;
             //cvNot(img,img);
               cvtColor(im, im,cv::COLOR_YCrCb2RGB);
           ////////////////////////////////////

            cvtColor(im, im,cv::COLOR_BGR2RGB);
            QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
              // cvNot(img, img);
               //  cvNamedWindow("MyWindow");
               //  cvShowImage("MyWindow", img);
        //çevirme
    ui->label->setPixmap(QPixmap::fromImage(ima));
        }


    //////////////////////////////////////////////////////////////////////////////////////
    if(ui->list->item(7)==item)
        {
            int width=image.toImage().width();
            int height=image.toImage().height();
            IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

            for(int y=0;y<height;y++)
                {
                    for(int x=0;x<width;x++ )
                    {
                        QRgb color = image.toImage().pixel(x,y);
                        cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
                    }
                }
           ////////
            Mat im;
            im=img;

             //cvNot(img,img);
            cvtColor(im,im,cv::COLOR_BGR2RGB);


           //////////

            cvtColor(im, im,cv::COLOR_BGR2RGB);
            QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
              // cvNot(img, img);
               //  cvNamedWindow("MyWindow");
               //  cvShowImage("MyWindow", img);
        //çevirme
    ui->label->setPixmap(QPixmap::fromImage(ima));
        }


    ////////////////////////////////////////////////////////////
    if(ui->list->item(8)==item)
        {
            int width=image.toImage().width();
            int height=image.toImage().height();
            IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

            for(int y=0;y<height;y++)
                {
                    for(int x=0;x<width;x++ )
                    {
                        QRgb color = image.toImage().pixel(x,y);
                        cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
                    }
                }
           ///////////
            Mat im;
            im=img;

             //cvNot(img,img);
               //cvtColor(im,im,cv::COLOR_RGB2GRAY);
            medianBlur(im,im,15);
           //////////////
            cvtColor(im, im,cv::COLOR_BGR2RGB);
            QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
              // cvNot(img, img);
               //  cvNamedWindow("MyWindow");
               //  cvShowImage("MyWindow", img);
        //çevirme
    ui->label->setPixmap(QPixmap::fromImage(ima));
        }



    //////////////////////////////////////////////////////////////
    if(ui->list->item(9)==item)
    {
        int width=image.toImage().width();
        int height=image.toImage().height();
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
            {
                for(int x=0;x<width;x++ )
                {
                    QRgb color = image.toImage().pixel(x,y);
                    cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
                }
            }
       ////////////
         cvNot(img,img);

       ////////////
        Mat im;
        im=img;
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
          // cvNot(img, img);
           //  cvNamedWindow("MyWindow");
           //  cvShowImage("MyWindow", img);
    //çevirme
ui->label->setPixmap(QPixmap::fromImage(ima));


////////////////////////////////////////////////////////////////////////////

    }if(ui->list->item(10)==item)
    {
        int width=image.toImage().width();
        int height=image.toImage().height();
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
            {
                for(int x=0;x<width;x++ )
                {
                    QRgb color = image.toImage().pixel(x,y);
                    cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
                }
            }
       ////////
        Mat im;
        im=img;
         //cvNot(img,img);
           cvtColor(im, im,cv::COLOR_BGR2YCrCb);
       //////////////

        cvtColor(im, im,cv::COLOR_BGR2RGB);
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
          // cvNot(img, img);
           //  cvNamedWindow("MyWindow");
           //  cvShowImage("MyWindow", img);
    //çevirme
ui->label->setPixmap(QPixmap::fromImage(ima));
    }


 // //////////////////////////////////////////////////////////////////////////
     if(ui->list->item(11)==item)
    {
        int width=image.toImage().width();
        int height=image.toImage().height();
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
            {
                for(int x=0;x<width;x++ )
                {
                    QRgb color = image.toImage().pixel(x,y);
                    cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
                }
            }
       ////////////
        Mat im;
        im=img;
         //cvNot(img,img);
           cvtColor(im, im,cv::COLOR_RGB2XYZ);
       //////////////

        cvtColor(im, im,cv::COLOR_BGR2RGB);
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
          // cvNot(img, img);
           //  cvNamedWindow("MyWindow");
           //  cvShowImage("MyWindow", img);
    //çevirme
ui->label->setPixmap(QPixmap::fromImage(ima));
    }



 //    /////////////////////////////////////////////////////////////////////////
     if(ui->list->item(12)==item)
         {
             int width=image.toImage().width();
             int height=image.toImage().height();
             IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

             for(int y=0;y<height;y++)
                 {
                     for(int x=0;x<width;x++ )
                     {
                         QRgb color = image.toImage().pixel(x,y);
                         cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
                     }
                 }
            ////////////////////
             Mat im;
             im=img;

              //cvNot(img,img);
                //cvtColor(im,im,cv::COLOR_RGB2GRAY);
             Mat kernel = Mat::zeros(9,9,CV_32F)/9;
             kernel.at<float>(0,0)= 3;
             kernel.at<float>(0,1)= -2;
             kernel.at<float>(0,2)= 3;
             kernel.at<float>(2,0)= 3;
             kernel.at<float>(2,1)= -2;
             kernel.at<float>(2,2)= 3;
             filter2D(im,im,-1,kernel);
            ////////////////////////////////////

             cvtColor(im, im,cv::COLOR_BGR2RGB);
             QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
               // cvNot(img, img);
                //  cvNamedWindow("MyWindow");
                //  cvShowImage("MyWindow", img);
         //çevirme
     ui->label->setPixmap(QPixmap::fromImage(ima));

         }



    ////////////// /////////////////////////////////////////////////////////////////////

   // //////////////////////////////////////////////////////////////////////


    if(ui->list->item(13)==item)
        {
            int width=image.toImage().width();
            int height=image.toImage().height();
            IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

            for(int y=0;y<height;y++)
                {
                    for(int x=0;x<width;x++ )
                    {
                        QRgb color = image.toImage().pixel(x,y);
                        cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
                    }
                }
           ////////////////////
            Mat im;
            im=img;

             //cvNot(img,img);
               //cvtColor(im,im,cv::COLOR_RGB2GRAY);
            Laplacian(im,im,-1);
           ////////////////////////////////////

            cvtColor(im, im,cv::COLOR_BGR2RGB);
            QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
              // cvNot(img, img);
               //  cvNamedWindow("MyWindow");
               //  cvShowImage("MyWindow", img);
        //çevirme
    ui->label->setPixmap(QPixmap::fromImage(ima));
        }
}

void MainWindow::on_actionExport_triggered()
//secilen resmi kayıt etme

{
    QString file = QFileDialog::getSaveFileName(this,
                                                "Save as...",
                                                "image",
                                                "PNG (*.png);; BMP (*.bmp);;TIFF (*.tiff *.tif);; JPEG (*.jpg *.jpeg)");

    ui->label->grab().save(file);
    if(file.isNull())
            QMessageBox::about(this, tr("Uyarı"),
                             tr("<font color=red>Dosya kaydedilmedi...</font>"));
        else
            QMessageBox::about(this, tr("Uyarı"),
                         tr("Dosya kaydedildi.."));

}

void MainWindow::on_actionAbout_triggered()//hakkinda kısmı
{
    QMessageBox::about(this, tr("Uygulama Hakkında"),
              tr("Bu <b>Uygulama</b> ile yüklediğiniz resme "
                 "listeden istediğiniz filtreyi uygulayabilirsiniz "
                 ));
}

void MainWindow::on_actionInstagram_triggered()
{
   // ui->actionBack->setEnabled(true);
   // ui->menuShare->setEnabled(false);
   // ui->actionOpen->setEnabled(false);
  window = new QWidget;

   // QPushButton *button1 = new QPushButton("One");
   // QLabel *lkullanici=new QLabel("Kullanici Adi:\n");
   // QLabel *lsifre=new QLabel("Sifre:\n");
    giris=new QPushButton("Login");
    cikis=new QPushButton("Cancel");
    lk=new QLineEdit();
    ls=new QLineEdit();
   // QTextStream *t=new QTextStream();
    ls->setEchoMode(QLineEdit::Password);//şifreli yazma
    QFormLayout *layout = new QFormLayout;
    //layout->addWidget(button1);
    layout->addRow(tr("&Kullanici Adi:"),lk);
   // layout->addWidget(lkullanici);
   // layout->addWidget(lk);
   // layout->addWidget(lsifre);
   // layout->addWidget(ls);
    layout->addRow(tr("&Sifre:"),ls);
    //layout->addWidget(giris);
    //layout->addWidget(cikis);
    layout->addRow(cikis,giris);


   //  layout->addWidget(t);


   //
    //setCentralWidget(window);//diğer pencerenin yerine gecti
    window->resize(350,100);
    window->setWindowTitle("Giris Yapin");
    window->setLayout(layout);
    window->show();
    connect(giris, SIGNAL(clicked()), this, SLOT(instagramGiris()));
    connect(cikis, SIGNAL(clicked()), window, SLOT(close()));
   // connect(lk,SIGNAL(cursorPositionChanged(int,int)),window,SLOT(lk->setText("");));


}
void MainWindow::instagramGiris()
{
    QString pin = "";
    QString id = "";
    w2 = new QWidget;
    QFormLayout *layo = new QFormLayout;
    QLabel *ll=new QLabel("merve");
    layo->addRow(ll);
    w2->setLayout(layo);
    ll->setPixmap(ui->label->grab());//o anki filtrelenen resmi alabilme
    w2->show();

    ////lineeditlerin icerigine ulasmaa
    id =lk->text();
    pin =ls->text();

    if(id.isEmpty() or pin.isEmpty())
    {
        QMessageBox::about(this, tr("Uyari"),
                  tr("<font color=red>Boş alanlari doldurunuz... </font>"
                    "  "
                     ));
       // lk->text()="aaa";
        //lk->setText("Bu bilgi gereklidir.");


    }

     QMessageBox::information(this,tr("File Name"),id);
}

void MainWindow::on_actionBack_triggered()
{


}

void MainWindow::on_actionBluetooth_triggered()
{


    window2=new QWidget;

    liste = new QListWidget;

    QVBoxLayout *layout = new QVBoxLayout;
    {
        layout->addWidget(liste);
        liste->setMaximumWidth(500);
        liste->setMaximumHeight(300);

        giris2 = new QPushButton;
        cikis2 = new QPushButton;


        QFormLayout *layout2 = new QFormLayout;

        layout2->addRow(giris2,cikis2);
        giris2->setText("İleri");
        cikis2->setText("İptal");

        //buton1->setMaximumWidth(100);
        cikis2->setMaximumWidth(100);
        layout2->setContentsMargins(350,0,0,0);
        layout->addLayout(layout2);

    }

    window2->setLayout(layout);
    window2->resize(500,350);
    window2->setWindowTitle("Bluetooth");
    window2->show();

    connect(giris2, SIGNAL(clicked()), this,SLOT(deviceDiscovered()) );
}
void MainWindow::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Found new device:" << device.name() << '(' << device.address().toString() << ')';
}




void MainWindow::on_actionTake_Photo_triggered()
{

    QMessageBox::about(this, tr("Uyari"),
                 tr("<font color=red>Lütfen fotoğrafi çekmek istediğinizde enter tuşuna basınız.. </font>"
                   "  "
                    ));


    cvNamedWindow("goruntu", CV_WINDOW_NORMAL );

    CvCapture* video=cvCaptureFromCAM(0);
    cvSetCaptureProperty( video, CV_CAP_PROP_FRAME_WIDTH, 256 );
    cvSetCaptureProperty( video, CV_CAP_PROP_FRAME_HEIGHT, 256 );

    if(video==NULL)
    {
        printf("Dosya okunamadi... \n");
    }
    IplImage * frame;

        while(1)
        {
                frame = cvQueryFrame(video);

                if ( !(frame)  )
                        break;
                cvShowImage( "goruntu", frame);
                char c = cvWaitKey(30);
                if ( c == 13 )

                        break;
        }
        //printf("Okuma islemi bitmistir.\n");
       // cvvResizeWindow("goruntu",256,256);
        cvSize(256,256);
        cvDestroyWindow( "goruntu" );
        cvWaitKey(0);


        Mat im=frame;
        //
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
          // cvNot(img, img);
           //  cvNamedWindow("MyWindow");
            //cvShowImage("MyWindow", img);
    //çevirme

            ui->label->setPixmap(QPixmap::fromImage(ima));

            image = QPixmap::fromImage(ima);
            cvReleaseCapture( &video );
           //  cv::resize(image,image,256,256,0.5,0.5);


            ui->actionReset->setEnabled(true);
            ui->actionExport->setEnabled(true);
            ui->actionExit->setEnabled(true);
            ui->menuShare->setEnabled(true);
            ui->list->setEnabled(true);

}
