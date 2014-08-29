/*Bu program ile yüklendiğiniz ya da çektiğiniz  fotoğrafa
 * listede bulunan filtreleri uygulayabilirsiniz.
 * İstediğiniz fotoğrafı istediğiniz yere kaydedebilirsiniz.
 * İstediğiniz fotoğrafı instagramda paylaşabilir veya bluetooth
 * aracılığıyla herhangi bir cihazla paylaşabilirsiniz.
 * Not:Bu programda instagram ve bluetooth kısımları
 * geliştirme aşamasındadır.
 * Bu proje Meltem Akdemir ve Merve Bayrak tarafından geliştirilmiştir.
 * İletişim Bilgileri:
 *  mervebayrak93@gmail.com
 *  akdemir.meltem02@gmail.com
 */


#include "filtreprogrami.h"
#include "ui_filtreprogrami.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QListWidgetItem>
#include <QPushButton>
#include <QProgressBar>
#include <qlineedit.h>
#include <qstring.h>
#include <qlist.h>
#include <qdebug.h>
using namespace cv;
FiltreProgrami::FiltreProgrami(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FiltreProgrami)
{
    ui->setupUi(this);
}

FiltreProgrami::~FiltreProgrami()
{
    delete ui;
}

void FiltreProgrami::on_actionOpen_triggered()
{
    filename=QFileDialog::getOpenFileName(
                this,
                tr("open file"),
                "C://",
                "All Files (*.*);;Image file (*.png *.xpm *.jpg)"
                );

    if(filename.isNull())//Dosya yükleme kontrolu
    {
        QMessageBox::about(this, tr("Uyari"),
                           tr("Dosya Yuklenemedi. "
                              "  "
                              ));
    }
    else
    {
        // QMessageBox::information(this,tr("File Name"),filename);
        //QDesktopServices::openUrl(QUrl("file:///"+filename,QUrl::TolerantMode));


        //resmi ekrana getirmek için
        imageObject = new QImage();
        imageObject->load(filename);

        //Resmin labelde görüntülenebilmesi için Qpixmap 'e dönüşümünün  yapılması
        image = QPixmap::fromImage(*imageObject);
        ui->label->setPixmap(image);

        //Open tuşu tıklandığında pasif olan kısımların aktif yapılması
        ui->actionReset->setEnabled(true);
        ui->actionExport->setEnabled(true);
        ui->actionExit->setEnabled(true);
        ui->menuShare->setEnabled(true);
        ui->list->setEnabled(true);
    }
}


void FiltreProgrami::on_actionExit_triggered()
{
     close();//exit ile çıkış
}

