#include "widget.h"
#include "ui_widget.h"
#include <QByteArray>
#include <QPixmap>
#include <QString>
extern struct fileinfolink info_link;
extern struct dirent **namelist;

void *pth2_work(void * arg);

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    ui->setupUi(this);
    this->show();
    this->resize(900,600);
    this->setWindowFlags(Qt::CustomizeWindowHint);

    myimage[0]="/home/lzx/mp_music/image/02.jpg";
    myimage[1]="/home/lzx/mp_music/image/01.png";
    myimage[2]="/home/lzx/mp_music/image/03.jpg";
    myimage[3]="/home/lzx/mp_music/image/04.jpg";

    myimage_num=0;

    pthread_mutex_init(&mutex,NULL);
    myclock=true;
    mymute=true;
    slider2_time=true;
    mylength=1000;
    mytime=0;
    mylist_search=true;

    fd=open("/home/lzx/mp_music/fifo",O_RDWR);
    if(fd<0)
        qDebug()<<"open error";

    label_tabel.setParent(this);
    label_tabel.resize(this->width(),this->height());
    label_tabel.setPixmap(QPixmap("/home/lzx/mp_music/image/02.jpg"));
    label_tabel.setScaledContents(true);
    label_tabel.show();

    button0.resize(50,50);
    button0.setParent(this);
    button0.show();
    button0.move(70,400);
    button0.setFlat(true);
    button0.setIcon(QIcon("/home/lzx/mp_music/image/prev.png"));
    button0.setIconSize(QSize(25,25));

    button1.resize(50,50);
    button1.setParent(this);
    button1.show();
    button1.move(140,400);
    button1.setFlat(true);
    button1.setIcon(QIcon("/home/lzx/mp_music/image/zt13.png"));
    button1.setIconSize(QSize(25,25));

    button2.resize(50,50);
    button2.setParent(this);
    button2.show();
    button2.move(210,400);
    button2.setFlat(true);
    button2.setIcon(QIcon("/home/lzx/mp_music/image/next.png"));
    button2.setIconSize(QSize(25,25));

    button_close.resize(50,50);
    button_close.setParent(this);
    button_close.show();
    button_close.move(850,0);
    button_close.setFlat(true);
    button_close.setIcon(QIcon("/home/lzx/mp_music/image/exit.png"));
    button_close.setIconSize(QSize(25,25));

    button_mute.resize(20,20);
    button_mute.setParent(this);
    button_mute.show();
    button_mute.move(285,450);
    button_mute.setFlat(true);
    button_mute.setIcon(QIcon("/home/lzx/mp_music/image/mute0.png"));
   // button_mute.setIconSize(QSize(25,25));

    button_change.resize(50,50);
    button_change.setParent(this);
    button_change.show();
    button_change.move(850,50);
    button_change.setFlat(true);
    button_change.setIconSize(QSize(50,100));
    button_change.setIcon(QIcon("/home/lzx/mp_music/image/change.png"));

    slider1.setParent(this);
    slider1.setMinimum(0);
    slider1.setMaximum(100);
    slider1.setValue(50);
    slider1.show();
    slider1.move(280,360);

    slider2.setParent(this);
    slider2.setMinimum(0);
    slider2.setMaximum(100);
    slider2.setOrientation(Qt::Horizontal);
    slider2.setValue(50);
    slider2.resize(700,15);
    slider2.show();
    slider2.move(100,500);
    slider2.setTracking(false);
    slider2.setSingleStep(1);
    slider2.setPageStep(0);

    label_volume.setParent(this);
    label_volume.resize(this->width()/30,this->height()/30);
    label_volume.setText("50");
    label_volume.show();
    label_volume.move(282,340);

    label_length.setParent(this);
    label_length.resize(40,40);
    label_length.setText("nihao");
    label_length.show();
    label_length.move(810,485);

    label_time.setParent(this);
    label_time.resize(40,40);
    label_time.setText("nihao");
    label_time.show();
    label_time.move(60,485);

    label_title.setParent(this);
    label_title.resize(200,30);
    label_title.setText("title");
    label_title.show();
    label_title.move(200,80);

    label_filename.setParent(this);
    label_filename.resize(200,30);
    label_filename.setText("title");
    label_filename.show();
    label_filename.move(200,50);

    QFont ft;
    ft.setPointSize(12);
    label_lrc[3].setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    label_lrc[3].setPalette(pa);

    for(int i=0;i<7;i++)
    {
        label_lrc[i].setParent(this);
        label_lrc[i].move(550,i*60);
    }

    fileline.setParent(this);
    fileline.show();
    fileline.move(30,60);

    music_list.setParent(this);
    music_list.show();
    music_list.move(30,100);
    //music_list.colorCount(256);
    //music_list.
    music_list.setStyleSheet("background-color:transparent");
    music_list.setFrameShape(QListWidget::NoFrame);
    music_list.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    music_list.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    search.setParent(this);
    search.show();
    search.resize(150,30);
    search.move(30,30);
    //search.setEditable(true);


    connect(&button0,SIGNAL(clicked()),this,SLOT(pushButton_0_clicked()));
    connect(&button1,SIGNAL(clicked()),this,SLOT(pushButton_1_clicked()));
    connect(&button2,SIGNAL(clicked()),this,SLOT(pushButton_2_clicked()));
    connect(&button_close,SIGNAL(clicked()),this,SLOT(myclose()));
    connect(&button_change,SIGNAL(clicked()),this,SLOT(pushButton_change_clicked()));


    connect(&slider1,SIGNAL(valueChanged(int)),this,SLOT(set_value_slider(int)));
    connect(&slider2,SIGNAL(sliderPressed()),this,SLOT(slider2_press()));
    connect(&slider2,SIGNAL(valueChanged(int)),this,SLOT(set_value_slider2(int)));


    connect(&button_mute,SIGNAL(clicked()),this,SLOT(pushButton_mute_clicked()));

    connect(&fileline,SIGNAL(editingFinished()),this,SLOT(line_edit()));

    connect(&music_list,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(list_widget_click(QListWidgetItem*)));

    connect(&music_list,SIGNAL(clicked(QModelIndex)),this,SLOT(list_single_click(QModelIndex)));

    connect(&search,SIGNAL(textChanged(QString)),this,SLOT(search_text_change(QString)));

    play_init();

}
void Widget::set_mylist()
{
    music_list.clear();
    QStringList music_name;
    QListWidgetItem *item;
    while(info_link.size<info_link.capacity)
    {
        /*music_name.append(info_link.current->name.section('/',4,4));
        qDebug()<<info_link.current->name<<info_link.size<<info_link.capacity;*/
        item=new QListWidgetItem(info_link.current->name.section('/',4,4),&music_list);
        item->setBackgroundColor(QColor(0,255,255,0));
        info_link.current=info_link.current->next;
        info_link.size++;
    }
    info_link.current=info_link.head;
    info_link.size=0;
}

