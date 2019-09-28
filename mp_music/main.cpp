#include <QApplication>
#include "catch_fileinfo.h"
#include "widget.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/prctl.h"
#include <unistd.h>
#include "fcntl.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include <QStringList>
#include <QString>
#include <QTextCodec>
#include "song_lrc.h"

bool mysong_lyric=false;
extern struct fileinfolink info_link;
extern char lrc_header[4][LRC_SIZE];//二维数组保存歌词前4行
extern LLIST* song_list;
void lrc_display(void *arg);
NODE * catch_lrc(NODE *, int );
int mycurrtime=0;
bool mylysics=true;
int fdpipe[2];

void task_handle(QString str,void *arg)
{
    Widget *w=reinterpret_cast<Widget *>(arg);
    QStringList list=str.split('=');

    if(list[0].compare("ANS_LENGTH")==0)
    {
        QString buf;
        buf=list[1];
        double a=buf.toDouble();
        int num=static_cast<int>(a);
        w->catch_length_value(num);
    }
    else if(list[0].compare("ANS_TIME_POSITION")==0)
    {
        QString buf;
        buf=list[1];
        double a=buf.toDouble();
        int num=static_cast<int>(a);
        w->set_time_value(num);
    }
}
void handle_signal(int signo)
{
    if (signo == SIGHUP)
    {

        printf("child recv SIGHUP..\n");
        kill(0,SIGINT);
    }
}

void* pth_work(void * arg)
{

    char buf[512];
    while(1)
    {

        close(fdpipe[1]);
        memset(buf,0,sizeof(buf));
        //read(fdpipe[0],buf,strlen(buf));
        dup2(fdpipe[0], STDIN_FILENO);
        fgets(buf, sizeof(buf),stdin);
        QString tmp(buf);
        task_handle(tmp,arg);
        //qDebug()<<buf<<"-task";
        close(fdpipe[0]);
    }
    return arg;
}

void *pth2_work(void * arg)
{
    Widget *w=reinterpret_cast<Widget *>(arg);
    char buf[256]="get_time_pos\n";
    char tmp[256]="get_percent_pos\n";
    while(1)
    {
        usleep(100000);
        if(w->myclock==true)
        {
            pthread_mutex_lock(&(w->mutex));
            int ret=write(w->fd,buf,strlen(buf));
            //qDebug()<<"order"<<buf;
            if(ret<0)
                qDebug()<<"open error";
            ret=write(w->fd,tmp,strlen(tmp));
            if(ret<0)
                qDebug()<<"open error";
            pthread_mutex_unlock(&(w->mutex));
        }
    }
    return arg;
}

NODE * catch_lrc(NODE *p, int i)
{
    while(p!=NULL&&i!=0)
    {
        if(i>0)
        {
            p=p->next;
            i--;
        }
        else if(i<0)
        {
            p=p->prev;
            i++;
        }
    }
    return p;
}

void lrc_display(void *arg)
{
    Widget *w=reinterpret_cast<Widget *>(arg);
    qDebug()<<"i  am  running"<<w->mytime;
    if(song_list->head->next==NULL)
    {
        //qDebug()<<"NULL";
        qDebug()<<"list==NULL";
        return;
    }
    else
    {
        qDebug()<<"adada";
        NODE *ptr=song_list->head->next;
        while(ptr!=NULL)
        {
            if(ptr->times>=w->mytime)
            {

                for(int i=0;i<7;i++)
                {
                    w->label_lrc[i].clear();
                    if(catch_lrc(ptr,i-4)==NULL)
                    {
                        w->label_lrc[i].setText("                   ");
                    }
                    else
                    {
                        /*char buf[256];
                    strcpy(buf,catch_lrc(ptr,i-4)->lrc);
                    char *ch=buf;
                    while(1)
                    {
                        if(*ch=='\n')
                        {
                            *ch=0;
                            break;
                        }
                        ch++;
                    }
                    qDebug()<<buf<<"--------";*/
                        w->label_lrc[i].setText(catch_lrc(ptr,i-4)->lrc);
                    }
                }
                break;
            }
            ptr=ptr->next;
        }}
}

