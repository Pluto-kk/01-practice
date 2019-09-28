#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "catch_fileinfo.h"
#include "sys/types.h"
#include "sys/stat.h"
#include <unistd.h>
#include "fcntl.h"
#include "stdio.h"
#include <string.h>
#include <pthread.h>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QPoint>
#include <QMouseEvent>
#include <QLineEdit>
#include <QListWidget>
#include "mylabel.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void set_title_label(QString);
    void catch_length_value(int);
    void set_time_value(int);
    void catch_pid(int);
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void play_init();
    void set_mylist();
    int catch_time()
    {return mytime;}
    mylabel label_lrc[7];
    int fd;
    bool myclock;
    bool cut_song;
    int mytime;
    bool slider2_time;
    pthread_mutex_t mutex;
    QLabel label_filename;
    QLabel label_title;
private slots:
    void pushButton_0_clicked();
    void pushButton_1_clicked();
    void pushButton_2_clicked();
    void pushButton_mute_clicked();
    void set_value_slider(int);
    void set_value_slider2(int);
    void slider2_press();
    void line_edit();
    void closemywidget();
    void list_widget_click(QListWidgetItem*);
    void pushButton_change_clicked();
    void myclose();
    void search_text_change(QString);
    void list_single_click(QModelIndex);
private:
    Ui::Widget *ui;
    int pid;

    QPushButton button0;
    QPushButton button1;
    QPushButton button2;
    QPushButton button_close;
    QPushButton button_mute;
    QPushButton button_change;
    QSlider slider1;
    QSlider slider2;
    QLabel label_volume;
    QLabel label_tabel;
    QLabel label_length;
    QLabel label_time;


    QLineEdit fileline;
    QString myimage[4];
    int myimage_num;
    QPoint movemo;
    bool mymute;
    int mylength;
    QListWidget music_list;
    QLineEdit search;
    bool mylist_search;
};

#endif // WIDGET_H