void Widget::list_single_click(QModelIndex index)
{
    music_list.setCurrentRow(index.row());
    if(mylist_search==false)
    {
        QListWidgetItem*item=music_list.currentItem();
        char buf[256];
        sprintf(buf,"loadfile %s\n",item->text().toLocal8Bit().data());
        qDebug()<<buf;
        mylist_search=true;
    }
}

void Widget::search_text_change(QString str)
{
    //qDebug()<<str<<"search";
    if(str.isEmpty()==1)
    {
        set_mylist();
    }
    else
    {
    music_list.clear();
    QListWidgetItem *item;
    while(info_link.size<info_link.capacity)
    {
        if(info_link.current->name.contains(str,Qt::CaseSensitive)==true)
        {
            item=new QListWidgetItem(info_link.current->name.section('/',4,4),&music_list);
            item->setBackgroundColor(QColor(0,255,255,0));
        }
        info_link.current=info_link.current->next;
        info_link.size++;
    }
    info_link.current=info_link.head;
    info_link.size=0;
    //music_list.insertItems(0,music_name);
    music_list.setCurrentRow(0);
    mylist_search=false;
    }
}

void Widget::myclose()
{
    FILE *fp=fopen("/home/lzx/mp_music/a.txt","w+");
    if(fp==NULL)
        qDebug()<<"fail open";
    char buf[256];
    sprintf(buf,"%s",info_link.current->name.toLocal8Bit().data());
    fputs(buf,fp);
    fclose(fp);
    qDebug()<<buf<<"fail open";
    this->close();
}


