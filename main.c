#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "main.h"
#include "clienttcp.h"
#include "servicetransmit.h"

/**
 * @brief 自己的调试输出函数
 * @param s
 */
void mydebug(char *s)
{
    printf("%s\n",s);
}


int main(void)
{
    int rul;

    clienttcp client_tcp;
    create_client_tcp(&client_tcp);
    if(client_tcp.init("192.168.0.240",1025) == -1)
        mydebug("连接失败");
    if(client_tcp.sendln("home;3538\n") == -1)
        mydebug("发送失败");
    char comdata[128];
    servicetransmit service_transmit;
    create_service_transmit(&service_transmit);
    while(1)
    {
        if (client_tcp.recvln(comdata) == -1)
        {
            mydebug("服务器断开连接");
            break;
        }
        else
        {
            if (strlen(comdata) == 0)
                continue;
            else if (comdata[0] == 'c')
                service_transmit.stop();
            else if (comdata[0] == 's')
                mydebug("与服务器成功建立连接");
            else
                service_transmit.run("192.168.0.240",atoi(comdata));
            mydebug(comdata);
        }
    }
    client_tcp.destory();
    return 0;
}