void FiltreProgrami::on_actionTake_Photo_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Fotoğraf Çekmek İçin Kamera Açılsın mı? ");
    QPushButton *abortButton = msgBox.addButton(tr("No"), QMessageBox::ActionRole);
    QPushButton *connectButton = msgBox.addButton(QMessageBox::Yes);
    msgBox.exec();
    if (msgBox.clickedButton() == connectButton)
    {

        //Kameradan görüntü alabilmek için openCV metodlarından yararlanılmıştır.
        cvNamedWindow("Fotograf Cekmek Icin Enter Tusuna Basiniz.", CV_WINDOW_NORMAL );//Kameradan alınan görüntünün gösterilmesini sağlayan pencere
        CvCapture* video=cvCaptureFromCAM(0);//CvCapture türünde pointer tanımlayarak,CvCaptureFromCAM metodu ile
                                             //kameradan görüntünün alınması ve video değişkenine atanması
        //Görüntü boyutunun ayarlanması
        cvSetCaptureProperty( video, CV_CAP_PROP_FRAME_WIDTH, 256 );
        cvSetCaptureProperty( video, CV_CAP_PROP_FRAME_HEIGHT, 256 );
        //Görüntü alma kontrolü
        if(video==NULL)
        {
            QMessageBox::about(this, tr("Uyari"),
                               tr("Goruntu Alınamadı!"
                                  "  "
                                  ));
        }

        IplImage * frame;//Resim saklaması IplImage veri tipinde  için frame değişkeni tanımlandı
        //Sonsuz döngü yardımıyla yakalananan görüntünün içerisinde ki frameler cvQueryFrame yardımıyla
        //tek tek sorgulanıp okutularak enter tuşuna basılana kadar ekranda gösterilmesi sağlanır
        while(1)
        {
            frame = cvQueryFrame(video);
            if ( !(frame)  )
                break;
            cvShowImage( "Fotograf Cekmek Icin Enter Tusuna Basiniz.", frame);
            char c = cvWaitKey(30);
            if ( c == 13 )
                break;
        }
        cvDestroyWindow( "Fotograf Cekmek Icin Enter Tusuna Basiniz." );//Görüntü peceresinin silinmesi
        cvWaitKey(0);//Sonlandırma metodu
        Mat im=frame;//Filtre uygulanabilmesi için IplImage tipindeki frame değişkeninin
        //Mat veri tipine dönüştürülerek im değişkenine atılması
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        //Mat veri tipindeki im değişkeninin Qimage veri tipine dönüştürülerek ima değişkenine atılması
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));//Alınan resmin labelde gösterilmesi için QPixmap veri tipine
                                                      //dönüştürülmesi
        image = QPixmap::fromImage(ima);//Çekilen görüntünün image değişkenine atılması
        cvReleaseCapture( &video );//Video nun bellekten silinmesi
        //Take Photo tuşu tıklandığında pasif olan kısımların aktif yapılması
        ui->actionReset->setEnabled(true);
        ui->actionExport->setEnabled(true);
        ui->actionExit->setEnabled(true);
        ui->menuShare->setEnabled(true);
        ui->list->setEnabled(true);
    }
    else if (msgBox.clickedButton() == abortButton)
        msgBox.close();
}

void FiltreProgrami::on_actionReset_triggered()
{
     ui->label->setPixmap(image);//Labelde resmin orjinal halinin gösterilmesi
}

void FiltreProgrami::on_actionExport_triggered()
{
    QString file = QFileDialog::getSaveFileName(this,
                                                "Save as...",
                                                "image",
                                                "PNG (*.png);; BMP (*.bmp);;TIFF (*.tiff *.tif);; JPEG (*.jpg *.jpeg)");
    ui->label->grab().save(file);//Labeldeki resmin kaydedilmesi

    if(file.isNull())//Resim kayıt kontrolü
        QMessageBox::about(this, tr("Uyarı"),
                           tr("<font color=blue>Dosya Kaydedilmedi!</font>"));
    else
        QMessageBox::about(this, tr("Uyarı"),
                           tr("Dosya Kaydedildi."));
}

