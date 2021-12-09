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
#include<ncurses.h>
#include<unistd.h>
int main(){
    int sockfd;int len;
    struct sockaddr_in address;
    int result;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=inet_addr("127.0.0.1");
    address.sin_port=htons(9734);
    len=sizeof(address);
    result=connect(sockfd,(struct sockaddr *)&address,len);
    if(result==-1)
    {perror("Error occured\n");return 1;}
    char query[4096];
    fflush(stdin);
    while(1)
    {
        char rd[17000];
        int bytes=read(sockfd,rd,17000);rd[bytes]='\0';
        if(bytes>0)
        {printf("%s",rd);}
        if(strstr(rd,"password")!=NULL)
        { 
         
             char *password;
             password=getpass("Password:");
             strcpy(query,password);
         
        }
        else
        {
        bytes=read(0,query,4096);query[bytes-1]='\0';
        }
        if(strcmp(query,"-1")!=0)
            write(sockfd,query,strlen(query));
        else break;
    }
    close(sockfd);
    return 0;
}
