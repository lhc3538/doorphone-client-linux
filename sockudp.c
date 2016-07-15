/*
   doorphone udp client
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

void mydebug(char *s)
{
    perror(s);
}
/*
  *初始化套接字
  *传入：服务器ip和端口
  *返回：套接字描述符
  */
int init_sock(char *ser_ip,int ser_port)
{
    int rul;    //结果返回值
    struct sockaddr_in si_ser;    //服务器地址结构体
    int sock;   //套接字描述符
    if ( (sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) //创建套接字
        perror("create socket failed");

    memset((char *) &si_ser, 0, sizeof(si_ser));
    si_ser.sin_family = AF_INET;
    si_ser.sin_port = htons(PORT);  //写入服务器端口
    rul = inet_aton(ser_ip , &si_other.sin_addr);   //写入服务器IP
    if (rul == 0)
        perror("inet_aton() failed");
    return sock;
}

int main(void)
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    while(1)
    {
        printf("Enter message : ");
        gets(message);

        //send the message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            die("sendto()");
        }

        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', BUFLEN);
        //try to receive some data, this is a blocking call
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            die("recvfrom()");
        }

        puts(buf);
    }

    close(s);
    return 0;
}
