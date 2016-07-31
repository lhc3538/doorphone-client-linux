#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "main.h"
#include "clientudp.h"
#include "audio.h"

clientudp client_udp;   //udp客户端操作结构体
audiocard audio;    //声卡操作结构体

/**发送音频数据到服务器线程函数
  */
void *fun_to_remote()
{
    unsigned long long id = 10;
    int rul;
    unsigned char buffer[BUFLEN];
    while(1)
    {
        audio.read(buffer); //录音
        client_udp.send(id++,buffer);    //发送
    }
}

/**从服务器接受音频数据线程函数
  */
void *fun_from_remote()
{
    unsigned long long old_id = 0,new_id = 0;
    int rul;
    unsigned char buffer[BUFLEN];
    while(1)
    {
        client_udp.recv(&new_id,buffer);    //接收
        mydebug(buffer);
        if (new_id > old_id)
        {
            //printf("%llu\n",new_id);
            audio.write(buffer);    //放音
        }
    }
}


//双向线程
pthread_t thread_to_remote,thread_from_remote;

/**
 * @brief start_trans_witn_remote 开启与云服务器音频传输
 * @param serIp 服务器ip
 * @param serPort 服务器端口
 * @return 操作结果
 */
int start_trans_witn_remote(char *serIp,int serPort)
{
    create_client_udp(&client_udp); //创建结构体函数连接
    create_audio(&audio);   //创建结构体函数连接

    int rul;
    rul = client_udp.init("192.168.0.80",8082);    //初始化
    if (rul < 0)
        return rul;
    rul = audio.init(22050,16,2);   //初始化
    //rul = audio.init(22050,16,2);   //初始化
    if (rul < 0)
        return rul;


    memset(&thread_to_remote, 0, sizeof(thread_to_remote));
    memset(&thread_from_remote, 0, sizeof(thread_from_remote));

    //创建向云服务器发送数据的线程
    if ((pthread_create(&thread_to_remote, NULL, fun_to_remote, NULL)) < 0)
        perror("create thread_to_remote failed!");
    //创建从云服务器接收数据的线程
    if ((pthread_create(&thread_from_remote, NULL, fun_from_remote, NULL)) < 0)
        perror("create thread_from_remote failed!");

//    if (thread_to_remote != 0)
//        pthread_join(thread_to_remote,NULL);//等待线程退出
//    if (thread_from_remote != 0)
//        pthread_join(thread_from_remote,NULL);//等待线程退出
    return 0;
}

/**
 * @brief stop_trans_with_remote 停止与服务器数据传输
 * @return 操作结果
 */
int stop_trans_with_remote()
{
    //停止双向线程
    pthread_cancel(thread_from_remote);
    pthread_cancel(thread_to_remote);
    if (thread_to_remote != 0)
        pthread_join(thread_to_remote,NULL);//等待线程退出
    if (thread_from_remote != 0)
        pthread_join(thread_from_remote,NULL);//等待线程退出
    //关闭操作
    client_udp.destory();
    audio.destory();
}








