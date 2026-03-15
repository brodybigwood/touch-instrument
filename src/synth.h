#pragma once

class Synth {
public:
    virtual int noteOn(float) = 0;
    virtual void noteOff(int) = 0;
    virtual void pitchBend(int, float) = 0;
};


#include <emscripten/val.h>
class JSSynth : public Synth {
    emscripten::val synth;
public:
    JSSynth();

    int noteOn(float) override;
    void noteOff(int) override;
    void pitchBend(int, float) override;
};
