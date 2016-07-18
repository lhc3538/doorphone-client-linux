/*   doorphone udp client
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "main.h"

int sock = -1;   //套接字描述符
struct sockaddr_in si_ser;    //服务器地址结构体
int pack_len = sizeof(package); //包长度
int si_ser_len;    //地址结构体长度
pthread_mutex_t mutex_send; //发送函数锁

void mydebug(char *s)
{
    printf("%s\n",s);
}
/**将id和数据合并为包
  *传入：id、数据
  *返回：合成后的包
  */
package combine_pack(unsigned long long id,char *databuf)
{
    package pack;
    pack.id = id;
    strcpy(pack.data,databuf);
    return pack;
}
/**将包拆分为id和数据
  *传入：id指针、数据指针
  *返回：操作结果
  */
int split_pack(package pack,unsigned long long *id,char *databuf)
{
    (*id) = pack.id;
    strcpy(databuf,pack.data);
    return 0;
}
/**初始化套接字
  *传入：服务器ip和端口
  *返回：套接字描述符
  */
int client_init(char *ser_ip,int ser_port)
{
    if (sock != -1)
    {
        client_destory(); //已经初始化一次，先关闭
    }
    int rul;    //结果返回值
    if ( (sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) //创建套接字
    {
        perror("create socket failed");
        return -1;
    }

    memset((char *) &si_ser, 0, sizeof(si_ser));
    si_ser.sin_family = AF_INET;
    si_ser.sin_port = htons(ser_port);  //写入服务器端口
    rul = inet_aton(ser_ip , &si_ser.sin_addr);   //写入服务器IP
    if (rul == 0)
    {
        perror("inet_aton() failed");
        return -1;
    }
    si_ser_len = sizeof(si_ser);

    pthread_mutex_init(&mutex_send,NULL);  //初始化发送锁
    mydebug("ok");
    return 0;
}
/**套接字发送
  *传入：数据包id、数据
  *返回：发送结果
  */
int client_send(unsigned long long id,char *databuf)
{
    pthread_mutex_lock(&mutex_send);    //加发送锁
    if (sock == -1)
        return -2;  //未初始化
    int rul;
    char buf[pack_len];
    package pack = combine_pack(id,databuf);    //合成包
    memcpy(buf,&pack,pack_len); //结构体序列化
    //发送
    rul = sendto(sock, buf, pack_len , 0 , (struct sockaddr *) &si_ser, si_ser_len);
    if (rul < 0)
        perror("send failed");
    pthread_mutex_unlock(&mutex_send);  //解发送锁
    return 0;
}
/**套接字接收
  *传入：id指针、数据指针
  *返回：接受结果
  */
int client_recv(unsigned long long *id,char *databuf)
{
    if (sock == -1)
        return -2;  //未初始化
    int rul;
    char buf[pack_len];
    package pack;
    rul = recvfrom(sock, buf, pack_len, 0, (struct sockaddr *) &si_ser, &si_ser_len);
    if (rul < 0)
        perror("recv failed");
    memcpy(&pack,buf,pack_len);
    split_pack(pack,id,databuf);
    return rul;
}
/**套接字关闭
  */
void client_destory()
{
    close(sock);
    sock = -1;
    pthread_mutex_destroy(&mutex_send); //销毁发送锁
}

