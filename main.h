#pragma once
#define BUFLEN 1024

typedef struct _package package;
struct _package
{
    unsigned long long id; //package's id
    unsigned char data[BUFLEN];    //audio data
};


