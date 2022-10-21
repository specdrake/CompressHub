#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = 0);
    ~ImageViewer();
    bool loadFile(const QString &);


private slots:
    void on_actionOpen_Image_triggered();

    void on_actionSwitch_to_Text_Mode_triggered();

    void on_actionExit_triggered();

    void on_actionCompress_this_Image_triggered();

private:
    QString currentFile = "";
//    void createActions();
//    void createMenus();
//    void updateActions();
//    bool saveFile(const QString &fileName);
//    void setImage(const QImage &newImage);
//    void scaleImage(double factor);
//    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    Ui::ImageViewer *ui;
//    QImage image;
//    QLabel *imageLabel;
//    QScrollArea *scrollArea;
//    double scaleFactor = 1;
};

#endif // IMAGEVIEWER_H
