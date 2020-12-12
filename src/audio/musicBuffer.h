#pragma once
#include <AL/al.h>
#include <AL/alext.h>
#include <sndfile.h>
#include <cstddef>
#include <malloc.h>

class MusicBuffer
{
 public:
  void Play();
  void Pause();
  void Stop();
  void Resume();

  void UpdateBufferStream();

  ALint GetSource();

  bool IsPlaying();

  MusicBuffer(const char* filename);
  ~MusicBuffer();

 private:
  ALuint source;
  static const int bufferSamples = 8192;
  static const int numBuffers = 4;
  ALuint buffers[numBuffers];
  SNDFILE* sndFile;
  SF_INFO sfinfo;
  short* membuf;
  ALenum format;

  MusicBuffer() = delete;
};
