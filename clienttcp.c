/**tcp客户端简单封装
  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int sock_client_tcp = -1;

/**
 * @brief tcp连接操作
 * @param ser_ip
 * @param ser_port
 * @return
 */
int client_tcp_init(char *ser_ip,int ser_port)
{
    struct sockaddr_in server_addr;
    struct hostent * host;
    if ((host = gethostbyname(ser_ip)) == NULL)
    {
        perror("gethostbyname 失败");
        return -1;
    }
    //客户端开始建立sockfd描述符
    if ((sock_client_tcp = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("tcp socket创建失败");
        return -1;
    }
    bzero(&server_addr,sizeof(server_addr));//初始化，置0.
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(ser_port);//将本机的short数据转化为网络的short数据
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    //客户程序发起连接请求
    if (connect(sock_client_tcp,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr)) == -1)
    {
        perror("连接失败");
        return -1;
    }
    return 0;
}

/**
 * @brief client_tcp_sendln 发送一行数据
 * @param databuf
 * @return
 */
int client_tcp_sendln(char *databuf)
{
    int len = strlen(databuf);
    databuf[len] = '\n';
    databuf[len+1] = 0;
    if (send(sock_client_tcp,databuf,len+1,0) == -1)
    {
        perror("tcp发送失败");
        return -1;
    }
    return 0;
}

/**
 * @brief client_tcp_recvln 接受一行数据
 * @param databuf
 * @return
 */
int client_tcp_recvln(char *databuf)
{
    int len;
    len = recv(sock_client_tcp,databuf,sizeof(databuf),0);
    if (len == -1)
    {
        perror("tcp接收失败");
        return -1;
    }
    //去掉换行符
    if (databuf[len-1] == '\n')
        databuf[len-1] = 0;
    return 0;
}

/**
 * @brief client_tcp_destory 销毁tcp套接字
 * @return
 */
int client_tcp_destory()
{
    close(sock_client_tcp);
    sock_client_tcp = -1;
    return 0;
}

