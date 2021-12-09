#include<unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/un.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<fcntl.h>
#include "admin.h"
#include "joint.h"
#include "single.h"
#include "summary.h"
void adminwindow(int fd)
{ char buff[23000];
    struct admin adm;char account[4096],password[4096],path[5000];
    strcpy(path,"./admin/");
    write(fd,"Enter your unqiue admin account number\n",strlen("Enter your unqiue admin account number\n"));
    int readbytes=read(fd,account,4096);account[readbytes]='\0';
    strcat(path,account);
    int record=open(path,O_RDWR);
    if(record==-1)
    {
        write(fd,"Invalid admin account number.Press -1 to exit.\n",strlen("Invalid admin account number.Press -1 to exit.\n"));
        return;
    }
    struct flock lock;
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;
    fcntl(record,F_GETLK,&lock);
    if(lock.l_type!=F_UNLCK)
    {
     write(fd,"Server busy.Try after sometime.Press -1 to exit.\n",strlen("Server busy.Try after sometime.Press -1 to exit.\n"));
    }  
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;
    fcntl(record,F_SETLK,&lock);
    int std=dup(0);
    dup2(record,0);
    scanf("%[^\n]s",adm.name);
    getchar();
    scanf("%[^\n]s",adm.password);
    getchar();
    sprintf(buff,"Hello %s, please enter your password \n",adm.name);
    write(fd,buff,strlen(buff));
    readbytes=read(fd,password,4096);password[readbytes]='\0';
    if(strcmp(adm.password,password)!=0)
    {write(fd,"Wrong Password.Press -1 to exit\n",strlen("Wrong Password.Press -1 to exit\n"));return;}
    dup2(std,0);
    strcpy(path,"./summary");
    int summ=open(path,O_RDWR);
    struct flock lock1;
    lock1.l_type=F_WRLCK;
    lock1.l_whence=SEEK_SET;
    lock1.l_start=0;
    lock1.l_len=0;
    fcntl(summ,F_SETLKW,&lock1);
    struct summary number;
    std=dup(0);
    dup2(summ,0);
    char field[10];
    getchar();
    scanf("%[^\n]s",field);
    getchar();
    sscanf(field,"%ld",&number.single);
    scanf("%[^\n]s",field);
    getchar();
    sscanf(field,"%ld",&number.joint);
    scanf("%[^\n]s",field);
    getchar();
    sscanf(field,"%ld",&number.admin);
    dup2(std,0);
    close(std);
    sprintf(buff,"Welcome %s\n",adm.name);
    write(fd,buff,strlen(buff));
    write(fd,"1. Add\n2. Delete\n3. Modify\n4. Search\n5. Exit\n",strlen("1. Add\n2. Delete\n3. Modify\n4. Search\n5. Exit\n"));
    char option;char confirm[4096];
    read(fd,&option,1);long int amount;
    switch(option)
    {
        case '1':
            write(fd,"Enter the account you want to add \n1.Admin\n2.Single User\n3.Joint User\n",strlen("Enter the account you want to add \n1.Admin\n2.Single User\n3.Joint User\n"));
            char o1;
            readbytes=read(fd,&o1,1);
            struct single user; struct joint user1; struct admin user2;
            switch(o1)
            {
                case '1':
                    write(fd,"Enter the name of admin\n",strlen("Enter the name of admin\n"));
                    readbytes=read(fd,user2.name,4096);user2.name[readbytes]='\0';
                    write(fd,"Enter the password of user\n",strlen("Enter the password of user\n"));
                    readbytes=read(fd,user2.password,4096);user2.password[readbytes]='\0';
                    write(fd,"Confirm password.\n",strlen("Confirm password.\n"));                    
                    readbytes=read(fd,confirm,4096);confirm[readbytes]='\0';
                    if(strcmp(user2.password,confirm)!=0)
                    {
                            write(fd,"Password dont match.Press -1 to exit.\n",strlen("Password dont match.Press -1 to exit.\n"));
                            return;
                    }
                    strcpy(path,"./admin/");
                    number.admin++;char anumber1[12];
                    sprintf(anumber1,"%ld",number.admin);
                    strcat(path,anumber1);
                    int file=open(path,O_RDWR|O_CREAT,0666);
                    sprintf(buff,"%s\n%s\n",user2.name,user2.password);
                    write(file,buff,strlen(buff));
                    close(file);
                    lseek(summ,0,SEEK_SET);
                    sprintf(buff,"%ld\n%ld\n%ld\n",number.single,number.joint,number.admin);
                    write(summ,buff,strlen(buff));
                    sprintf(buff,"Admin created successfully with account number %ld!Press -1 to exit\n",number.admin);
                    write(fd,buff,strlen(buff));
                    break;
                case '2':
                    write(fd,"Enter the name of user\n",strlen("Enter the name of user\n"));
                    readbytes=read(fd,user.name,4096);user.name[readbytes]='\0';
                    write(fd,"Enter the password of user\n",strlen("Enter the password of user\n"));
                    readbytes=read(fd,user.password,4096);user.password[readbytes]='\0';
                    write(fd,"Confirm password.\n",strlen("Confirm password.\n"));                    
                    readbytes=read(fd,confirm,4096);confirm[readbytes]='\0';
                    if(strcmp(user.password,confirm)!=0)
                    {
                            write(fd,"Password dont match.Press -1 to exit.\n",strlen("Password dont match.Press -1 to exit.\n"));
                            return;
                    }
                    write(fd,"Enter the balance of user\n",strlen("Enter the balance of user\n"));
                    char balance[11];
                    readbytes=read(fd,balance,4096);balance[readbytes]='\0';
                    sscanf(balance,"%ld",&user.balance);
                    strcpy(path,"./user/single/");
                    number.single++;char anumber[12];
                    sprintf(anumber,"%ld",number.single);
                    strcat(path,anumber);
                    int file1=open(path,O_RDWR|O_CREAT,0666);
                    sprintf(buff,"%s\n%s\n%ld\n",user.name,user.password,user.balance);
                    write(file1,buff,strlen(buff));
                    close(file1);
                    lseek(summ,0,SEEK_SET);
                    sprintf(buff,"%ld\n%ld\n%ld\n",number.single,number.joint,number.admin);
                    write(summ,buff,strlen(buff));
                    sprintf(buff,"Account created successfully with account number %ld!Press -1 to exit\n",number.single);
                    write(fd,buff,strlen(buff));
                    break;
                case '3':
                    write(fd,"Enter the name of first user\n",strlen("Enter the name of first user\n"));
                    readbytes=read(fd,user1.name1,4096);user1.name1[readbytes]='\0';
                    write(fd,"Enter the name of second user\n",strlen("Enter the name of second user\n"));
                    readbytes=read(fd,user1.name2,4096);user1.name2[readbytes]='\0';
                    write(fd,"Enter the password of user\n",strlen("Enter the password of user\n"));
                    readbytes=read(fd,user1.password,4096);user1.password[readbytes]='\0';
                    write(fd,"Confirm password.\n",strlen("Confirm password.\n"));                    
                    readbytes=read(fd,confirm,4096);confirm[readbytes]='\0';
                    if(strcmp(user1.password,confirm)!=0)
                    {
                            write(fd,"Password dont match.Press -1 to exit.\n",strlen("Password dont match.Press -1 to exit.\n"));
                            return;
                    }
                    write(fd,"Enter the balance of user\n",strlen("Enter the balance of user\n"));
                    char balance1[11];
                    readbytes=read(fd,balance,4096);balance[readbytes]='\0';
                    sscanf(balance,"%ld",&user1.balance);
                    strcpy(path,"./user/joint/");
                    number.joint++;char anumber2[12];
                    sprintf(anumber2,"%ld",number.joint);
                    strcat(path,anumber2);
                    int file2=open(path,O_RDWR|O_CREAT,0666);
                    sprintf(buff,"%s\n%s\n%s\n%ld\n",user1.name1,user1.name2,user1.password,user1.balance);
                    write(file2,buff,strlen(buff));
                    close(file2);
                    lseek(summ,0,SEEK_SET);
                    sprintf(buff,"%ld\n%ld\n%ld\n",number.single,number.joint,number.admin);
                    write(summ,buff,strlen(buff));
                    sprintf(buff,"Account created successfully with account number %ld!Press -1 to exit\n",number.joint);
                    write(fd,buff,strlen(buff));
                    break;
                default:
                    write(fd,"Invalid Choice. Press -1 to exit\n",strlen("Invalid Choice. Press -1 to exit\n"));
            }
            lock1.l_type=F_UNLCK;
            fcntl(summ,F_SETLKW,&lock1);
            close(summ);
            break;
        case '2':
            write(fd,"Enter the account you want to delete \n1.Admin\n2.Single User\n3.Joint User\n",strlen("Enter the account you want to delete \n1.Admin\n2.Single User\n3.Joint User\n"));
            char o2;
            readbytes=read(fd,&o2,1);
            switch(o2)
            {
                case '1':
                    strcpy(path,"./admin/");
                    write(fd,"Enter unqiue admin account number\n",strlen("Enter unqiue admin account number\n"));
                    readbytes=read(fd,account,4096);account[readbytes]='\0';
                    strcat(path,account);
                    remove(path);
                    write(fd,"Account successfully deleted.Press -1 to exit.\n",strlen("Account successfully deleted.Press -1 to exit.\n"));
                    break;
                case '2':
                    strcpy(path,"./user/single/");
                    write(fd,"Enter unqiue single account number\n",strlen("Enter unqiue single account number\n"));
                    readbytes=read(fd,account,4096);account[readbytes]='\0';
                    strcat(path,account);
                    remove(path);
                    write(fd,"Account successfully deleted.Press -1 to exit\n",strlen("Account successfully deleted.Press -1 to exit\n"));
                    break;
                case '3':
                    strcpy(path,"./user/joint/");
                    write(fd,"Enter unqiue joint account number\n",strlen("Enter unqiue joint account number\n"));
                    readbytes=read(fd,account,4096);account[readbytes]='\0';
                    strcat(path,account);
                    remove(path);
                    write(fd,"Account successfully deleted.Press -1 to exit\n",strlen("Account successfully deleted.Press -1 to exit\n"));
                    break;
                default:
                    write(fd,"Invalid choice.Press -1 to exit\n",strlen("Invalid choice.Press -1 to exit\n"));
            }
            break;
        case '3':
            write(fd,"Enter the account you want to modify \n1.Admin\n2.Single User\n3.Joint User\n",strlen("Enter the account you want to modify \n1.Admin\n2.Single User\n3.Joint User\n"));
            char o4;
            readbytes=read(fd,&o4,1);
            struct flock loc2;struct admin det1; struct single det2;struct joint det3;
            switch(o4)
            {
                case '1':
                    write(fd,"Enter the unqiue account number of admin\n",strlen("Enter the unqiue account number of admin\n"));
                    readbytes=read(fd,account,4096);account[readbytes]='\0';
                    strcpy(path,"./admin/");
                    strcat(path,account);
                    int admm=open(path,O_RDWR);
                    if(admm==-1){write(fd,"Invalid account number.Press -1 to exit\n",strlen("Invalid account number.Press -1 to exit\n"));return;}
                    write(fd,"Enter the name of admin\n",strlen("Enter the name of admin\n"));
                    readbytes=read(fd,det1.name,4096);det1.name[readbytes]='\0';
                    write(fd,"Enter the password of user\n",strlen("Enter the password of user\n"));
                    readbytes=read(fd,det1.password,4096);det1.password[readbytes]='\0';
                    write(fd,"Confirm password.\n",strlen("Confirm password.\n"));                    
                    readbytes=read(fd,confirm,4096);confirm[readbytes]='\0';
                    if(strcmp(det1.password,confirm)!=0)
                    {
                            write(fd,"Password dont match.Press -1 to exit.\n",strlen("Password dont match.Press -1 to exit.\n"));
                            return;
                    }
                    loc2.l_type=F_WRLCK;
                    loc2.l_whence=SEEK_SET;
                    loc2.l_start=0;
                    loc2.l_len=0;
                    fcntl(admm,F_SETLKW,&loc2);
                    lseek(admm,0,SEEK_SET);
                    sprintf(buff,"%s\n%s\n",det1.name,det1.password);
                    write(admm,buff,strlen(buff));
                    loc2.l_type=F_UNLCK;
                    fcntl(admm,F_SETLKW,&loc2);
                    close(admm);
                    write(fd,"Admin account successfully modified.Press -1 to exit\n",strlen("Admin account successfully modified.Press -1 to exit\n"));
                    break;
                case '2':
                    write(fd,"Enter the unqiue account number of user\n",strlen("Enter the unqiue account number of user\n"));
                    readbytes=read(fd,account,4096);account[readbytes]='\0';
                    strcpy(path,"./user/single/");
                    strcat(path,account);
                    int singl=open(path,O_RDWR);
                    if(singl==-1){write(fd,"Invalid account number.Press -1 to exit\n",strlen("Invalid account number.Press -1 to exit\n"));return;}
                    write(fd,"Enter the name of user\n",strlen("Enter the name of user\n"));
                    readbytes=read(fd,det2.name,4096);det2.name[readbytes]='\0';
                    write(fd,"Enter the password of user\n",strlen("Enter the password of user\n"));
                    readbytes=read(fd,det2.password,4096);det2.password[readbytes]='\0';
                    write(fd,"Confirm password.\n",strlen("Confirm password.\n"));                    
                    readbytes=read(fd,confirm,4096);confirm[readbytes]='\0';
                    if(strcmp(det2.password,confirm)!=0)
                    {
                            write(fd,"Password dont match.Press -1 to exit.\n",strlen("Password dont match.Press -1 to exit.\n"));
                            return;
                    }
                    write(fd,"Enter the balance of user\n",strlen("Enter the balance of user\n"));
                    char bal1[11];
                    readbytes=read(fd,bal1,4096);bal1[readbytes]='\0';
                    sscanf(bal1,"%ld",&det2.balance);
                    loc2.l_type=F_WRLCK;
                    loc2.l_whence=SEEK_SET;
                    loc2.l_start=0;
                    loc2.l_len=0;
                    fcntl(singl,F_SETLKW,&loc2);
                    lseek(singl,0,SEEK_SET);
                    sprintf(buff,"%s\n%s\n%ld",det2.name,det2.password,det2.balance);
                    write(singl,buff,strlen(buff));
                    loc2.l_type=F_UNLCK;
                    fcntl(singl,F_SETLKW,&loc2);
                    close(singl);
                    write(fd,"User account successfully modified.Press -1 to exit\n",strlen("User account successfully modified.Press -1 to exit\n"));
                    break;
                case '3':
                    write(fd,"Enter the unqiue account number of joint account\n",strlen("Enter the unqiue account number of joint account\n"));
                    readbytes=read(fd,account,4096);account[readbytes]='\0';
                    strcpy(path,"./user/joint/");
                    strcat(path,account);
                    int join=open(path,O_RDWR);
                    if(join==-1){write(fd,"Invalid account number.Press -1 to exit\n",strlen("Invalid account number.Press -1 to exit\n"));return;}
                    write(fd,"Enter the name of user 1\n",strlen("Enter the name of user 1\n"));
                    readbytes=read(fd,det3.name1,4096);det3.name1[readbytes]='\0';
                    write(fd,"Enter the name of user 2\n",strlen("Enter the name of user 2\n"));
                    readbytes=read(fd,det3.name2,4096);det3.name2[readbytes]='\0';
                    write(fd,"Enter the password of user\n",strlen("Enter the password of user\n"));
                    readbytes=read(fd,det3.password,4096);det3.password[readbytes]='\0';
                    write(fd,"Confirm password.\n",strlen("Confirm password.\n"));                    
                    readbytes=read(fd,confirm,4096);confirm[readbytes]='\0';
                    if(strcmp(det3.password,confirm)!=0)
                    {
                            write(fd,"Password dont match.Press -1 to exit.\n",strlen("Password dont match.Press -1 to exit.\n"));
                            return;
                    }
                    write(fd,"Enter the balance of user\n",strlen("Enter the balance of user\n"));
                    char bal2[11];
                    readbytes=read(fd,bal2,4096);bal2[readbytes]='\0';
                    sscanf(bal2,"%ld",&det3.balance);
                    loc2.l_type=F_WRLCK;
                    loc2.l_whence=SEEK_SET;
                    loc2.l_start=0;
                    loc2.l_len=0;
                    fcntl(join,F_SETLKW,&loc2);
                    lseek(join,0,SEEK_SET);
                    sprintf(buff,"%s\n%s\n%s\n%ld",det3.name1,det3.name2,det3.password,det3.balance);
                    write(join,buff,strlen(buff));
                    loc2.l_type=F_UNLCK;
                    fcntl(join,F_SETLKW,&loc2);
                    close(join);
                    write(fd,"Joint account successfully modified.Press -1 to exit\n",strlen("Joint account successfully modified.Press -1 to exit\n"));
                    break;
                default:
                    write(fd,"Invalid choice.Press -1 to exit\n",strlen("Invalid choice.Press -1 to exit\n"));
            }
            break;
        case '4':
            write(fd,"Enter the account you want to search \n1.Admin\n2.Single User\n3.Joint User\n",strlen("Enter the account you want to search \n1.Admin\n2.Single User\n3.Joint User\n"));
            char o3;
            readbytes=read(fd,&o3,1);
            struct flock loc1;struct admin detail; struct single detail1;struct joint detail2;
            switch(o3)
            {
                case '1':
                    strcpy(path,"./admin/");
                    write(fd,"Enter unqiue admin account number\n",strlen("Enter unqiue admin account number\n"));
                    readbytes=read(fd,account,4096);account[readbytes]='\0';
                    strcat(path,account);
                    int rec=open(path,O_RDWR);
                    if(rec==-1)
                    {write(fd,"Invalid account number.Press -1 to exit.\n",strlen("Invalid account number.Press -1 to exit.\n"));return;}
                    loc1.l_type=F_WRLCK;
                    loc1.l_whence=SEEK_SET;
                    loc1.l_start=0;
                    loc1.l_len=0;
                    fcntl(rec,F_SETLKW,&loc1);
                    dup2(rec,0);
                    getchar();
                    scanf("%[^\n]s",detail.name);
                    getchar();
                    scanf("%[^\n]s",detail.password);                    
                    dup2(std,0);
                    sprintf(buff,"Name :%s\nPassword :%s\nPress -1 to exit.\n",detail.name,detail.password);
                    write(fd,buff,strlen(buff));
                    loc1.l_type=F_UNLCK;
                    fcntl(rec,F_SETLKW,&loc1);
                    close(rec);
                    break;
                case '2':
                    strcpy(path,"./user/single/");
                    write(fd,"Enter unqiue single account number\n",strlen("Enter unqiue single account number\n"));
                    readbytes=read(fd,account,4096);account[readbytes]='\0';
                    strcat(path,account);
                    int rec1=open(path,O_RDWR);
                    if(rec1==-1)
                    {write(fd,"Invalid account number.Press -1 to exit.\n",strlen("Invalid account number.Press -1 to exit.\n"));return;}
                    loc1.l_type=F_WRLCK;
                    loc1.l_whence=SEEK_SET;
                    loc1.l_start=0;
                    loc1.l_len=0;
                    fcntl(rec1,F_SETLKW,&loc1);
                    dup2(rec1,0);
                    getchar();
                    scanf("%[^\n]s",detail1.name);
                    getchar();
                    scanf("%[^\n]s",detail1.password);
                    getchar();                    
                    char balan[4096];
                    scanf("%[^\n]s",balan);
                    getchar();
                    scanf("%[^;]s",detail1.log);                    
                    dup2(std,0);
                    sprintf(buff,"Name :%s\nPassword :%s\nBalance :%s\nTransaction Log:\n%sPress -1 to exit.\n",detail1.name,detail1.password,balan,detail1.log);
                    write(fd,buff,strlen(buff));
                    loc1.l_type=F_UNLCK;
                    fcntl(rec1,F_SETLKW,&loc1);
                    close(rec1);
                    break;
                case '3':
                    strcpy(path,"./user/joint/");
                    write(fd,"Enter unqiue joint account number\n",strlen("Enter unqiue joint account number\n"));
                    readbytes=read(fd,account,4096);account[readbytes]='\0';
                    strcat(path,account);
                    int rec2=open(path,O_RDWR);
                    if(rec2==-1)
                    {write(fd,"Invalid account number.Press -1 to exit.\n",strlen("Invalid account number.Press -1 to exit.\n"));return;}
                    loc1.l_type=F_WRLCK;
                    loc1.l_whence=SEEK_SET;
                    loc1.l_start=0;
                    loc1.l_len=0;
                    fcntl(rec2,F_SETLKW,&loc1);
                    dup2(rec2,0); 
                    getchar();                   
                    scanf("%[^\n]s",detail2.name1);
                    getchar();
                    scanf("%[^\n]s",detail2.name2);
                    getchar();
                    scanf("%[^\n]s",detail2.password);
                    getchar();
                    char bal[4096];
                    scanf("%[^\n]s",bal);
                    getchar();
                    scanf("%[^;]s",detail2.log);                    
                    dup2(std,0);
                    sprintf(buff,"Name :%s\nName :%s\nPassword :%s\nBalance :%s\nTransaction Log:\n%sPress -1 to exit.\n",detail2.name1,detail2.name2,detail2.password,bal,detail2.log);
                    write(fd,buff,strlen(buff));
                    loc1.l_type=F_UNLCK;
                    fcntl(rec2,F_SETLKW,&loc1);
                    close(rec2);
                    break;
                default:
                    write(fd,"Invalid Choice.Press -1 to exit\n",strlen("Invalid Choice.Press -1 to exit\n"));
            }
            break;
        case '5':
            write(fd,"Press -1 to exit\n",strlen("Press -1 to exit\n"));
            break;
        default:
            write(fd,"Invalid Choice.Press -1 to exit\n",strlen("Invalid Choice.Press -1 to exit\n"));
    }
    
    lock.l_type=F_UNLCK;
    fcntl(record,F_SETLKW,&lock);
    close(record);
    return;
}



