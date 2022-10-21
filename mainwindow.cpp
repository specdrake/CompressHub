#include "mainwindow.h"
#include "huffman.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <string>
#include <unordered_map>
#include <QDesktopServices>
#include "imageviewer.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->plainTextEdit);
    QLabel *label = new QLabel(statusBar());
    label->setText("Made By Anurag Hooda");
    label->setGeometry(1420,0,180,20);
    this->setWindowTitle("HuffPad");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
   currentFile.clear();
   setWindowTitle("New File");
   ui->plainTextEdit->setPlainText(QString());
   statusBar()->showMessage(tr("New File"), 2000);
}

void MainWindow::on_actionOpen_triggered()
{
   QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
   QFile file(fileName);
   if(!file.open(QIODevice::ReadOnly | QFile::Text)){
       QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
       return;
   }
   currentFile = fileName;
   setWindowTitle(fileName);
   QTextStream in(&file);
   QString text = in.readAll();
   ui->plainTextEdit->setPlainText(text);
   file.close();
   statusBar()->showMessage(QString("Opened ") + currentFile, 2000);
}

void MainWindow::on_actionSave_triggered()
{
   QString fileName = QFileDialog::getSaveFileName(this, "Save the file as");
   QFile file(fileName);
   if(!file.open(QFile::WriteOnly | QFile::Text)){
       QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
       return;
   }
   currentFile = fileName;
   setWindowTitle(fileName);
   QTextStream out(&file);
   QString text = ui->plainTextEdit->toPlainText();
   out << text;
   file.close();
}

void MainWindow::on_actionPrint_triggered()
{
   QPrinter printer;
   printer.setPrinterName("Printer name");
   QPrintDialog pDialog(&printer, this);
   if(pDialog.exec() == QDialog::Rejected){
       QMessageBox::warning(this, "Warning", "Cannot access printer");
   }
   ui->plainTextEdit->print(&printer);
}

void MainWindow::on_actionExit_triggered()
{
   QApplication::quit();
}

void MainWindow::on_actionCopy_triggered()
{
   ui->plainTextEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
   ui->plainTextEdit->paste();
}

void MainWindow::on_actionCut_triggered()
{
   ui->plainTextEdit->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->plainTextEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
   ui->plainTextEdit->redo();
}

void MainWindow::on_actionAuthors_triggered()
{
    QMessageBox::about(this, "About Us", "Anurag Hooda - 2K19/CO/070 && Aadi Rawat - 2K19/CO/002");
}

void MainWindow::on_actionAbout_Qt_triggered()
{
   QApplication::aboutQt();
}

void MainWindow::on_actionCompress_triggered()
{
//       QString fileName = QFileDialog::getSaveFileName(this, "Save the file as");
//       QString fileName =currentFile + "_compressed.bin";
//       QFile file(fileName);
//       if(!file.open(QFile::WriteOnly | QFile::Text)){
//           QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
//           return;
//       }
//       currentFile = fileName;
//       setWindowTitle(fileName);
//       QTextStream out(&file);
//       QString text = ui->plainTextEdit->toPlainText();
//       QString hufftext = QString::fromStdString(buildHuffmanTree(text.toStdString()));
//       out << hufftext;
//       file.close();
       QString x = "_compressed.bin";
       QString y = "_tree.bin";
       QString z = "Saved as ";
       std::string inputName = currentFile.toStdString();
       std::string outputName = currentFile.toStdString() + x.toStdString();
       std::string treeName = currentFile.toStdString() + y.toStdString();
       auto lastpos1 = inputName.find_last_of('/');
       auto lastpos2 = outputName.find_last_of('/');
       auto lastpos3 = treeName.find_last_of('/');
       inputName = inputName.substr(lastpos1+1, inputName.length() - lastpos1);
       outputName = outputName.substr(lastpos2+1, outputName.length() - lastpos2);
       treeName = treeName.substr(lastpos3+1, treeName.length() - lastpos3);
       QMessageBox::information(this, "Compressing Text File!", QString::fromStdString(inputName));
//       hcompress(compressedName, treeName);
       hcompress(inputName, outputName, treeName, true);
//       decompress("binary.bin", "tree.bin", "decompressed.txt");
       QMessageBox::information(this, "Compressed Successfully!", QString::fromStdString(z.toStdString() + outputName));
       statusBar()->showMessage(QString("Compressed ") + currentFile, 2000);
       on_actionOpen_triggered();
}

