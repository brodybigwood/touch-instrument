#include "synth.h"
#include <iostream>

#ifdef __EMSCRIPTEN__
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
#else
CSynth::CSynth() {
    SDL_AudioSpec desired;
    desired.format = SDL_AUDIO_F32;
    desired.channels = 1;
    desired.freq = 48000;

    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &desired, NULL, NULL);
    if (!stream) {
        SDL_Log("Failed to open audio: %s", SDL_GetError());
        return;
    }

    auto audio_callback = [](void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount) {
        CSynth* synth = (CSynth*)userdata;
        synth->bufferSize = additional_amount / sizeof(float);
        if (synth->buffer.size() != synth->bufferSize) synth->buffer.resize(synth->bufferSize);
        synth->process();
        SDL_PutAudioStreamData(stream, synth->buffer.data(), synth->bufferSize * sizeof(float));
    };
    SDL_SetAudioStreamGetCallback(stream, audio_callback, this);
 
    SDL_AudioSpec src_spec;
    SDL_GetAudioStreamFormat(stream, &src_spec, NULL);
    sampleRate = src_spec.freq;

    delayBuffer.resize(sampleRate * 0.5f, 0.0f); 
    SDL_ResumeAudioStreamDevice(stream);
}

int CSynth::noteOn(float pitch) {
    int id = nextId++;
    Voice v;
    v.pitch = pitch;
    v.targetGain = 0.8f;
    v.active = true;
    notes[id] = v;
    return id;
}

void CSynth::noteOff(int id) {
    if (notes.count(id)) {
        notes[id].targetGain = 0.0f;
        notes[id].active = false;
    }
}

void CSynth::pitchBend(int id, float bendAmount) {
    if (notes.count(id)) {
        notes[id].pitch += bendAmount;
    }
}

void CSynth::process() {
    for (int i = 0; i < bufferSize; i++) {
        float sample = 0.0f;

        for (auto it = notes.begin(); it != notes.end();) {
            Voice& v = it->second;
            
            float freq = 2 * midiToHz(v.pitch);
            v.phase += freq / sampleRate;
            if (v.phase > 1.0f) v.phase -= 2.0f;
            float raw = v.phase;

            float rc = 1.0f / (2.0f * M_PI * 900.0f);
            float dt = 1.0f / sampleRate;
            float alpha = dt / (rc + dt);
            v.filterState = v.filterState + alpha * (raw - v.filterState);

            v.gain += (v.targetGain - v.gain) * 0.01f;
            
            sample += v.filterState * v.gain;

            if (!v.active && v.gain < 0.001f) it = notes.erase(it);
            else ++it;
        }

        sample *= 0.2f; // masterGain

        float delayOut = delayBuffer[delayPtr];
        
        float feedbackAlpha = 0.1f; // approximation of 800Hz RC
        float filteredDelay = delayOut * 0.45f; 
        
        delayBuffer[delayPtr] = sample + filteredDelay;
        delayPtr = (delayPtr + 1) % delayBuffer.size();

        buffer[i] = sample + (delayOut * 0.3f);
    }
}
#endif
