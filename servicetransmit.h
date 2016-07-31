#pragma once
typedef struct _servicetransmit servicetransmit;
struct _servicetransmit {
  int (*run)(char *,int );
  int (*stop)();
};

extern int start_trans_witn_remote(char *serIp,int serPort);
extern int stop_trans_with_remote();

void create_service_transmit(servicetransmit *service)
{
    service->run = start_trans_witn_remote;
    service->stop = stop_trans_with_remote;
}
