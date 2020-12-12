#include "musicBuffer.h"

void MusicBuffer::Play()
{
  ALsizei i;

  // clear any al errors
  alGetError();

  /* Rewind the source position and clear the buffer queue */
  alSourceRewind(source);
  alSourcei(source, AL_BUFFER, 0);

  /* Fill the buffer queue */
  for (i = 0; i < numBuffers; i++)
  {
    /* Get some data to give it to the buffer */
    sf_count_t slen = sf_readf_short(sndFile, membuf, bufferSamples);
    if (slen < 1) break;

    slen *= sfinfo.channels * (sf_count_t)sizeof(short);
    alBufferData(buffers[i], format, membuf, (ALsizei)slen, sfinfo.samplerate);
  }
  if (alGetError() != AL_NO_ERROR)
  {
    throw("Error buffering for playback");
  }

  /* Now queue and start playback! */
  alSourceQueueBuffers(source, i, buffers);
  alSourcePlay(source);
  if (alGetError() != AL_NO_ERROR)
  {
    throw("Error starting playback");
  }
}

void MusicBuffer::Pause()
{
  alSourcePause(source);
}

void MusicBuffer::Stop()
{
  alSourceStop(source);
}

void MusicBuffer::Resume()
{
  alSourcePlay(source);
}

void MusicBuffer::UpdateBufferStream()
{
  ALint processed, state;

  // clear error
  alGetError();
  /* Get relevant source info */
  alGetSourcei(source, AL_SOURCE_STATE, &state);
  alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
  if (alGetError() != AL_NO_ERROR)
  {
    throw("error checking music source state");
  }

  /* Unqueue and handle each processed buffer */
  while (processed > 0)
  {
    ALuint bufid;
    sf_count_t slen;

    alSourceUnqueueBuffers(source, 1, &bufid);
    processed--;

    /* Read the next chunk of data, refill the buffer, and queue it
     * back on the source */
    slen = sf_readf_short(sndFile, membuf, bufferSamples);
    if (slen > 0)
    {
      slen *= sfinfo.channels * (sf_count_t)sizeof(short);
      alBufferData(bufid, format, membuf, (ALsizei)slen, sfinfo.samplerate);
      alSourceQueueBuffers(source, 1, &bufid);
    }
    if (alGetError() != AL_NO_ERROR)
    {
      throw("error buffering music data");
    }
  }

  /* Make sure the source hasn't underrun */
  if (state != AL_PLAYING && state != AL_PAUSED)
  {
    ALint queued;

    /* If no buffers are queued, playback is finished */
    alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
    if (queued == 0) return;

    alSourcePlay(source);
    if (alGetError() != AL_NO_ERROR)
    {
      throw("error restarting music playback");
    }
  }
}

ALint MusicBuffer::GetSource()
{
  return source;
}

bool MusicBuffer::IsPlaying()
{
  ALint state;
  alGetSourcei(source, AL_SOURCE_STATE, &state);
  return (state == AL_PLAYING);
}

MusicBuffer::MusicBuffer(const char* filename)
{
  alGenSources(1, &source);
  alGenBuffers(numBuffers, buffers);

  std::size_t frame_size;

  sndFile = sf_open(filename, SFM_READ, &sfinfo);
  if (!sndFile)
  {
    throw("could not open provided music file -- check path");
  }

  /* Get the sound format, and figure out the OpenAL format */
  if (sfinfo.channels == 1)
    format = AL_FORMAT_MONO16;
  else if (sfinfo.channels == 2)
    format = AL_FORMAT_STEREO16;
  else if (sfinfo.channels == 3)
  {
    if (sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
      format = AL_FORMAT_BFORMAT2D_16;
  }
  else if (sfinfo.channels == 4)
  {
    if (sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
      format = AL_FORMAT_BFORMAT3D_16;
  }
  if (!format)
  {
    sf_close(sndFile);
    sndFile = NULL;
    throw("Unsupported channel count from file");
  }

  frame_size = ((size_t)bufferSamples * (size_t)sfinfo.channels) * sizeof(short);
  membuf = static_cast<short*>(malloc(frame_size));
}

MusicBuffer::~MusicBuffer()
{
  alDeleteSources(1, &source);

  if (sndFile) sf_close(sndFile);

  sndFile = nullptr;

  free(membuf);

  alDeleteBuffers(numBuffers, buffers);
}