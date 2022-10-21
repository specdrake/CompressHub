#include "imageviewer.h"
#include "ui_imageviewer.h"
#include <QScrollArea>
#include <QMessageBox>
#include <QLabel>
#include <QFileDialog>
#include <mainwindow.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "huffman.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
//    scrollArea(new QScrollArea)
{
    ui->setupUi(this);
    this->setWindowTitle("Image View");

//    imageLabel->setBackgroundRole(QPalette::Base);
//    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//    imageLabel->setScaledContents(true);
//    scrollArea->setBackgroundRole(QPalette::Dark);
//    scrollArea->setWidget(imageLabel);
//    scrollArea->setVisible(false);
//    setCentralWidget(scrollArea);
//    createActions();
//    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}
//static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
//{
//    static bool firstDialog = true;

//    if (firstDialog) {
//        firstDialog = false;
//        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
//        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
//    }

//    QStringList mimeTypeFilters;
//    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
//        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
//    for (const QByteArray &mimeTypeName : supportedMimeTypes)
//        mimeTypeFilters.append(mimeTypeName);
//    mimeTypeFilters.sort();
//    dialog.setMimeTypeFilters(mimeTypeFilters);
//    dialog.selectMimeTypeFilter("image/jpeg");
//    if (acceptMode == QFileDialog::AcceptSave)
//        dialog.setDefaultSuffix("jpg");
//}
ImageViewer::~ImageViewer()
{
    delete ui;
}
//bool ImageViewer::loadFile(const QString &fileName)
//{
//    QImageReader reader(fileName);
//    reader.setAutoTransform(true);
//    const QImage newImage = reader.read();
//    if (newImage.isNull()) {
//        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
//                                 tr("Cannot load %1: %2")
//                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
//        return false;
//	}
//}

void ImageViewer::on_actionOpen_Image_triggered()
{
   QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
   QFile file(fileName);
   if(!file.open(QIODevice::ReadOnly | QFile::Text)){
       QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
       return;
   }
   std::string strfname = fileName.toStdString();
   std::string command = "eog " + strfname;
   if(strfname.substr(strfname.length() - 4, 4) == ".tga")
   {
      system(command.c_str());
      currentFile = fileName;
      setWindowTitle(fileName);
      ui->label->setText("Cannot Display This Format Natively!");
      ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
      ui->label->show();
   }
   else {
   currentFile = fileName;
   setWindowTitle(fileName);
   QPixmap pixmap(fileName);
   ui->label->setPixmap(pixmap);
   ui->label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

//   ui->label->setGeometry(0,0,100,200);
//   ui->label->setScaledContents(true);
//   ui->label->setMask(pixmap.mask());
   ui->label->show();
   }
//   QTextStream in(&file);
//   QString text = in.readAll();
//   ui->plainTextEdit->setPlainText(text);
//   file.close();
   long long fsize = filesize(currentFile.toStdString().c_str());
   std::stringstream ss;
   ss << fsize;

   statusBar()->showMessage(QString("Opened ") + currentFile, 2000);
   statusBar()->showMessage(QString("File Size: ") + QString::fromStdString(ss.str()) + QString(" Bytes"), 0);

}

void ImageViewer::on_actionSwitch_to_Text_Mode_triggered()
{
    auto* window = new MainWindow;
    window->show();
    this->close();

}

void ImageViewer::on_actionExit_triggered()
{
   QApplication::quit();
}

void ImageViewer::on_actionCompress_this_Image_triggered()
{
    QString x = "_compressed.jpg";
    QString fileName = currentFile;
    std::string inputName = fileName.toStdString();
    auto lastpos1 = inputName.find_last_of('/');
    inputName = (inputName.substr(lastpos1+1, inputName.length() - lastpos1 - 5));
    fileName = QString::fromStdString(inputName);
    QString ofname = fileName + x;
    fileName = fileName + QString(".tga");

    QMessageBox::information(this, "Compressing Image File", fileName);

    QString command = "./compressimg " + fileName + QString(" ") +  ofname + QString(" 100");
    std::string command_str = command.toStdString();
    std::cout << command_str << std::endl;
    system("cd /home/anurag/Workspace/QtSpace/build-CompressHub-Desktop-Debug/");
    system(command_str.c_str());
        QMessageBox::information(this, "Compressed Successfully!", fileName);
    on_actionOpen_Image_triggered();

}
