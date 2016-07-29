#pragma once
typedef struct _cilenttcp clienttcp;
struct _cilenttcp {
  //char *plugin;
  //void *handle;
  //output_parameter param;

  int (*init)(char *,int);
  int (*sendln)(char *);
  int (*recvln)(char *);
  int (*destory)();
};

extern int client_tcp_init(char *ser_ip,int ser_port);
extern int client_tcp_sendln(char *databuf);
extern int client_tcp_recvln(char *databuf);
extern int client_tcp_destory();


void create_client_tcp(clienttcp *sock)
{
    sock->init = client_tcp_init;
    sock->sendln = client_tcp_sendln;
    sock->recvln = client_tcp_recvln;
    sock->destory = client_tcp_destory;
}