void jointwindow(int fd)
{
    struct joint client; char account[4096],password[4096],path[5000],buff[170000];
    strcpy(path,"./user/joint/");
    write(fd,"Enter your unqiue account number\n",strlen("Enter your unqiue account number\n"));
    int readbytes=read(fd,account,4096);account[readbytes]='\0';
    strcat(path,account);
    int record=open(path,O_RDWR);
    if(record==-1)
    {write(fd,"Invalid account number.Press -1 to exit.\n",strlen("Invalid account number.Press -1 to exit.\n"));return;}
    struct flock lock;
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;
    fcntl(record,F_GETLK,&lock);
    if(lock.l_type!=F_UNLCK)
    {
     write(fd,"Server busy.Try after sometime.Press -1 to exit.\n",strlen("Server busy.Try after sometime.Press -1 to exit.\n"));
    }  
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;
    fcntl(record,F_SETLK,&lock);
    int std=dup(0);
    dup2(record,0);
    scanf("%[^\n]s",client.name1);
    getchar();
    scanf("%[^\n]s",client.name2);
    getchar();
    scanf("%[^\n]s",client.password);
    getchar();
    scanf("%ld",&client.balance);
    getchar();    
    scanf("%[^;]s",client.log);
    getchar();
    sprintf(buff,"Hello %s and %s, please enter your password \n",client.name1,client.name2);
    write(fd,buff,strlen(buff));
    readbytes=read(fd,password,4096);password[readbytes]='\0';
    if(strcmp(client.password,password)!=0)
    {write(fd,"Wrong Password.Press -1 to exit\n",strlen("Wrong Password.Press -1 to exit\n"));return;}
    sprintf(buff,"Welcome %s and %s\n",client.name1,client.name2);
    write(fd,buff,strlen(buff));
    write(fd,"1. Deposit\n2. Withdraw\n3. Balance Inquiry\n4. Password Change\n5. View Details\n6. Exit\n",strlen("1. Deposit\n2. Withdraw\n3. Balance Inquiry\n4. Password Change\n5. View Details\n6. Exit\n"));
    char option;
    read(fd,&option,1);long int amount;
    time_t t=time(NULL);
    struct tm *tm=localtime(&t);   
    switch(option)
    {
        case '1':
            write(fd,"Enter amount to deposit\n",strlen("Enter amount to deposit\n"));
            char deposit[4096];
            readbytes=read(fd,deposit,4096);deposit[readbytes]='\0';
            sscanf(deposit,"%ld",&amount);
            client.balance+=amount;                   
            sprintf(buff,"%sDeposit--Rs%ld\n",asctime(tm),amount);
            strcat(buff,client.log);
            strncpy(client.log,buff,4094);            
            lseek(record,0,SEEK_SET);
            sprintf(buff,"%s\n%s\n%s\n%ld\n%s",client.name1,client.name2,client.password,client.balance,client.log);
            write(record,buff,strlen(buff));
            write(fd,"Amount deposited successfully!Press -1 to exit\n",strlen("Amount deposited successfully!Press -1 to exit\n"));
            break;
        case '2':
            write(fd,"Enter amount to withdraw\n",strlen("Enter amount to withdraw\n"));
            char withdraw[4096];
            readbytes=read(fd,withdraw,4096);withdraw[readbytes]='\0';
            sscanf(withdraw,"%ld",&amount);
            client.balance-=amount;
            if(client.balance<0)
            {
            write(fd,"Insufficient balance!Press -1 to exit\n",strlen("Insufficient balance!Press -1 to exit\n"));
            return ;
            }
            sprintf(buff,"%sWithdraw--Rs%ld\n",asctime(tm),amount);
            strcat(buff,client.log);
            strncpy(client.log,buff,4094);
            
            lseek(record,0,SEEK_SET);
            sprintf(buff,"%s\n%s\n%s\n%ld\n%s\n",client.name1,client.name2,client.password,client.balance,client.log);
            write(record,buff,strlen(buff));
            write(fd,"Amount withdrawn successfully!Press -1 to exit\n",strlen("Amount withdrawn successfully!Press -1 to exit\n"));
            break;
        case '3':
            sprintf(buff,"Your balance is Rs%ld.\nPress -1 to exit\n",client.balance);
            write(fd,buff,strlen(buff));
            break;
        case '4':
            write(fd,"Enter new password.\n",strlen("Enter new password.\n"));
            char newpass[4096],confirm[4096];
            readbytes=read(fd,newpass,4096);newpass[readbytes]='\0';
            write(fd,"Confirm new password.\n",strlen("Confirm new password.\n"));
            readbytes=read(fd,confirm,4096);confirm[readbytes]='\0';
            if(strcmp(newpass,confirm)!=0)
            {
                 write(fd,"Password does not match.Press -1 to exit.\n",strlen("Password does not match.Press -1 to exit.\n"));
                 return;
            }
            strcpy(client.password,newpass);
            lseek(record,0,SEEK_SET);
            sprintf(buff,"%s\n%s\n%s\n%ld\n",client.name1,client.name2,client.password,client.balance);
            write(record,buff,strlen(buff));
            write(fd,"Password set successfully!Press -1 to exit\n",strlen("Password set successfully!Press -1 to exit\n"));
            break;
        case '5':
            sprintf(buff,"Name of first holder:%s\nName of second holder:%s\nBalance:%ld\nTransaction Log:\n%s",client.name1,client.name2,client.balance,client.log);
            write(fd,buff,strlen(buff));
            write(fd,"Press -1 to exit\n",strlen("Press -1 to exit\n"));
            break;
        case '6':
            write(fd,"Press -1 to exit\n",strlen("Press -1 to exit\n"));
            break;
        default:
            write(fd,"Invalid option. Press -1 to exit\n",strlen("Invalid option. Press -1 to exit\n"));
    }
    lock.l_type=F_UNLCK;
    fcntl(record,F_SETLKW,&lock);
    close(record);
    return;
}



