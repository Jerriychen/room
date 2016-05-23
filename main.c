

//
//  main.c
//  room
//
//  Created by   chenzzh on 16/5/17.
//  Copyright © 2016年   chenzzh. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<windows.h>
#define LEN1 sizeof(struct room)
#define LEN2 sizeof(struct bed)

struct room{
    int sex;
    int left_bed;                  //每个房间剩的床数
    int sumbed;                   //每个房间总的床数
    int tenant;                  //房间已住房客的数量
    int roomlevel;
    int roomnumber;
    struct bed *bedpoint;
    struct room *right;
    struct room *down;
};
struct bed{
    int sex;                  //1-男性  0-女性  －1-没人
    int years;
    int year;
    int month;
    int day;
    int bednumber;            //床位号
    char name[10];
    struct bed *next;
};

struct room *Creat();
struct room *CheckIn(struct room *head,char name[10],int sex,int roomlevel,int years,int year,int month,int day);
struct room *CheckOut(struct room *head,char name[10],int roomlevel,int roomnumber);
struct room *Inqureroom(struct room *head,int roomlevel);
void show(struct room *,struct bed *);
void showmenu();


int main()
{
    int n,sex,years,year,month,day,roomnumber,roomlevel;
    char name[10];
    struct room *head=Creat();
    system("cls");
    while(1)
    {
        showmenu();
        printf("\nplease input your choice:");
        scanf("%d",&n);
        switch(n)
        {
            case 1:{
                printf("please input name:");
                scanf("%s",name);
                printf("please input sex,roomlevel,years,and date:");
                scanf("%d,%d,%d,%d,%d,%d",&sex,&roomlevel,&years,&year,&month,&day);
                head=CheckIn(head,name,sex,roomlevel,years,year,month,day);
                break;
            }
            case 2:{
                printf("please input name:");
                scanf("%s",name);
                printf("please input roomlevel and roomnumber:");
                scanf("%d,%d",&roomlevel,&roomnumber);
                head=CheckOut(head,name,roomlevel,roomnumber);
                break;
            }
            case 3:{
                printf("please input roomlevel:");
                scanf("%d",&roomlevel);
                head=Inqureroom(head,roomlevel);
                break;
            }
            case 4:{
                system("cls");
                break;
            }
            default:break;
        }
    }
    return 0;
}

void showmenu()
{
    printf("---Welcome to HanTing Hotel---\n");
    printf("-------1-Checkin--------------\n");
    printf("-------2-Checkout-------------\n");
    printf("-------3-Inqure---------------\n");
    printf("-------4-Clear----------------\n");
    printf("------------------------------\n");
}

struct room *Creat()
{
    int n,m,bed,i,j;
    printf("please input the max roomlevel:");
    scanf("%d",&n);
    struct room *head=NULL;
    struct room *p1,*p2,*p3,*p4;
    p1=p2=(struct room *)malloc(LEN1);
    p3=(struct room *)malloc(LEN1);
    for(i=1;i<=n;i++)
    {
        printf("please input rank %d the number of bed each room and the number of each roomlevel room:",i);
        scanf("%d,%d",&m,&bed);
        p1->sex=-1;
        p1->left_bed=bed;
        p1->sumbed=bed;
        p1->roomlevel=i;
        p1->tenant=0;
        p1->roomnumber=i*100+1;
        p1->bedpoint=NULL;
        if(i==1)head=p1;
        else p2->right=p1;
        p2=p1;
        p1=p4=(struct room *)malloc(LEN1);
        for(j=1;j<m;j++)
        {
            p3->sex=-1;
            p3->left_bed=bed;
            p3->sumbed=bed;
            p3->roomlevel=i;
            p3->tenant=0;
            p3->roomnumber=i*100+j+1;
            p3->bedpoint=NULL;
            if(j==1)p2->down=p3;
            else p4->down=p3;
            p4=p3;
            p3=(struct room *)malloc(LEN1);
        }
        if(m==1)p2->down=NULL;
        else p4->down=NULL;
    }
    p2->right=NULL;
    return head;
};


