#include "beeper.h"

void fillBuffer(void* userdata, Uint8* _stream, int len)
{
    short* stream = reinterpret_cast<short*>(_stream);
    int length = len;
    Beeper* beeper = (Beeper*)userdata;

    beeper->generateSamples(stream, length);
}

void Beeper::initializeAudio()
{
    SDL_AudioSpec desired, returned;
    SDL_AudioDeviceID devID;

    SDL_zero(desired);
    desired.freq = samplingRate;
    desired.format = AUDIO_S16SYS; //16-bit audio
    desired.channels = 1;
    desired.samples = bufferSize;
    desired.callback = &fillBuffer;
    desired.userdata = this;

    devID = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(0, 0), 0, &desired, &returned, 0);
    SDL_PauseAudioDevice(devID, 0);
}

void Beeper::generateSamples(short* stream, int length)
{
    int samplesToWrite = length / sizeof(short);
    for (int i = 0; i < samplesToWrite; i++)
    {
        if (soundOn)
        {
            switch (waveType)
            {
            case 0:
                {
                    stream[i] = (short)(amplitude * sampleSine(samplingIndex));
                    break;
                }
            case 1:
                {
                    stream[i] = (short)(amplitude * sampleSquare(samplingIndex));
                    break;
                }
            case 2:
                {
                    stream[i] = (short)(amplitude * sampleTriangle(samplingIndex));
                    break;
                }
            case 3:
                {
                    stream[i] = (short)(amplitude * sampleSawtooth(samplingIndex));
                    break;
                }
            default:
                {
                    stream[i] = 0;
                    break;
                }
            }
        }
        else
        {
            stream[i] = 0;
        }
        //INFO << "Sampling index: " << samplingIndex;
        samplingIndex += (waveTone * M_PI * 2) / samplingRate;
        //INFO << "Stream input: " << stream[i];
        if (samplingIndex >= (M_PI * 2))
        {
            samplingIndex -= M_PI * 2;
        }
    }
}

void Beeper::setSoundOn(bool soundOnOrOff)
{
    soundOn = soundOnOrOff;
    //if (soundOnOrOff) {
    //  samplingIndex = 0;
    //}
}

void Beeper::setWaveType(int waveTypeID)
{
    waveType = waveTypeID;
    //samplingIndex = 0;
}

void Beeper::setWaveTone(float waveHz)
{
    waveTone = waveHz;
    //samplingIndex = 0;
}

float Beeper::sampleSine(float index)
{
    double result = sin((index));
    //INFO << "Sine result: " << result;
    return result;
}

float Beeper::sampleTriangle(float index)
{
    double res = 12 * (asin(sin(index * M_PI / (3)))) / M_PI;
    return res;
}

float Beeper::sampleSawtooth(float index)
{
    return 2 * (index / 2 - floor(0.5f + index / 2));
}

float Beeper::sampleSquare(float index)
{
    int unSquaredSin = sin((index));
    if (unSquaredSin >= 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}
