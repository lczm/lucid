#include "soundEffectsPlayer.h"

SoundEffectsPlayer::SoundEffectsPlayer() {
  alGenSources(1, &source);
  alSourcei(source, AL_BUFFER, buffer);
}

SoundEffectsPlayer::~SoundEffectsPlayer() {
  alDeleteSources(1, &source);
}

void SoundEffectsPlayer::Play(const ALuint& buffer_to_play) {
  if (buffer_to_play != buffer) {
    buffer = buffer_to_play;
    alSourcei(source, AL_BUFFER, (ALint)buffer);
  }

  alSourcePlay(source);
}

void SoundEffectsPlayer::Stop() {
  alSourceStop(source);
}

void SoundEffectsPlayer::Pause() {
  alSourcePause(source);
}

void SoundEffectsPlayer::Resume() {
  alSourcePlay(source);
}

void SoundEffectsPlayer::SetBufferToPlay(const ALuint& buffer_to_play) {
  if (buffer_to_play != buffer) {
    buffer = buffer_to_play;
    alSourcei(source, AL_BUFFER, (ALint)buffer);
  }
}

void SoundEffectsPlayer::SetLooping(const bool& loop) {
  alSourcei(source, AL_LOOPING, (ALint)loop);
}

bool SoundEffectsPlayer::IsPlaying() {
  ALint playState;
  alGetSourcei(source, AL_SOURCE_STATE, &playState);
  return (playState == AL_PLAYING);
}
