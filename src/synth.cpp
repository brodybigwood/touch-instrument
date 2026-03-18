#include "synth.h"
#include <iostream>

JSSynth::JSSynth() : synth(emscripten::val::global("synth")) {}

int JSSynth:: noteOn(float pitch) {
    emscripten::val obj = emscripten::val::object();
    obj.set("type", "noteOn");
    obj.set("pitch", pitch);

    return synth.call<int>("handleNote", obj);
}

void JSSynth::noteOff(int id) {
    emscripten::val obj = emscripten::val::object();
    obj.set("type", "noteOff");
    obj.set("id", id);
    synth.call<void>("handleNote", obj);
}

void JSSynth::pitchBend(int id, float bendAmount) {
    emscripten::val obj = emscripten::val::object();
    obj.set("type", "pitchBend");
    obj.set("id", id);
    obj.set("bendAmount", bendAmount);
    synth.call<void>("handleNote", obj);
}

