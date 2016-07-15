typedef struct _sockudp sockudp;
struct _sockudp {
  //char *plugin;
  //void *handle;
  //output_parameter param;

  int (*init)(char *,int );
  int (*send)(unsigned long long ,char *);
  int (*recv)(unsigned long long *,char *);
  int (*destory)();
};

int sock_init(char *ser_ip,int ser_port);
int sock_send(unsigned long long id,char *databuf);
int sock_recv(unsigned long long *id,char *databuf);
int sock_destory();