void Widget::pushButton_change_clicked()
{
    myimage_num=myimage_num+1;
    if(myimage_num==4)
        myimage_num=0;
    label_tabel.setPixmap(QPixmap(myimage[myimage_num]));
}

void Widget::play_init()
{
    myclock=true;
    mymute=true;
    slider2_time=false;
    mylength=1000;
    mytime=0;
    cut_song=false;
    /*char loadfile[256];
    char *ch=info_link.current->name.toLocal8Bit().data();
    sprintf(loadfile,"loadfile %s\n",ch);*/
    char length[256]="get_time_length\n";
    char time[256]="get_time_pos\n";
    char volume[256];
    sprintf(volume,"volume %d 1\n",slider1.value());
    char title[256]="get_meta_title\n";
    pthread_mutex_lock(&mutex);
    int ret=write(fd,length,strlen(length));
        ret=write(fd,time,strlen(time));
        ret=write(fd,volume,strlen(volume));
       // ret=write(fd,loadfile,strlen(loadfile));
        //ret=write(fd,time,strlen(time));
    pthread_mutex_unlock(&mutex);
    for(int i=0;i<7;i++)
    {
        label_lrc[i].clear();
    }
    slider2_time=true;
}



Widget::~Widget()
{
    delete ui;
}

void Widget::closemywidget()
{
    this->close();
    qDebug()<<"aaaaaaaaaaaaaaaaaaaaaaaaaa";
}

void Widget::list_widget_click(QListWidgetItem *item)
{
    int num;
    num=music_list.currentRow();
    if(num>info_link.size)
    {
        while(info_link.size!=num-1)
        {
            info_link.current=info_link.current->next;
            info_link.size++;
        }
        pushButton_2_clicked();
    }
    else if(num<info_link.size)
    {
        while(info_link.size!=num+1)
        {
            info_link.current=info_link.current->prev;
            info_link.size--;
        }
        pushButton_0_clicked();
    }
}

void Widget::line_edit()
{

    QString str=fileline.text();
    qDebug()<<str;
    catch_fileinfo1(str);
    clear_repeat();
    set_mylist();
    fileline.clear();
}


void Widget::slider2_press()
{
    if(myclock==true)
    slider2_time=false;
}

void Widget::set_title_label(QString str)
{
    QString buf;
    buf=str.section('/',4,4);
    label_title.setText(buf);
    //qDebug()<<info_link.current->name;
    //label_filename.
    label_filename.setText(info_link.current->name.section('/',4,4).toLocal8Bit());
    //label_title.setText(str);
}

/*
 *move slider2
*/
void Widget::set_value_slider2(int pos_value)
{
    int pos=pos_value;
    if(slider2_time==false&&myclock==true)
    {
        if(pos!=mytime)
        {
            pos=pos-mytime;
            char buf[256];
            sprintf(buf,"seek %d\n",pos);
            pthread_mutex_lock(&mutex);
            write(fd,buf,strlen(buf));
            pthread_mutex_unlock(&mutex);
            //set_time_value(mytime+pos);
            slider2.setValue(mytime+pos);
        }
        usleep(100000);
        slider2_time=true;
        mytime=pos_value;
        qDebug()<<mytime;
    }
    if(pos_value>=mylength-1)
    {
        pushButton_2_clicked();
    }
}
/*
 *mute
*/
void  Widget::pushButton_mute_clicked()
{
    if(myclock==true)
    {
        char buf[128];
        if(mymute==true)
        {
            char tmp[]="mute 1\n";
            strncpy(buf,tmp,strlen(tmp));
            slider1.hide();
            label_volume.hide();
            mymute=false;
            button_mute.setIcon(QIcon("/home/lzx/mp_music/image/mute1.png"));
        }
        else
        {
            char tmp[]="mute 0\n";
            strncpy(buf,tmp,strlen(tmp));
            slider1.show();
            label_volume.show();
            mymute=true;
            button_mute.setIcon(QIcon("/home/lzx/mp_music/image/mute0.png"));
        }
        //qDebug()<<buf;
        pthread_mutex_lock(&mutex);
        int ret=write(fd,buf,strlen(buf));
        if(ret<0)
            qDebug()<<"write mute error";
        pthread_mutex_unlock(&mutex);
    }
}
/*
 *slider2 label and value
*/
void Widget::set_time_value(int value)
{
    //qDebug()<<value<<mytime<<mylength<<myclock<<slider2_time;
    if(value>=mylength-1)
    {
        pushButton_2_clicked();
    }
        if(slider2_time==true)
        {
            mytime=value;
            QString buf;
           // qDebug()<<"time now"<<value;
            slider2.setValue(value);
            int a=value/60;
            int b=value%60;
            if(b<10)
            {
                char tmp[128];
                sprintf(tmp,"%d:0%d",a,b);
                buf=tmp;
            }
            else
            {
                char tmp[128];
                sprintf(tmp,"%d:%d",a,b);
                buf=tmp;
            }
            label_time.setText(buf);
        }
}

