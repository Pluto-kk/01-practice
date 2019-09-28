#include "wrap.h"
#include "pub.h"
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#define DATA_LENGTH 5000000
char *my_work_dir="/mnt/hgfs/share/";
char *html_way_buf="./index.html";			  //html文件路径
char *dir_head="<html><head><title>Index of ./</title><meta http-equiv=content-Type content=text/html; charset=utf8></head><body bgcolor=#99cc99><h4>Index of ./</h4><ul type=circle>";
char *dir_tail="</ul><address><a href=http://www.itcast.com/>xhttpd</a></address></body></html>";


int scan_dir(char *str_buf);
int file_write(char *str_buf,int sock_fd);
void dir_html_init(char *dir_buf,int sock_fd);
void read_work(char *buf,int fd);

/*	int scan_dir(char *str)
	char *str : 文件名  
  		  功能 : 判断文件目录下是否有str文件
  		返回值 : 有 返回1  没有 返回0
*/
int scan_dir(char *str_buf)
{
	char dir_buf[128]="";
	strcpy(dir_buf,str_buf);
	char  str[128]="";
	if(dir_buf[strlen(dir_buf)-1]=='/')
		dir_buf[strlen(dir_buf)-1]='\0';
	char *s1=&dir_buf[strlen(dir_buf)-1];
	while(*(s1-1)!='/')
	{
		s1--;
	}
	strcpy(str,s1);
	*s1='\0';
	printf("打开的目录:%s,打开的文件:%s\n",dir_buf,str);
	DIR *dir=opendir(dir_buf);
	if(dir==NULL)
	{
		perror("opendir");
		return 0;
	}
	struct dirent *dirp=NULL;
	while(dirp=readdir(dir))
	{
		//printf("目录下的文件name:%s\n",dirp->d_name);
		if(strcmp(str,dirp->d_name)==0)
		{
			closedir(dir);
			return 1;
		}
	}
	closedir(dir);
	return 0;
}

/*	void file_write(char *str,int sock_fd)
	char *str : 文件名  
  int sock_fd : 请求套接字
  		  功能 : 判断文件名后缀，组包，发送文件数据
*/
int file_write(char *str_buf,int sock_fd)
{
	char str[512]="";
	strcpy(str,str_buf);
	int textlen=0;                  		//读取文件长度
	char buf[1500]="";              		//数据包
	char *contenttype_buf=NULL;				//数据类型字符串
	char text_buf[DATA_LENGTH];				//文件数据存放数组
	char file_str[128]="";					//文件路径

	if(str_buf[strlen(str_buf)-1]=='/')
		str_buf[strlen(str_buf)-1]='\0';
	strcpy(file_str,str);
	memset(str,0,sizeof(str));
	char *s1=&file_str[strlen(file_str)-1];
	while(*(s1-1)!='/')
	{
		s1--;
	}
	strcpy(str,s1);
	*(s1-1)='\0';

	printf("-------------str_buf:%s\n",str_buf);
	struct stat file_stat;
	int ret=stat(str_buf,&file_stat);                //获取文件信息
	if(ret==-1)
	{
		perror("stat");
		return -1;
	}

	if((file_stat.st_mode&S_IFMT)==S_IFDIR)          //判断是否为目录
	{
		file_str[strlen(file_str)]='/';
		file_str[strlen(file_str)]='\0';
		printf("-------------file_str:%s\n",file_str);
		dir_html_init(file_str,sock_fd);
		return 1;
	}

	int fd=open(str_buf,O_RDONLY);                   //只读方式打开文件
	if(fd==-1)
	{
		perror("file open");
		return -1;
	}


	contenttype_buf=get_mime_type(str);         //获取文件类型字符串
	sprintf(buf,"HTTP/1.0 200 OK\r\nConnection: Keep-Alive\r\nContent-Length:%ld\r\n%s\r\n\r\n",file_stat.st_size,contenttype_buf);   //协议包组包   文件大小  文件类型  
	write(sock_fd,buf,strlen(buf));                   //写入协议包头部	
	while(1)                                          //写入文件数据
	{
		textlen=read(fd,text_buf,sizeof(text_buf));
		
		write(sock_fd,text_buf,textlen);
		if(textlen<DATA_LENGTH)
		{
			close(fd);
			break;
		}
		//printf("%s\n",text_buf);
	}
	close(fd);
	return 0;
}

