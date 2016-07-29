#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>
#include "main.h"

int fd_audio;   //声音设备的文件描述符

int audio_init(int rate,int size,int channels)
{
    int arg;	// 用于ioctl调用的参数
    int rul = 0;   // 系统调用的返回值
    // 打开声音设备
    fd_audio = open("/dev/dsp", O_RDWR);
    if (fd_audio < 0)
    {
        perror("open of /dev/dsp failed");
        return -1;
    }
    // 设置采样时的量化位数
    arg = size;
    rul = ioctl(fd_audio, SOUND_PCM_WRITE_BITS, &arg);
    if (rul == -1)
        perror("SOUND_PCM_WRITE_BITS ioctl failed");
    if (arg != size)
        perror("unable to set sample size");
    // 设置采样时的声道数目
    arg = channels;
    rul = ioctl(fd_audio, SOUND_PCM_WRITE_CHANNELS, &arg);
    if (rul == -1)
        perror("SOUND_PCM_WRITE_CHANNELS ioctl failed");
    if (arg != channels)
        perror("unable to set number of channels");
    // 设置采样时的采样频率
    arg = rate;
    rul = ioctl(fd_audio, SOUND_PCM_WRITE_RATE, &arg);
    if (rul == -1)
        perror("SOUND_PCM_WRITE_WRITE ioctl failed");
    return rul;
}

int audio_write(unsigned char *databuf)
{
    int rul;
    printf("size=%d:%s\n",sizeof(databuf),databuf);
    rul = write(fd_audio, databuf, BUFLEN); // 放音
    if (rul != BUFLEN)
        perror("wrote wrong number of bytes");
    return rul;
}

int audio_read(unsigned char *databuf)
{
    int rul;
    rul = read(fd_audio, databuf, BUFLEN); // 录音
    if (rul != BUFLEN)
        perror("read wrong number of bytes");
    return rul;
}

int audio_destory()
{
    close(fd_audio);
}
