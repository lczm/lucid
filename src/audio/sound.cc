#include "sound.h"

Sound::Sound()
{
  sd = SoundDevice::Get();
  seLibrary = SoundEffectsLibrary::Get();
  player = new SoundEffectsPlayer();
  play = false;
  looping = false;
  gain = 1;
}

Sound::Sound(std::string path)
{
  sd = SoundDevice::Get();
  seLibrary = SoundEffectsLibrary::Get();
  player = new SoundEffectsPlayer();
  play = false;
  looping = false;
  gain = 1;
  Sound::path = path;

  soundBuffer = seLibrary->Load(path.c_str());
}

Sound::~Sound()
{
  seLibrary->UnLoad(soundBuffer);
}

void Sound::Play()
{
  player->Play(soundBuffer);
}

void Sound::SetPosition(float x, float y, float z)
{
  player->SetPosition(x, y, z);
}

void Sound::SetLooping(bool loop)
{
  player->SetLooping(loop);
}

void Sound::SetFilePath(std::string path)
{
  if (Sound::path != path)
  {
    // Unload previous sound buffer
    seLibrary->UnLoad(soundBuffer);
    // Load new sound buffer
    soundBuffer = seLibrary->Load(path.c_str());
    Sound::path = path;
  }
}
