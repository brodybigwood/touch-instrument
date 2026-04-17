#pragma once

class Synth {
public:
    virtual int noteOn(float) = 0;
    virtual void noteOff(int) = 0;
    virtual void pitchBend(int, float) = 0;
};

#ifdef __EMSCRIPTEN__
#include <emscripten/val.h>
class JSSynth : public Synth {
    emscripten::val synth;
public:
    JSSynth();

    int noteOn(float) override;
    void noteOff(int) override;
    void pitchBend(int, float) override;
};
#else
#include <SDL3/SDL.h>
#include <vector>
#include <map>
#include <cmath>

struct Voice {
    float pitch;
    float phase = 0.0f;
    float gain = 0.0f;
    float targetGain = 0.0f;
    float filterState = 0.0f;
    bool active = false;
};

class CSynth : public Synth {
public:
CSynth();
    ~CSynth();
    int noteOn(float pitch);
    void noteOff(int id);
    void pitchBend(int id, float bendAmount);
private:
    void process();

    SDL_AudioStream* stream;
    std::map<int, Voice> notes;
    int nextId = 1;
    int sampleRate;
    int bufferSize = 0;

    std::vector<float> buffer;
    std::vector<float> delayBuffer;
    int delayPtr = 0;

    float midiToHz(float midi) { return 440.0f * powf(2.0f, (midi - 69.0f) / 12.0f); }
};
#endif