void FiltreProgrami::on_actionInstagram_triggered()
{
   //Instagrama giriş için arayüz oluşturuldu
   windowInstagram = new QWidget;
   loginInstagram=new QPushButton("Login");
   cancelInstagram=new QPushButton("Cancel");

   lineUserName=new QLineEdit();
   linePassword=new QLineEdit();
   linePassword->setEchoMode(QLineEdit::Password);//şifreli yazma
   layoutInstagram = new QFormLayout;
   layoutInstagram->addRow(tr("&Kullanici Adi:"),lineUserName);
   layoutInstagram->addRow(tr("&Sifre:"),linePassword);
   layoutInstagram->addRow(cancelInstagram,loginInstagram);
   windowInstagram->resize(350,100);
   windowInstagram->setWindowTitle("Giris Yapin");
   windowInstagram->setLayout(layoutInstagram);
   windowInstagram->show();
   //Login butonu tıklandığında InstagramaGiris fonksiyonunu çağrılması
   //Cancel butonuna tıklandığında windowInstagram ekranının kapatılması
   connect(loginInstagram, SIGNAL(clicked()), this, SLOT(instagramGiris()));
   connect(cancelInstagram, SIGNAL(clicked()), windowInstagram, SLOT(close()));
}
void FiltreProgrami::instagramGiris()
{
    //Instagrama giriş için arayüz oluşturulması
    QString userName = "";
    QString password = "";
    windowInstagramGiris = new QWidget;
    layoutInstagramGiris = new QFormLayout;
    QLabel *labelInstagramGiris=new QLabel("Goruntu");
    layoutInstagramGiris->addRow(labelInstagramGiris);
    windowInstagramGiris->setLayout(layoutInstagramGiris);
    labelInstagramGiris->setPixmap(ui->label->grab());//o anki filtrelenen resmi alabilme
    windowInstagramGiris->show();

    //Girilen kulanıcı adının userName değişkenine, şifrenin password değişkenine atanması
    userName =lineUserName->text();
    password =linePassword->text();

    //Kullanıcı adi ve şifre girilmediği takdirde uyarı verilmesi
    if(userName.isEmpty() or password.isEmpty())
    {
        QMessageBox::about(this, tr("Uyari"),
                           tr("<font color=blue>Boş alanlari doldurunuz... </font>"
                              "  "
                              ));
    }
    else
        //Kullanici adi ve şifrenin ekranda gösterilmesi
        QMessageBox::information(this,tr("Instagram Giriş Bilgileri"),"Kullanici Adi:  "  + userName + " \n " + "Sifre:  "  +password);
}

void FiltreProgrami::on_actionBluetooth_triggered()
{
    //Bluetooth için arayüz oluşturulması
    windowBluetooth = new QWidget;
    listBluetooth1 = new QListWidget;

    layoutBluetooth = new QVBoxLayout;
    {
        layoutBluetooth->addWidget(listBluetooth1);
        listBluetooth1->setMaximumWidth(500);
        listBluetooth1->setMaximumHeight(300);

        forward1= new QPushButton;
        cancelBluetooth1 = new QPushButton;


        butonLayout = new QFormLayout;

        butonLayout->addRow(forward1,cancelBluetooth1);
        forward1->setText("Forward");
        cancelBluetooth1->setText("Cancel");
        cancelBluetooth1->setMaximumWidth(100);
        butonLayout->setContentsMargins(350,0,0,0);
        layoutBluetooth->addLayout(butonLayout);

    }

    windowBluetooth->setLayout(layoutBluetooth);
    windowBluetooth->resize(500,350);
    windowBluetooth->setWindowTitle("Bluetooth");
    windowBluetooth->show();
    connect(forward1,SIGNAL(clicked()), this, SLOT(imageTransfer()));
    connect(cancelBluetooth1, SIGNAL(clicked()),windowBluetooth , SLOT(close()));

}
void FiltreProgrami::imageTransfer()
{
    windowBluetooth->close();
    windowImageTransfer = new QWidget;

    layoutImageTransfer1 = new QVBoxLayout;
    {

        forward2 = new QPushButton;
        cancelBluetooth2 = new QPushButton;
        progresBar=new QProgressBar;
        QLabel *label=new QLabel;
        layoutImageTransfer2 = new QFormLayout;
        lineAygit=new QLineEdit;
        lineAdi=new QLineEdit;
        layoutImageTransfer2->addRow(tr("&Gönderilen Aygıt Adi:"),lineAygit);
        layoutImageTransfer2->addRow(tr("&Gönderilen Dosya Adi:"),lineAdi);
        layoutImageTransfer2->setContentsMargins(0,30,0,0);
        /////////////////////////////////////
        QFormLayout *layoutImageTransfer3=new QFormLayout;
        layoutImageTransfer3->addRow(tr("Gönderiliyor..."),label);
        layoutImageTransfer3->addRow(progresBar);

        progresBar->setContentsMargins(0,0,0,0);
        layoutImageTransfer3->setContentsMargins(60,100,0,0);
        ////////////////////////////////////////////

        QFormLayout *layoutImageTransfer4=new QFormLayout;
        layoutImageTransfer4->addRow(forward2,cancelBluetooth2);

        forward2->setText("Forward");
        cancelBluetooth2->setText("Cancel");

        cancelBluetooth2->setMaximumWidth(100);
        layoutImageTransfer4->setContentsMargins(350,100,0,0);

        layoutImageTransfer1->addLayout(layoutImageTransfer2);
        layoutImageTransfer1->addLayout(layoutImageTransfer3);
        layoutImageTransfer1->addLayout(layoutImageTransfer4);


    }

    windowImageTransfer->setLayout(layoutImageTransfer1);
    windowImageTransfer->resize(500,350);
    windowImageTransfer->setWindowTitle("Bluetooth");
    windowImageTransfer->show();
    connect(cancelBluetooth2, SIGNAL(clicked()),windowImageTransfer , SLOT(close()));
}

