#pragma once
typedef struct _audiocard audiocard;
struct _audiocard {
  //char *plugin;
  //void *handle;
  //output_parameter param;

  int (*init)(int ,int ,int );
  int (*write)(unsigned char *);
  int (*read)(unsigned char *);
  int (*destory)();
};

extern int audio_init(int rate,int size,int channels);
extern int audio_write(unsigned char *databuf);
extern int audio_read(unsigned char *databuf);
extern int audio_destory();


void create_audio(audiocard *audio)
{
    audio->init = audio_init;
    audio->write = audio_write;
    audio->read = audio_read;
    audio->destory = audio_destory;
}
