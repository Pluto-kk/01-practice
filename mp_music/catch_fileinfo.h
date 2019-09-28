#ifndef CATCH_FILEINFO1_H
#define CATCH_FILEINFO1_H
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <QString>
#include <QDebug>
#include <string.h>
typedef struct fileinfo
{
    struct fileinfo *prev;
    QString  name;
    struct fileinfo *next;
}INFO;

typedef struct fileinfolink
{
    struct fileinfo *head;
    struct fileinfo *current;
    int size;
    int capacity;
    struct fileinfo *tail;
}LINK;
int catch_fileinfo1(QString);
int catch_fileinfo();
void clear_repeat();
void mydir(struct dirent *);
struct fileinfo;
struct fileinfolink;
#endif // CATCH_FILEINFO_H