void MainWindow::on_actionDecompress_triggered()
{
       QString x = "_decompressed.bin";
       QString y = "_tree.bin";
       QString z = "Saved as ";
       std::string inputName = currentFile.toStdString();
//       std::string outputName = currentFile.toStdString() + x.toStdString();
       std::string outputName = currentFile.toStdString().substr(0,currentFile.toStdString().length()-15) + x.toStdString();
       std::string treeName = currentFile.toStdString().substr(0, currentFile.toStdString().length()-15) + y.toStdString();
       std::cout << outputName << ", " << treeName << ", " << inputName << std::endl;
       auto lastpos1 = inputName.find_last_of('/');
       auto lastpos2 = outputName.find_last_of('/');
       auto lastpos3 = treeName.find_last_of('/');
       inputName = inputName.substr(lastpos1+1, inputName.length() - lastpos1);
       outputName = outputName.substr(lastpos2+1, outputName.length() - lastpos2-5);
       treeName = treeName.substr(lastpos3+1, treeName.length() - lastpos3);
        QMessageBox::information(this, "Decompressing Text File" , QString::fromStdString(inputName));
//       QMessageBox::warning(this, "Deompressing this file!", QString::fromStdString(outputName + treeName));
//       hcompress(compressedName, treeName);
//       hcompress(inputName, outputName, treeName);
       hdecompress(inputName, treeName, outputName, true);
        QMessageBox::information(this, "Decompressed Successfully!" , QString::fromStdString(z.toStdString() + outputName));
//       QMessageBox::warning(this, "Deompressed Successfully!", QString::fromStdString(z.toStdString() + outputName));
       statusBar()->showMessage(QString("Decompressed ") + currentFile, 2000);
       on_actionOpen_triggered();
}

void MainWindow::on_actionMail_triggered()
{
   QMessageBox::information(this, "Preparing to send mail!", "................");
//    QDesktopServices::openUrl(QUrl("https://gmail.com"));
    QDesktopServices::openUrl(QUrl("mailto:example@example.com?subject=Compressed%20file&body=" + ui->plainTextEdit->toPlainText()));
}

void MainWindow::on_actionCompress_Image_triggered()
{
   QString x = "_compressed.jpg";
   QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
   QFile file(fileName);
   if(!file.open(QIODevice::ReadOnly | QFile::Text)){
       QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
       return;
   }
   std::string inputName = fileName.toStdString();
   auto lastpos1 = inputName.find_last_of('/');
   inputName = (inputName.substr(lastpos1+1, inputName.length() - lastpos1 - 5));
   fileName = QString::fromStdString(inputName);
   QString ofname = fileName + x;
   fileName = fileName + QString(".tga");

   QMessageBox::information(this, "Compressing Image File", fileName);
//   auto lastpos2 = ofname.find_last_of('/');
//   auto lastpos3 = treeName.find_last_of('/');
//   inputName = inputName.substr(lastpos1+1, inputName.length() - lastpos1);
//   outputName = outputName.substr(lastpos2+1, outputName.length() - lastpos2);
//   treeName = treeName.substr(lastpos3+1, treeName.length() - lastpos3);

   QString command = "./compressimg " + fileName + QString(" ") +  ofname + QString(" 100");
   std::string command_str = command.toStdString();
   std::cout << command_str << std::endl;
//   system("bash");
   system("cd /home/anurag/Workspace/QtSpace/build-CompressHub-Desktop-Debug/");
//   system("\nls\n");
//   system(command_str);
   system(command_str.c_str());
//   system("exit");
//   char   psBuffer[128];
//   FILE   *pPipe;

//   if( (pPipe = popen( reinterpret_cast<const char *>(&command_str), "rt")) == NULL )
//       exit( 1 );
//   while(fgets(psBuffer, 128, pPipe)) {
//       printf(psBQPixmap pixmap(":/path/to/your/image.jpg");
//   }

//   if (feof( pPipe))
//       printf( "\nProcess returned %d\n", pclose( pPipe ) );

   QMessageBox::information(this, "Compressed Successfully!", fileName);
   on_actionOpen_triggered();
}

void MainWindow::on_actionImage_Mode_triggered()
{
   //TODO open image viewer
    auto* window = new ImageViewer();
    window->show();
    this->close();

}