void Widget::catch_length_value(int value)
{
    qDebug()<<value;
    mylength=value;
    slider2.setMaximum(mylength);
    char buf[128];
    int a=value/60;
    int b=value%60;
    //qDebug()<<a<<b;
    if(b<10)
    {
        sprintf(buf,"%d:0%d",a,b);
    }
    else
    {
        sprintf(buf,"%d:%d",a,b);
    }
    //qDebug()<<buf;
    label_length.setText(buf);
}
void Widget::set_value_slider(int value)
{
    //int value=slider1.value();
    if(myclock==true)
    {
        char buf[256];
        sprintf(buf,"volume %d 1\n",value);
        //qDebug()<<buf;
        pthread_mutex_lock(&mutex);
        int ret=write(fd,buf,strlen(buf));
        if(ret<0)
            qDebug()<<"open error";
        pthread_mutex_unlock(&mutex);
        sprintf(buf," %d",value);
        label_volume.setText(buf);
    }
}



void Widget::mousePressEvent(QMouseEvent *event)
{
    //if(event->MouseButtonPress==Qt::LeftButton)
    if(event->x()<700&&event->y()>0&&event->y()<150)
        movemo=event->globalPos()-this->geometry().topLeft();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->x()<700&&event->y()>0&&event->y()<150)
        this->move(event->globalPos()-movemo);
    //qDebug()<<event->x()<<event->y();
}

void  Widget::catch_pid(int pid)
{
    this->pid=pid;
}

void Widget::pushButton_0_clicked()
{
    if(info_link.current!=info_link.head&&myclock)
    {
        info_link.current=info_link.current->prev;
        info_link.size--;
        QByteArray ba = info_link.current->name.toAscii();
        char buf[256];
        char *ch=ba.data();
        sprintf(buf,"loadfile %s\n",ch);
        pthread_mutex_lock(&mutex);
        int ret=write(fd,buf,strlen(buf));
        if(ret<0)
            qDebug()<<"open error";
        pthread_mutex_unlock(&mutex);
        play_init();
        cut_song=false;
        music_list.setCurrentRow(info_link.size);
    }
}

void Widget::pushButton_1_clicked()
{

    if(myclock==true)
    {
        pthread_mutex_lock(&mutex);
        int ret=write(fd,"pause\n",strlen("pause\n"));
        myclock=false;
        pthread_mutex_unlock(&mutex);
        button1.setIcon(QIcon("/home/lzx/mp_music/image/zangting.png"));
    }
    else if(myclock==false)
    {
        pthread_mutex_lock(&mutex);
        int ret=write(fd,"pause\n",strlen("pause\n"));
        pthread_mutex_unlock(&mutex);
        myclock=true;
        button1.setIcon(QIcon("/home/lzx/mp_music/image/zt13.png"));
        set_value_slider(slider1.value());
    }


}

void Widget::pushButton_2_clicked()                 //next song
{

    if(info_link.current!=info_link.tail&&myclock)
    {
        info_link.current=info_link.current->next;
        info_link.size++;
        QByteArray ba = info_link.current->name.toLocal8Bit();//.toLatin1();
        char buf[256];
        char *ch=ba.data();
        sprintf(buf,"loadfile %s\n",ch);
        qDebug()<<buf;
        pthread_mutex_lock(&mutex);
        int ret=write(fd,buf,strlen(buf));
        if(ret<0)
            qDebug()<<"open error";
        pthread_mutex_unlock(&mutex);
        play_init();
        cut_song=false;
        music_list.setCurrentRow(info_link.size);
    }
}
