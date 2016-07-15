#define DEFAULT_PORT 8081
#define BUFLEN 1024

typedef struct
{
    unsigned long long id; //package's id
    unsigned char data[BUFLEN];    //audio data
} Package;
