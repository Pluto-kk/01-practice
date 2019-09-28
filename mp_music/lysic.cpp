#include "lysic.h"

char line_data[256]="";                //储存读取的行数据
KK *p=NULL;
KK *myhead=NULL;
/*******************************

功能:创建一个新节点，插入尾端

返回值:无

*******************************/
void KK_CREATE(KK **p)
{
    if(*p==NULL)
    {
        KK *myhead=static_cast<KK *>(malloc(sizeof(KK)));
        myhead->t=static_cast<TIME *>(malloc(sizeof(TIME)));
        myhead->next=NULL;
        myhead->prev=NULL;
        *p=myhead;
    }
    else
    {
        KK *myhead=*p;
        KK *current=NULL;
        while(myhead->next!=NULL)
            myhead=myhead->next;
        current=static_cast<KK *>(malloc(sizeof(KK)));
        current->t=static_cast<TIME *>(malloc(sizeof(TIME)));
        current->next=NULL;
        current->prev=myhead;
        myhead->next=current;
    }
}
/*******************************

功能:传入读取到的字符串，将时间和歌词分割存入到链表中

返回值:无

*******************************/
void SRC_HANDLE(char *s,KK *myhead)
{
    int n=1,i=0;
    char str[128];
    while(*s=='[')
    {
        if(n==1)
        {
            sscanf(s+1,"%[^]]",str);
            sscanf(str,"%2d:%2d.%2d",&(myhead->t->h),&(myhead->t->m),&(myhead->t->s));
            n=0;
        }
        while(*s!=']')
            s++;
        s++;
    }
    while(*s!='\n')
    {
        myhead->src[i++]=*s;
        s++;
    }
//sscanf(s,"%[^ ]",myhead->src);
}
/*******************************

功能:链表排序

返回值:无

*******************************/
void SRC_SORT(KK **p)
{
        KK	*myhead=*p,*current=NULL;
        for(myhead;myhead->next!=NULL;myhead=myhead->next)
        {
            for(current=myhead->next;current!=NULL;current=current->next)
            {
                if((myhead->t->h*60+myhead->t->m)>(current->t->h*60+current->t->m))
                {
                        KK tmp;
                        tmp=*myhead;
                        *myhead=*current;
                        *current=tmp;

                        tmp.prev=myhead->prev;
                        tmp.next=myhead->next;
                        myhead->prev=current->prev;
                        myhead->next=current->next;
                        current->prev=tmp.prev;
                        current->next=tmp.next;
                }
            }
        }
}
/*******************************

功能:判断文件是否到结尾处，从文件中读取一行数据并调用SRC_HANDLE(char*,KK *myhead);

返回值:无

*******************************/
void GET_SRC(FILE *fp,KK *p)
{
    KK *myhead=NULL;
    while(feof(fp)==0)
    {
        KK_CREATE(&p);
        myhead=p;
        while(myhead->next!=NULL)
        myhead=myhead->next;
        fgets(line_data,sizeof(line_data),fp);
        //puts(buf);
        SRC_HANDLE(line_data,myhead);
        if(line_data[10]=='[')
        {
        KK_CREATE(&p);
        myhead=p;
        while(myhead->next!=NULL)
        myhead=myhead->next;
        SRC_HANDLE(line_data+10,myhead);
        }
        if(line_data[20]=='[')
        {
        KK_CREATE(&p);
        myhead=p;
        while(myhead->next!=NULL)
        myhead=myhead->next;
        SRC_HANDLE(line_data+20,myhead);
        }
    }
}
/*******************************

功能:显示节点前五个节点和后五个节点的数据

返回值:无

*******************************/
void DIS_SRC(TIME *h,KK *p,KK *myhead)
{
        TIME clock=*h;
        printf("\t\t\t\t\t\t\t\t\t\t歌名:%s\n",p->src);
        printf("\t\t\t\t\t\t\t\t\t\t歌手:%s\n",p->next->src);
        printf("\t\t\t\t\t\t\t\t\t\t专辑:%s\n",p->next->next->src);
        printf("\t\t\t\t\t\t\t\t\t\t制作:%s\n",p->next->next->next->src);
        printf("\t\t\t\t\t\t\t\t\t\t    %02d:%02d\n",clock.h,clock.m);//cusor_get_pos(); cusor_show();
        if(myhead->prev!=NULL)
        {
        if(myhead->prev->prev!=NULL)
        {
        if(myhead->prev->prev->prev!=NULL)
        {
        if(myhead->prev->prev->prev->prev!=NULL)
        {
        if(myhead->prev->prev->prev->prev->prev!=NULL)
        {
        if(myhead->prev->prev->prev->prev->prev->prev!=NULL)
        {
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->prev->prev->prev->prev->src);
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->prev->prev->prev->src);
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->prev->prev->src);
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->prev->src);
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->src);
        }
        }
        else
        {
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->prev->prev->prev->src);
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->prev->prev->src);
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->prev->src);
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->src);
        }
        }
        else
        {
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->prev->prev->src);
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->prev->src);
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->src);
        }
        }
        else
        {
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->prev->src);
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->src);
        }
        }
        else
        {
            printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->prev->src);
        }
        }

        printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->src);
        if(myhead->next!=NULL)
        {
        printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->next->src);
        if(myhead->next->next!=NULL)
        {
        printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->next->next->src);
        if(myhead->next->next->next!=NULL)
        {
        printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->next->next->next->src);
        if(myhead->next->next->next->next!=NULL)
        {
        printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->next->next->next->next->src);
        if(myhead->next->next->next->next->next!=NULL)
        {
        printf("\t\t\t\t\t\t\t\t\t\t%s\n",myhead->next->next->next->next->next->src);
        }}}}}
}
/*******************************

功能:显示歌词，调用DIS_SRC(TIME *h,KK *p,KK *myhead)

返回值:无

*******************************/
void DISPLAY(TIME *h,KK *p)
{
        KK *myhead=p;
        TIME clock=*h;

        if(clock.h==0&&clock.m==0)
        {

            printf("\t\t\t\t\t\t\t\t\t\t歌名:%s\n",p->src);
            printf("\t\t\t\t\t\t\t\t\t\t歌手:%s\n",p->next->src);
            printf("\t\t\t\t\t\t\t\t\t\t专辑:%s\n",p->next->next->src);
            printf("\t\t\t\t\t\t\t\t\t\t制作:%s\n",p->next->next->next->src);
        }

            printf("\t\t\t\t\t\t\t\t\t\t    %02d:%02d\n",clock.h,clock.m);
        while(myhead!=NULL)
        {
            if((myhead->t->h==clock.h)&&(myhead->t->m==clock.m))
            {
                DIS_SRC(&clock,p,myhead);
            }
        myhead=myhead->next;

        }
}
/*******************************

功能:读取最后一句歌词的时间点

返回值:无

*******************************/
void END_OF_SRC(KK *p)
{
    /*while(p!=NULL)
    {
    if((((p->t->h*60)+p->t->m)*100+p->t->s)>(((x*60)+y)*100+z))
    {
        x=p->t->h;
        y=p->t->m;
        z=p->t->s;
    }
        p=p->next;
    }*/
}

int lysic_catch()
{
    FILE *fp=fopen("/home/lzx/mp_music/a.lrc","r");
    if(fp==NULL)
    {
        perror("fopen");
    }
    for(int i=0;i<4;i++)          //读取前四句歌词
    {
        KK_CREATE(&p);
        myhead=p;
        while(myhead->next!=NULL)
            myhead=myhead->next;
        fgets(line_data,sizeof(line_data),fp);
        sscanf(line_data+4,"%[^]]",myhead->src);
    }
    GET_SRC(fp,p);                 //获取并分割歌词
    fclose(fp);
    myhead=p;
    SRC_SORT(&p);                  //链表排序
    END_OF_SRC(p);
    return 0;
}

