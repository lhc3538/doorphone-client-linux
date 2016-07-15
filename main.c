#include <stdio.h>
#include "sockudp.h"
void create_sock(sockudp *sock)
{
    sock->init = sock_init;
    sock->send = sock_send;
    sock->recv = sock_recv;
    sock->destory = sock_destory;
}

int main(void)
{
    /*sockudp sock;
    create_sock(&sock);
    sock.init("127.0.0.1",8081);*/
    //sock_init("127.0.0.1",8081);
    printf("Hello World!\n");
    return 0;
}


