#pragma once
typedef struct _cilentudp clientudp;
struct _cilentudp {
  //char *plugin;
  //void *handle;
  //output_parameter param;

  int (*init)(char *,int );
  int (*send)(unsigned long long ,unsigned char *);
  int (*recv)(unsigned long long *,unsigned char *);
  int (*destory)();
};

extern int client_udp_init(char *ser_ip,int ser_port);
extern int client_udp_send(unsigned long long id,unsigned char *databuf);
extern int client_udp_recv(unsigned long long *id,unsigned char *databuf);
extern int client_udp_destory();


void create_client_udp(clientudp *sock)
{
    sock->init = client_udp_init;
    sock->send = client_udp_send;
    sock->recv = client_udp_recv;
    sock->destory = client_udp_destory;
}
