#include "soundDevice.h"

static SoundDevice* _instance = nullptr;

SoundDevice::SoundDevice()
{
  ALCDevice = alcOpenDevice(nullptr);  // nullptr = get default device
  if (!ALCDevice) throw("Failed to get sound device");

  ALCContext = alcCreateContext(ALCDevice, nullptr);  // create context
  if (!ALCContext) throw("Failed to set sound context");

  if (!alcMakeContextCurrent(ALCContext))  // make context current
    throw("Failed to make context current");

  const ALCchar* name = nullptr;

  if (alcIsExtensionPresent(ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
    name = alcGetString(ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
  if (!name || alcGetError(ALCDevice) != AL_NO_ERROR)
    name = alcGetString(ALCDevice, ALC_DEVICE_SPECIFIER);
  printf("Opened \"%s\"\n", name);
}

SoundDevice::~SoundDevice()
{
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(ALCContext);
  alcCloseDevice(ALCDevice);
}

SoundDevice* SoundDevice::Get()
{
  Init();
  return _instance;
}

void SoundDevice::Init()
{
  if (_instance == nullptr)
  {
    _instance = new SoundDevice();
  }
}

void SoundDevice::GetLocation(float& x, float& y, float& z)
{
  alGetListener3f(AL_POSITION, &x, &y, &z);
}

void SoundDevice::GetOrientation(float& ori)
{
  alGetListenerfv(AL_ORIENTATION, &ori);
}

float SoundDevice::GetGain()
{
  float curr_gain;
  alGetListenerf(AL_GAIN, &curr_gain);
  return curr_gain;
}

void SoundDevice::SetLocation(const float& x, const float& y, const float& z)
{
  alListener3f(AL_POSITION, x, y, z);
}

void SoundDevice::SetOrientation(const float& atx, const float& aty, const float& atz,
                                 const float& upx, const float& upy, const float& upz)
{
  std::vector<float> orientation;
  orientation.push_back(atx);
  orientation.push_back(aty);
  orientation.push_back(atz);
  orientation.push_back(upx);
  orientation.push_back(upy);
  orientation.push_back(upz);
  alListenerfv(AL_ORIENTATION, orientation.data());
}

void SoundDevice::SetGain(const float& val)
{
  // clamp between 0 and 5
  float newVol = val;
  if (newVol < 0.f)
    newVol = 0.f;
  else if (newVol > 5.f)
    newVol = 5.f;

  alListenerf(AL_GAIN, newVol);
}

///#define AL_INVERSE_DISTANCE                      0xD001
///#define AL_INVERSE_DISTANCE_CLAMPED              0xD002
///#define AL_LINEAR_DISTANCE                       0xD003
///#define AL_LINEAR_DISTANCE_CLAMPED               0xD004
///#define AL_EXPONENT_DISTANCE                     0xD005
///#define AL_EXPONENT_DISTANCE_CLAMPED             0xD006
void SoundDevice::SetAttunation(int key)
{
  if (key < 0xD001 || key > 0xD006) throw("bad attunation key");

  alListeneri(AL_DISTANCE_MODEL, key);
}
