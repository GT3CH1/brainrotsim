//
// Created by GT3CH1 on 12/2/23.
//

#ifndef PHYSICSSIM_BEEPER_H
#define PHYSICSSIM_BEEPER_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <cmath>

class Beeper {
private:
    //Should there be sound right now
    bool soundOn = true;
    //Type of wave that should be generated
    int waveType = 0;
    //Tone that the wave will produce (may or may not be applicable based on wave type)
    float waveTone = 440;
    //Running index for sampling
    float samplingIndex = 0;

    //These are useful variables that cannot be changed outside of this file:
    //Volume
    const Sint16 amplitude = 32000;
    //Sampling rate
    const int samplingRate = 44100;
    //Buffer size
    const int bufferSize = 1024;

    //Samples a sine wave at a given index
    float sampleSine(float index);

    // Samples a triangle wave at a given index
    float sampleTriangle(float index);

    float sampleSawtooth(float index);
    
    //Samples a square wave at a given index
    float sampleSquare(float index);

public:
    //Initializes SDL audio, audio device, and audio specs
    void initializeAudio();

    //Function called by SDL audio_callback that fills stream with samples
    void generateSamples(short *stream, int length);

    //Turn sound on or off
    void setSoundOn(bool soundOnOrOff);

    //Set timbre of tone produced by beeper
    void setWaveType(int waveTypeID);

    //Set tone (in Hz) produced by beeper
    void setWaveTone(int waveHz);
};


#endif //PHYSICSSIM_BEEPER_H
