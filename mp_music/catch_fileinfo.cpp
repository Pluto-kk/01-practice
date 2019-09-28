#include "catch_fileinfo.h"
#include <QtCore/QString>
#include <qstring.h>
#include <QString>
#include <dirent.h>
#include "gb2312_ucs2.h"
struct fileinfolink info_link;
QString buf;
QString mp3=".mp3";

void mydir(struct dirent *dir,QString file)//筛选mp3文件并存入链表
{
    buf=dir->d_name;
    qDebug()<<buf<<buf.mid(buf.length()-4,buf.length());
    if(mp3.compare(buf.mid(buf.length()-4,buf.length()))==0)
    {
        if(info_link.head==NULL)
        {
            struct fileinfo *tmp=new struct fileinfo;
            tmp->name=file;
            tmp->name.append("/").append(QString::fromLocal8Bit(dir->d_name));
            tmp->prev=NULL;
            tmp->next=NULL;
            info_link.head=tmp;
            info_link.current=tmp;
            info_link.tail=tmp;
            info_link.size=1;
            info_link.capacity=1;
        }
        else
        {
            struct fileinfo *tmp=new struct fileinfo;
            tmp->name=file;
            tmp->name.append("/").append(QString::fromLocal8Bit(dir->d_name));
            tmp->prev=info_link.tail;
            tmp->next=NULL;
            info_link.tail->next=tmp;
            info_link.tail=tmp;
            info_link.capacity++;
        }
    }
}

int catch_fileinfo()//获取歌曲文件名
{
    char *buf;
    info_link.head=NULL;
    info_link.tail=NULL;
    DIR *namelist=NULL;
    namelist=opendir("/home/lzx/song");
    if(namelist==NULL)
        return 0;
    struct dirent *dirt;
    int i=0;
    while((dirt=readdir(namelist))!=NULL)
      {
        mydir(dirt,"/home/lzx/song");
          i++;
      }
    qDebug()<<info_link.capacity<<info_link.size;
    return 0;
}
int catch_fileinfo1(QString str)//有参获取歌曲文件名
{
    char *buf;
    buf=str.toLatin1().data();
    qDebug()<<buf<<"aa";
    /*int n=scandir("/home/lzx/mp_music",&namelist,NULL,alphasort);
    if(n<0)
       perror("scandir");*/
    DIR *namelist=NULL;
    namelist=opendir(buf);
    if(namelist==NULL)
        return 0;
    struct dirent *dirt;
    int i=0;
    while((dirt=readdir(namelist))!=NULL)
      {

          mydir(dirt,str);
          i++;
      }
    qDebug()<<info_link.capacity<<info_link.size;
    return 0;
}

void clear_repeat()//释放链表
{
    for(INFO *p=info_link.head;p->next!=NULL;p=p->next)
        for(INFO *np=p->next;np->next!=NULL;np=np->next)
        {
            if(p->name.compare(np->name)==0)
            {
                np->prev->next=np->next;
                np->next->prev=np->prev;
                info_link.capacity--;
                free(np);
            }
        }
}