struct room *CheckIn(struct room *head,char name[10],int sex,int roomlevel,int years,int year,int month,int day)
{
    struct room *p=head;
    struct bed *q1,*q2,*q3;
    q1=(struct bed *)malloc(LEN2);q2=q3=NULL;
    strcpy(q1->name,name);
    q1->sex=sex;
    q1->years=years;
    q1->year=year;
    q1->month=month;
    q1->day=day;
    q1->bednumber=0;
    q1->next=NULL;

    while(p->roomlevel!=roomlevel&&p->right!=NULL)
        p=p->right;
    if(p==NULL){printf("The roomlevel you input error!\n");return head;}

    while(p!=NULL)
    {
        if((p->sex==sex||p->sex==-1)&&p->left_bed!=0)
            break;
        else
            p=p->down;
    }
    if(p==NULL){printf("The room of this roomlevel is over!\n");return head;}

    if(p->bedpoint==NULL)
    {q1->bednumber=1;p->bedpoint=q1;}
    else
    {
        q2=p->bedpoint;
        if(q2->bednumber!=1){q1->bednumber=1;q1->next=q2;p->bedpoint=q1;}
        else
            if(q2->next==NULL)
        {q1->bednumber=2;q2->next=q1;}
        else
        {
            q3=q2->next;
            while(q3->bednumber-q2->bednumber==1&&q3->next!=NULL)
            {q2=q3;q3=q3->next;}

            if(q3->bednumber-q2->bednumber==1&&q3->next==NULL)
            {q1->bednumber=q3->bednumber+1;q3->next=q1;}

            if(q3->bednumber-q2->bednumber!=1)
            {q1->bednumber=q2->bednumber+1;q2->next=q1;q1->next=q3;}
        }
    }
    p->left_bed--;
    p->sex=sex;
    p->tenant++;
    show(p,q1);
    return head;
}


struct room *CheckOut(struct room *head,char name[10],int roomlevel,int roomnumber)
{
    struct room *p;
    struct bed *q1,*q2;
    p=head;q1=q2=NULL;
    while(p->roomlevel!=roomlevel&&p->right!=NULL)
        p=p->right;
    if(p==NULL){printf("The roomlevel you input error!\n");return head;}

    while(p->roomnumber!=roomnumber&&p->down!=NULL)
        p=p->down;
    if(p==NULL){printf("The roomnumber you input error!\n");return head;}

    q1=p->bedpoint;
    while(strcmp(q1->name,name)!=0&&q1->next!=NULL)
    {
        q2=q1;q1=q1->next;
    }

    if(strcmp(q1->name,name)==0)
    {
        if(q1==p->bedpoint)
            p->bedpoint=p->bedpoint->next;
        else
            q2->next=q1->next;
        printf("Checkout succeed,%s,welcome to come back again!\n",q1->name);
        p->left_bed++;
        if(p->tenant==1)
            {p->tenant--;p->sex=-1;}
        else
            p->tenant--;
    }
    else
        printf("can't find this person.\n");
    return head;
}


struct room *Inqureroom(struct room *head,int roomlevel)
{
    struct room *p;
    p=head;
    while(p->roomlevel!=roomlevel&&p->right!=NULL)
        p=p->right;
    if(p==NULL){printf("The roomlevel you input error!\n");return head;}

    for(;p!=NULL;p=p->down)
    {
        printf("roomnumber:%d",p->roomnumber);
        if(p->sex==0) printf(" It's a female room,");
        if(p->sex==1) printf(" It's a male room,");
        if(p->sex==-1) printf(" It's a  empty room,");
        printf("  there are %d tenant ",p->tenant);
        printf("and there are %d beds remain.\n\n",p->left_bed);
    }
    return head;
}

void show(struct room *p,struct bed *q)
{
    if(p->sex==1)
        printf("Welcome,Mr %s.\nage:%d",q->name,q->years);
    if(p->sex==0)
        printf("Welcome,Ms %s.\nage:%d",q->name,q->years);
    printf(" roomlevel:%d",p->roomlevel);
    printf(" roomnumber:%d",p->roomnumber);
    printf(" bednumber:%d",q->bednumber);
    printf(" the date of Checkin:%d-%d-%d\n\n",q->year,q->month,q->day);
}