void singlewindow(int fd)
{
    struct single client; char account[4096],password[4096],path[5000],buff[17000];
    strcpy(path,"./user/single/");
    write(fd,"Enter your unqiue account number\n",strlen("Enter your unqiue account number\n"));
    int readbytes=read(fd,account,4096);account[readbytes]='\0';
    strcat(path,account);
    int record=open(path,O_RDWR);
    if(record==-1)
    {write(fd,"Invalid account number.Press -1 to exit.\n",strlen("Invalid account number.Press -1 to exit.\n"));return;}
    struct flock lock;
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;
    fcntl(record,F_GETLK,&lock);
    if(lock.l_type!=F_UNLCK)
    {
     write(fd,"Server busy.Try after sometime.Press -1 to exit.\n",strlen("Server busy.Try after sometime.Press -1 to exit.\n"));
    }  
    lock.l_type=F_WRLCK;
    lock.l_whence=SEEK_SET;
    lock.l_start=0;
    lock.l_len=0;
    lock.l_pid=getpid();
    fcntl(record,F_SETLK,&lock);  
    int std=dup(0);
    dup2(record,0);
    scanf("%[^\n]s",client.name);
    getchar();
    scanf("%[^\n]s",client.password);
    getchar();
    scanf("%ld",&client.balance);
    getchar();
    scanf("%[^;]s",client.log);
    getchar();
    sprintf(buff,"Hello %s , please enter your password \n",client.name);
    write(fd,buff,strlen(buff));
    readbytes=read(fd,password,4096);password[readbytes]='\0';
    if(strcmp(client.password,password)!=0)
    {write(fd,"Wrong Password.Press -1 to exit\n",strlen("Wrong Password.Press -1 to exit\n"));return;}
    sprintf(buff,"Welcome %s\n",client.name);
    write(fd,buff,strlen(buff));
    write(fd,"1. Deposit\n2. Withdraw\n3. Balance Inquiry\n4. Password Change\n5. View Details\n6. Exit\n",strlen("1. Deposit\n2. Withdraw\n3. Balance Inquiry\n4. Password Change\n5. View Details\n6. Exit\n"));
    dup2(std,0);
    close(std);
    char option;
    read(fd,&option,1);long int amount;
    time_t t=time(NULL);
    struct tm *tm=localtime(&t);  
    switch(option)
    {
        case '1':
            write(fd,"Enter amount to deposit\n",strlen("Enter amount to deposit\n"));
            char deposit[4096];
            readbytes=read(fd,deposit,4096);deposit[readbytes]='\0';
            sscanf(deposit,"%ld",&amount);
            client.balance+=amount;
            sprintf(buff,"%sDeposit--Rs%ld\n",asctime(tm),amount);
            strcat(buff,client.log);
            strncpy(client.log,buff,4094); 
            lseek(record,0,SEEK_SET);
            sprintf(buff,"%s\n%s\n%ld\n%s",client.name,client.password,client.balance,client.log);
            write(record,buff,strlen(buff));
            write(fd,"Amount deposited successfully!Press -1 to exit\n",strlen("Amount deposited successfully!Press -1 to exit\n"));
            break;
        case '2':
            write(fd,"Enter amount to withdraw\n",strlen("Enter amount to withdraw\n"));
            char withdraw[4096];
            readbytes=read(fd,withdraw,4096);withdraw[readbytes]='\0';
            sscanf(withdraw,"%ld",&amount);
            client.balance-=amount;
            if(client.balance<0)
            {
            write(fd,"Insufficient balance!Press -1 to exit\n",strlen("Insufficient balance!Press -1 to exit\n"));
            return ;
            }
            sprintf(buff,"%sWithdraw--Rs%ld\n",asctime(tm),amount);
            strcat(buff,client.log);
            strncpy(client.log,buff,4094); 
            lseek(record,0,SEEK_SET);
            sprintf(buff,"%s\n%s\n%ld\n%s",client.name,client.password,client.balance,client.log);
            write(record,buff,strlen(buff));
            write(fd,"Amount withdrawn successfully!Press -1 to exit\n",strlen("Amount withdrawn successfully!Press -1 to exit\n"));
            break;
        case '3':
            sprintf(buff,"Your balance is Rs%ld.\nPress -1 to exit\n",client.balance);
            write(fd,buff,strlen(buff));
            break;
        case '4':
            write(fd,"Enter new password.\n",strlen("Enter new password.\n"));
            char newpass[4096],confirm[4096];
            readbytes=read(fd,newpass,4096);newpass[readbytes]='\0';
            write(fd,"Confirm new password.\n",strlen("Confirm new password.\n"));
            readbytes=read(fd,confirm,4096);confirm[readbytes]='\0';
            if(strcmp(newpass,confirm)!=0)
            {
                 write(fd,"Password does not match.Press -1 to exit.\n",strlen("Password does not match.Press -1 to exit.\n"));
                 return;
            }
            strcpy(client.password,newpass);
            lseek(record,0,SEEK_SET);
            sprintf(buff,"%s\n%s\n%ld\n",client.name,client.password,client.balance);
            write(record,buff,strlen(buff));
            write(fd,"Password set successfully!Press -1 to exit\n",strlen("Password set successfully!Press -1 to exit\n"));
            break;
        case '5':
            sprintf(buff,"Name:%s\nBalance:%ld\nTransaction Log:\n%s",client.name,client.balance,client.log);
            write(fd,buff,strlen(buff));
            write(fd,"Press -1 to exit\n",strlen("Press -1 to exit\n"));
            break;
        case '6':
            write(fd,"Press -1 to exit\n",strlen("Press -1 to exit\n"));
            break;
        default:
            write(fd,"Invalid option. Press -1 to exit\n",strlen("Invalid option. Press -1 to exit\n"));
    }
    lock.l_type=F_UNLCK;
    fcntl(record,F_SETLKW,&lock);
    close(record);
    return;
}