/*	void dir_html_init(char *dir_buf,int sock_fd)
	char *dir_buf ：文件目录
	  int sock_fd ：请求套接字
	  		功  能 ：读取目录下的文件名，组包写入html文件中
*/
void dir_html_init(char *dir_buf,int sock_fd)
{
	char str[128]="";
	struct dirent *dirp=NULL;         			  //目录信息结构体
	char buf[1024]="";				  			  //html数据包
	
	DIR *dir=opendir(dir_buf);
	if(dir==NULL)
	{
		perror("opendir");
		return;
	}
	sscanf(dir_buf,"./%s",str);
	int file_fd=open(html_way_buf,O_WRONLY|O_CREAT|O_TRUNC,0777);   //打开html文件
	if(file_fd<0)
	{
		perror("");
	}

	write(file_fd,dir_head,strlen(dir_head));					//写入html头部
	char *s=str;
	while(*s=='/')
	{
		s++;
	}
	while(dirp=readdir(dir))
	{
		if(strcmp(dirp->d_name,".")==0)
			continue;
		memset(buf,0,sizeof(buf));
		if(dirp->d_type==DT_REG) 			//普通文件
		{
			sprintf(buf,"<li><a href=%chttp://10.20.155.205:8090/%s/%s%c>%s</a>\r\n</li>",'"',s,dirp->d_name,'"',dirp->d_name);
		}
		else if(dirp->d_type==DT_DIR)    	//目录文件
		{
			sprintf(buf,"<li><a href=%chttp://10.20.155.205:8090/%s/%s%c>%s/</a>\r\n</li>",'"',s,dirp->d_name,'"',dirp->d_name);
		}
		else								//其他文件
		{
			printf("这是一个其他文件\n");
			continue;
		}
		write(file_fd,buf,strlen(buf));
	}
	write(file_fd,dir_tail,strlen(dir_tail));					//写入html尾部
	closedir(dir);
	close(file_fd);
}

/*    void read_work(char *buf,int fd)
	char *buf : 读取到的帧数据
	  int fd  : 请求套接字
 	     功能  : 解析数据，是为GET请求，检测有无请求资源，有，返回数据，没有，返回404
*/
void read_work(char *buf,int fd)
{
	printf("%s\n",buf);
	char type_str[128]="";                       //请求类型
	char index_ht[512]="";						 //请求标题（原始）
	char index_html[512]="";					 //请求标题（转换后）
	sscanf(buf,"%s %s ",type_str,index_html);        		 //解析数据

	strdecode(index_ht,index_html);                 		 //url转unicode

	sprintf(index_html,".%s",index_ht);
	printf("index_html:[%s]--%ld\n",index_html,strlen(index_html));
	while(index_html[strlen(index_html)-1]=='/')             //如果是目录，去掉最后一个/
	{
		index_html[strlen(index_html)-1]='\0';
	}
	if(strcasecmp(type_str,"get")==0)                		 //是否为GET请求
	{
		if(strcmp(index_ht,"HTTP/1.1")==0 || strcmp(index_ht,"/")==0)	 //无请求标题，显示目录界面
		{
			printf("无请求标题，显示目录界面\n");
			dir_html_init("./",fd);
			file_write("./index.html",fd);
		}
		else if(scan_dir(index_html)==1)                     //文件目录下是否存在该文件
		{	
			if(file_write(index_html,fd)==1)				 //如果文件为目录，重写index.html文件
			{
				file_write("./index.html",fd);
			}
		}	
		else 	      										 //没有请求的文件，返回404
		{
			file_write("./404.jpg",fd);
		}
	}
}

int main(int argc, char const *argv[])
{
	chdir(my_work_dir);
	int lfd=tcp4bind(8090,NULL);

	Listen(lfd,128);

	int epfd=epoll_create(1);

	struct epoll_event ev,evs[1024];
	ev.events=EPOLLIN;
	ev.data.fd=lfd;

	epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&ev);

	sigset_t mysig;
	sigemptyset(&mysig);
	sigaddset(&mysig,SIGPIPE);
	sigprocmask(SIG_BLOCK,&mysig,NULL);

	while(1)
	{
		int num=epoll_wait(epfd,evs,1024,-1);
		if(num<0)
		{
			perror("epoll_wait");
		}
		else if(num>=0)
		{
			for(int i=0;i<num;i++)
			{
				int fd=evs[i].data.fd;
				if(fd==lfd&&evs[i].events&EPOLLIN)
				{
					struct sockaddr_in client;
					int len=sizeof(client);
					int cfd=accept(lfd,(struct sockaddr *)&client,&len);

					printf("%s  %d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
					ev.data.fd=cfd;
					epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ev);
				}
				else if(evs[i].events&EPOLLIN)
				{
					char buf[1500]="";
					while(1)
					{
						int num=read(fd,buf,sizeof(buf));
						if(num<0)
						{
							if(errno==EAGAIN)
							{
								break;
							}
							else
							{
								perror("read");
								close(fd);
								epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&evs[i]);
								break;
							}
						}
						else if(num==0)
						{
							printf("client close\n");
							close(fd);
							epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&evs[i]);
							break;
						}
						else
						{
							read_work(buf,fd);
							memset(buf,0,sizeof(buf));
							break;
						}
					}
				}
		}
	}
}
	return 0;
}