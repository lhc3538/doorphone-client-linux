#include <stdio.h>
#include "main.h"
#include "clientudp.h"
#include "audio.h"

/*开启与云服务器音频传输
  */
void trans_with_remote()
{
    pthread_t thread_to_remote,thread_from_remote;
    memset(&thread_to_remote, 0, sizeof(thread_to_remote));
    memset(&thread_from_remote, 0, sizeof(thread_from_remote));

    //创建向云服务器发送数据的线程
    if ((pthread_create(&thread_to_remote, NULL, fun_to_remote, NULL)) < 0)
        perror("create thread_to_remote failed!");
    //创建从云服务器接收数据的线程
    if ((pthread_create(&thread_from_remote, NULL, fun_to_remote, NULL)) < 0)
        perror("create thread_from_remote failed!");

    if (thread_to_remote != 0)
        pthread_join(thread_to_remote,NULL);//等待线程退出
    if (thread_from_remote != 0)
        pthread_join(thread_from_remote,NULL);//等待线程退出
}

/*发送音频数据到服务器线程函数
*/
void *fun_to_remote()
{
    Package pack;
    int pack_len = sizeof(Package);
    char buf_sock[pack_len];
    int ret;
    while(1)
    {
        pthread_mutex_lock(&mutex_isbusy_audio);
        if (isbusy_audio == 1)  //be used in local
        {
            pthread_mutex_unlock(&mutex_isbusy_audio);
            continue;
        }
        else
            pthread_mutex_unlock(&mutex_isbusy_audio);

        pthread_mutex_lock(&mutex_local_send);
        while(current_send_id == 0)
        {
            pthread_cond_wait(&cond_local,&mutex_local_send);   //wait connnect
            current_send_id = 1;
        }
        pack.id = current_send_id++;
        pthread_mutex_unlock(&mutex_local_send);
        //read audio from audio-card
        ret = read(fd_audio, pack.data, BUFLEN); // 录音
        memcpy(buf_sock,&pack,pack_len);
        //printf("send%llu:%s\n",pack.id,pack.data);
        //addNum(sendbuf);
        write(sock_remote, buf_sock, pack_len);
    }
}

/*从服务器接受音频数据线程函数
 * */
void *fun_from_remote()
{
    Package pack;
    int pack_len = sizeof(Package);
    char buf_sock[pack_len];
    int ret;
    while(1)
    {
        pthread_mutex_lock(&mutex_isbusy_audio);
        if (isbusy_audio == 1)  //be used in local
        {
            pthread_mutex_unlock(&mutex_isbusy_audio);
            continue;
        }
        else
            pthread_mutex_unlock(&mutex_isbusy_audio);

        ret = read(sock_remote, buf_sock, pack_len);
        if (ret == -1)
        {
            perror("recvfrom err");
            continue;
        }

        memcpy(&pack,buf_sock,pack_len);
        //printf("recv%llu:%s\n",pack.id,pack.data);

        //write audio buf to audio-card
        pthread_mutex_lock(&mutex_local_recv);   //lock
        if (pack.id > current_recv_id)
        {
            pthread_mutex_lock(&mutex_isbusy_audio);
            if (isbusy_audio == 0)
                isbusy_audio = 2;   //set be used in remote
            pthread_mutex_unlock(&mutex_isbusy_audio);   //unlock

            pthread_mutex_lock(&mutex_local_send);
            pthread_cond_signal(&cond_local);   //open send
            pthread_mutex_unlock(&mutex_local_send);   //unlock


            current_recv_id = pack.id;
            pthread_mutex_unlock(&mutex_local_recv);   //unlock
            ret = write(fd_audio, pack.data, BUFLEN); // 放音
            if (ret != BUFLEN)
                perror("wrote wrong number of bytes");
        }
        else
            pthread_mutex_unlock(&mutex_local_recv);   //unlock

    }
}

int main(void)
{
    int rul;

    clientudp client;
    create_client(&client);
    rul = client.init("127.0.0.1",8081);

    audiocard audio;
    create_audio(&audio);
    rul = audio.init(22050,16,2);
    if (rul < 0)
        return 1;

    int id;
    char buf[BUFLEN];
    while(1)
    {

        client.recv(&id,buf);
        if (id == 1)

        audio.read(buf);
        client.send(0,buf);
    }

    //sock_init("127.0.0.1",8081);
    //printf("Hello World!\n");
    return 0;
}