void * pth3_work(void *arg)
{
    Widget *w=reinterpret_cast<Widget *>(arg);
    while(1)
    {
        usleep(500000);
        if(mylysics==true&&w->slider2_time==true)
        {
            lrc_display(arg);
        }
    }
}

void * pth4_work(void *arg)
{
    Widget *w=reinterpret_cast<Widget *>(arg);
    while(1)
    {
        usleep(10000);
        if(w->cut_song==false)
        {
            if(song_list!=NULL)
            {
                NODE *p=song_list->head;
                while(p!=song_list->tail)
                {
                    qDebug()<<p->lrc<<p->times;
                    p=p->next;
                }
            }
            mylysics=false;
            destroy_linklist(song_list);
            mylysics=true;
            QString buf=info_link.current->name.section('/',4,4);
            QStringList list=buf.split('.');
            buf=list[0];
            buf.append(".lrc");
            char tmp[256];
            char *ch=buf.toLocal8Bit().data();
            sprintf(tmp,"/home/lzx/lyrics/%s",ch);
            qDebug()<<tmp<<ch<<"---------";
            if(my_lrc_catch(tmp)==0)
                mysong_lyric=true;
            w->label_filename.setText(lrc_header[0]);
            w->label_title.setText(lrc_header[1]);
            w->cut_song=true;
            qDebug()<<w->cut_song;
        }
    }
}

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");       //UTF-8
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    int ret=mkfifo("/home/lzx/mp_music/fifo",0666|O_CREAT|O_TRUNC);
    if(ret<0)
        qDebug()<<"sss";

    Widget w;
    w.show();

    int stin=dup(STDIN_FILENO);
    int stou=dup(STDOUT_FILENO);
    int pipe_ret=pipe(fdpipe);

    //unlink("/home/lzx/mp_music/fifo");

    /*fd=open("/home/lzx/mp_music/fifo",O_RDWR);
    if(fd<0)
        qDebug()<<"open error";*/
    pid_t pid;
    pid=fork();
    if(pid<0)
        perror("aa");
    else if(pid==0)
    {
        signal(SIGHUP, handle_signal);
        prctl(PR_SET_PDEATHSIG, SIGHUP);
        close(fdpipe[0]);
        dup2(fdpipe[1],STDOUT_FILENO);
        //dup2(STDIN_FILENO,STDIN_FILENO);
        FILE *fp=fopen("/home/lzx/mp_music/a.txt","r");
        char tmp[256];
        info_link.size=0;
        fgets(tmp,sizeof(tmp),fp);
        /*usleep(500000);
        while(1)
        {
            qDebug()<<info_link.current->name<<"--"<<tmp;
            if(info_link.current->name.compare(tmp)==0)
            {
                break;
            }
            info_link.current=info_link.current->next;
            info_link.size++;
        }*/
        execlp("mplayer","mplayer","-slave","-quiet","-idle","-input","file=/home/lzx/mp_music/fifo",tmp,NULL);
        execlp("mplayer","mplayer","-slave","-quiet","-idle","-input","file=/home/lzx/mp_music/fifo","/home/lzx/song/小半.mp3",NULL);
    }
    else
    {
        catch_fileinfo();
        pthread_t pth1,pth2,pth3,pth4;
        int pth_ret=pthread_create(&pth1,NULL,pth_work,&w);
        if(pth_ret!=0)
        {
            qDebug()<<"pthread error";
        }
        pth_ret=pthread_create(&pth2,NULL,pth2_work,&w);
        if(pth_ret!=0)
        {
            qDebug()<<"pthread error";
        }
        pth_ret=pthread_create(&pth3,NULL,pth3_work,&w);
        if(pth_ret!=0)
        {
            qDebug()<<"pthread error";
        }
        pth_ret=pthread_create(&pth4,NULL,pth4_work,&w);
        if(pth_ret!=0)
        {
            qDebug()<<"pthread error";
        }

        w.set_mylist();
    }
    return a.exec();
    dup2(stin,STDIN_FILENO);
    dup2(stou,STDOUT_FILENO);
}