void FiltreProgrami::on_actionAbout_triggered()
{
    //Proje hakkında bilgi bu bölümde bulunmakta
    QMessageBox::about(this, tr("Uygulama Hakkında"),
                       tr("<ul><li>Bu <b>Uygulama</b> ile yüklediğiniz resim dosyasına veya"
                          "bilgisayarınızın kamerasından çekilen fotoğrafın üzerine listeden seçtiğiniz herhangi bir filtreyi uygulayabilirsiniz "
                          "listeden istediğiniz filtreyi uygulayabilirsiniz.</li>"
                          "<b>File</b> menusu altındaki;"
                          "<li><b>Reset</b> seçeneği ile resminizin il haline ulaşabilirsiniz.</li>"
                          "<li><b>Export</b> seçeneği ile resminizi istediğiniz yere kaydedebilirsiniz.</li>"
                          "<li><b>Exit</b> seçeneği ile uygulamadan çıkabilirsiniz.\n"
                          "<li><b>Instagram</b> seçeneği ile resminizi instagram sayfasında paylaşabilirsiniz.</li>"
                          "<li><b>Bluetooth</b> seçeneği ile resminizi bluetooth ile başka aygitlara gönderebilirsiniz.</li></ul>"


                          ));
}

void FiltreProgrami::on_list_itemClicked(QListWidgetItem *item)
{
    /////////////////////////////////////Filtre1/////////////////////////////////////////////////////////////////////////////////////////////////
    //Filtre1 e tıklandığında labeldeki resime filtre1 in uygulanması
    if(ui->list->item(0)==item)
    {
        int width=image.toImage().width();//Resmin genişliğinin alınması
        int height=image.toImage().height();//Resmin uzunluğunun alınması
        //image değişkeninde ki resme filtre uygulanabilmesi için IplImage veri tipine dönüştürülmesi gerekir.
        ///////////////////////image in IplImage dönüştürülme işlemleri///////////
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////Filtre1 in Resime  Uygulanması/////////////////////
        Mat im;
        im=img;
        cvtColor(im, im,cv::COLOR_RGB2HSV);
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        ///////////////////////////////////////////////////////////////////////////

        ////Resmin labelde gösterilebilmesi için QPixmap e çevrilmesi/////////////
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    /////////////////////////////////////Filtre2//////////////////////////////////////////////////////////////////////////////////////////////////
    //Filtre2 e tıklandığında labeldeki resime filtre1 in uygulanması
    if(ui->list->item(1)==item)
    {
        int width=image.toImage().width();//Resmin genişliğinin alınması
        int height=image.toImage().height();//Resmin uzunluğunun alınması
        //image değişkeninde ki resme filtre uygulanabilmesi için IplImage veri tipine dönüştürülmesi gerekir.
        ///////////////////////image in IplImage dönüştürülme işlemleri///////////
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////Filtre2 in Resime Uygulanması//////////////////////////////
        Mat im;
        im=img;
        blur(im,im,Size(9,9));
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        ///////////////////////////////////////////////////////////////////////////
        ////Resmin labelde gösterilebilmesi için QPixmap e çevrilmesi/////////////
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));
        ///////////////////////////////////////////////////////////////////////////

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    /////////////////////////////////////Filtre3////////////////////////////////////////////////////////////////////////////////////////////////
    //Filtre3 e tıklandığında labeldeki resime filtre1 in uygulanması
    if(ui->list->item(2)==item)
    {
        int width=image.toImage().width();//Resmin genişliğinin alınması
        int height=image.toImage().height();//Resmin uzunluğunun alınması
        //image değişkeninde ki resme filtre uygulanabilmesi için IplImage veri tipine dönüştürülmesi gerekir.
        ///////////////////////image in IplImage dönüştürülme işlemleri///////////
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////Filtre3 in Resime Uygulanması//////////////////////////////
        Mat im;
        im=img;
        cvtColor(im,im,cv::COLOR_HLS2RGB_FULL);
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        ///////////////////////////////////////////////////////////////////////////


        ////Resmin labelde gösterilebilmesi için QPixmap e çevrilmesi/////////////
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));
        ///////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    /////////////////////////////////////Filtre4/////////////////////////////////////////////////////////////////////////////////////////////////
    //Filtre4 e tıklandığında labeldeki resime filtre1 in uygulanması
    if(ui->list->item(3)==item)
    {
        int width=image.toImage().width();//Resmin genişliğinin alınması
        int height=image.toImage().height();//Resmin uzunluğunun alınması
        //image değişkeninde ki resme filtre uygulanabilmesi için IplImage veri tipine dönüştürülmesi gerekir.
        ///////////////////////image in IplImage dönüştürülme işlemleri///////////
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////Filtre4 in Resime Uygulanması//////////////////////////////
        Mat im;
        im=img;
        cvtColor(im, im,cv::COLOR_RGB2HLS);
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        ///////////////////////////////////////////////////////////////////////////

        ////Resmin labelde gösterilebilmesi için QPixmap e çevrilmesi/////////////
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));
        ///////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    /////////////////////////////////////Filtre5//////////////////////////////////////////////////////////////////////////////////////////////////
    //Filtre5 e tıklandığında labeldeki resime filtre1 in uygulanması
    if(ui->list->item(4)==item)
    {
        int width=image.toImage().width();//Resmin genişliğinin alınması
        int height=image.toImage().height();//Resmin uzunluğunun alınması
        //image değişkeninde ki resme filtre uygulanabilmesi için IplImage veri tipine dönüştürülmesi gerekir.
        ///////////////////////image in IplImage dönüştürülme işlemleri///////////
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////Filtre5 in Resime Uygulanması//////////////////////////////
        Mat im;
        im=img;
        GaussianBlur(im,im,Size(11,11),5);//Resmi Bulanıklaştırma.
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        ///////////////////////////////////////////////////////////////////////////

        ////Resmin labelde gösterilebilmesi için QPixmap e çevrilmesi/////////////
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    /////////////////////////////////////Filtre6//////////////////////////////////////////////////////////////////////////////////////////////////
    //Filtre6 e tıklandığında labeldeki resime filtre1 in uygulanması
    if(ui->list->item(5)==item)
    {
        int width=image.toImage().width();//Resmin genişliğinin alınması
        int height=image.toImage().height();//Resmin uzunluğunun alınması
        //image değişkeninde ki resme filtre uygulanabilmesi için IplImage veri tipine dönüştürülmesi gerekir.
        ///////////////////////image in IplImage dönüştürülme işlemleri///////////
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////Filtre6 in Resime Uygulanması//////////////////////////////
        Mat im;
        im=img;
        cvtColor(im, im,cv::COLOR_YCrCb2BGR);
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        ///////////////////////////////////////////////////////////////////////////

        ////Resmin labelde gösterilebilmesi için QPixmap e çevrilmesi/////////////
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    /////////////////////////////////////Filtre7//////////////////////////////////////////////////////////////////////////////////////////////////
    //Filtre7 e tıklandığında labeldeki resime filtre1 in uygulanması
    if(ui->list->item(6)==item)
    {
        int width=image.toImage().width();//Resmin genişliğinin alınması
        int height=image.toImage().height();//Resmin uzunluğunun alınması
        //image değişkeninde ki resme filtre uygulanabilmesi için IplImage veri tipine dönüştürülmesi gerekir.
        ///////////////////////image in IplImage dönüştürülme işlemleri///////////
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////Filtre7 in Resime Uygulanması//////////////////////////////
        Mat im;
        im=img;
        cvtColor(im, im,cv::COLOR_YCrCb2RGB);
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        ///////////////////////////////////////////////////////////////////////////

        ////Resmin labelde gösterilebilmesi için QPixmap e çevrilmesi/////////////
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    /////////////////////////////////////Filtre8//////////////////////////////////////////////////////////////////////////////////////////////////
    //Filtre8 e tıklandığında labeldeki resime filtre1 in uygulanması
    if(ui->list->item(7)==item)
    {
        int width=image.toImage().width();//Resmin genişliğinin alınması
        int height=image.toImage().height();//Resmin uzunluğunun alınması
        //image değişkeninde ki resme filtre uygulanabilmesi için IplImage veri tipine dönüştürülmesi gerekir.
        ///////////////////////image in IplImage dönüştürülme işlemleri///////////
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////Filtre8 in Resime Uygulanması//////////////////////////////
        Mat im;
        im=img;
        cvtColor(im,im,cv::COLOR_BGR2RGB);
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        ///////////////////////////////////////////////////////////////////////////

        ////Resmin labelde gösterilebilmesi için QPixmap e çevrilmesi/////////////
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    /////////////////////////////////////Filtre9///////////////////////////////////////////////////////////////////////////////////////////////////
    //Filtre9 e tıklandığında labeldeki resime filtre1 in uygulanması
    if(ui->list->item(8)==item)
    {
        int width=image.toImage().width();//Resmin genişliğinin alınması
        int height=image.toImage().height();//Resmin uzunluğunun alınması
        //image değişkeninde ki resme filtre uygulanabilmesi için IplImage veri tipine dönüştürülmesi gerekir.
        ///////////////////////image in IplImage dönüştürülme işlemleri///////////
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////Filtre9 in Resime Uygulanması//////////////////////////////
        Mat im;
        im=img;
        medianBlur(im,im,15);
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        ///////////////////////////////////////////////////////////////////////////

        ////Resmin labelde gösterilebilmesi için QPixmap e çevrilmesi/////////////
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));
        ///////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    /////////////////////////////////////Filtre10///////////////////////////////////////////////////////////////////////////////////////////////////
    //Filtre10 e tıklandığında labeldeki resime filtre1 in uygulanması
    if(ui->list->item(9)==item)
    {
        int width=image.toImage().width();//Resmin genişliğinin alınması
        int height=image.toImage().height();//Resmin uzunluğunun alınması
        //image değişkeninde ki resme filtre uygulanabilmesi için IplImage veri tipine dönüştürülmesi gerekir.
        ///////////////////////image in IplImage dönüştürülme işlemleri///////////
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////Filtre10 in Resime Uygulanması//////////////////////////////
        cvNot(img,img);
        Mat im;
        im=img;
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        ///////////////////////////////////////////////////////////////////////////

        ////Resmin labelde gösterilebilmesi için QPixmap e çevrilmesi/////////////
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));
        ///////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    /////////////////////////////////////Filtre11///////////////////////////////////////////////////////////////////////////////////////////////////
    //Filtre11 e tıklandığında labeldeki resime filtre1 in uygulanması
    if(ui->list->item(10)==item)
    {
        int width=image.toImage().width();//Resmin genişliğinin alınması
        int height=image.toImage().height();//Resmin uzunluğunun alınması
        //image değişkeninde ki resme filtre uygulanabilmesi için IplImage veri tipine dönüştürülmesi gerekir.
        ///////////////////////image in IplImage dönüştürülme işlemleri///////////
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////Filtre11 in Resime Uygulanması//////////////////////////////
        Mat im;
        im=img;
        cvtColor(im, im,cv::COLOR_BGR2YCrCb);
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        ///////////////////////////////////////////////////////////////////////////

        ////Resmin labelde gösterilebilmesi için QPixmap e çevrilmesi/////////////
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));
        ///////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    /////////////////////////////////////Filtre12///////////////////////////////////////////////////////////////////////////////////////////////////
    //Filtre12 e tıklandığında labeldeki resime filtre1 in uygulanması
    if(ui->list->item(11)==item)
    {
        int width=image.toImage().width();//Resmin genişliğinin alınması
        int height=image.toImage().height();//Resmin uzunluğunun alınması
        //image değişkeninde ki resme filtre uygulanabilmesi için IplImage veri tipine dönüştürülmesi gerekir.
        ///////////////////////image in IplImage dönüştürülme işlemleri///////////
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////Filtre12 in Resime Uygulanması//////////////////////////////
        Mat im;
        im=img;
        cvtColor(im, im,cv::COLOR_RGB2XYZ);
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        ///////////////////////////////////////////////////////////////////////////

        ////Resmin labelde gösterilebilmesi için QPixmap e çevrilmesi/////////////
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));
        ///////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    /////////////////////////////////////Filtre13///////////////////////////////////////////////////////////////////////////////////////////////////
    //Filtre13 e tıklandığında labeldeki resime filtre1 in uygulanması
    if(ui->list->item(12)==item)
    {
        int width=image.toImage().width();//Resmin genişliğinin alınması
        int height=image.toImage().height();//Resmin uzunluğunun alınması
        //image değişkeninde ki resme filtre uygulanabilmesi için IplImage veri tipine dönüştürülmesi gerekir.
        ///////////////////////image in IplImage dönüştürülme işlemleri///////////
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////Filtre13 in Resime Uygulanması//////////////////////////////
        Mat im;
        im=img;
        Mat kernel = Mat::zeros(9,9,CV_32F)/9;
        kernel.at<float>(0,0)= 3;
        kernel.at<float>(0,1)= -2;
        kernel.at<float>(0,2)= 3;
        kernel.at<float>(2,0)= 3;
        kernel.at<float>(2,1)= -2;
        kernel.at<float>(2,2)= 3;
        filter2D(im,im,-1,kernel);
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        ///////////////////////////////////////////////////////////////////////////

        ////Resmin labelde gösterilebilmesi için QPixmap e çevrilmesi/////////////
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));
        ///////////////////////////////////////////////////////////////////////////

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    /////////////////////////////////////Filtre14///////////////////////////////////////////////////////////////////////////////////////////////////
    //Filtre14 e tıklandığında labeldeki resime filtre1 in uygulanması
    if(ui->list->item(13)==item)
    {
        int width=image.toImage().width();//Resmin genişliğinin alınması
        int height=image.toImage().height();//Resmin uzunluğunun alınması
        //image değişkeninde ki resme filtre uygulanabilmesi için IplImage veri tipine dönüştürülmesi gerekir.
        ///////////////////////image in IplImage dönüştürülme işlemleri///////////
        IplImage *img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

        for(int y=0;y<height;y++)
        {
            for(int x=0;x<width;x++ )
            {
                QRgb color = image.toImage().pixel(x,y);
                cvSet2D(img,y,x,cvScalar(qBlue(color),qGreen(color),qRed(color),1));
            }
        }
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////Filtre14 in Resime Uygulanması//////////////////////////////
        Mat im;
        im=img;
        Laplacian(im,im,-1);
        cvtColor(im, im,cv::COLOR_BGR2RGB);
        ///////////////////////////////////////////////////////////////////////////

        ////Resmin labelde gösterilebilmesi için QPixmap e çevrilmesi/////////////
        QImage ima( (uchar*)im.data, im.cols, im.rows, im.step, QImage::Format_RGB888);
        ui->label->setPixmap(QPixmap::fromImage(ima));
        ///////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
}
