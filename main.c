#include <stdio.h>
#include "main.h"
#include "clientudp.h"
#include "audio.h"



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

    char buf[BUFLEN];
    while(1)
    {
        audio.read(buf);
        client.send(0,buf);
    }

    //sock_init("127.0.0.1",8081);
    //printf("Hello World!\n");
    return 0;
}