int main(){
    int server_fd,client_fd;
    int slen,clen;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    server_fd=socket(AF_INET,SOCK_STREAM,0);
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=inet_addr("127.0.0.1");
    server_address.sin_port=htons(9734);
    slen=sizeof(server_address);
    bind(server_fd,(struct sockaddr *)&server_address,slen);
    listen(server_fd,5);
    while(1)
    {
        char c;
        clen=sizeof(client_address);
        client_fd=accept(server_fd,(struct sockaddr *)&client_address,&clen);
        if(client_fd==-1)
        {perror("Error occured\n");return 0;}        
            write(client_fd,"Welcome to online banking\nEnter your account type\n1.Admin\n2.Single\n3.Joint\n",strlen("Welcome to online banking\nEnter your account type\n1.Admin\n2.Single\n3.Joint\n"));
            read(client_fd,&c,1);
        if(fork()==0)
         {
            switch(c)
            {
                case '1':
                    adminwindow(client_fd);
                    break;
                case '2':
                    singlewindow(client_fd);
                    break;
                case '3':
                    jointwindow(client_fd);
                    break;
                default:
                    write(client_fd,"Invalid option.Press -1 to exit \n",strlen("Invalid option.Press -1 to exit \n"));
            }
            close(client_fd);exit(0);
         }   
       close(client_fd);        
    }
    return 0;
}

