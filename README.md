## Setup

make sure you have emscripten (em++) and SDL3 installed

mkdir build, then run the makefile, it will compile the cpp code to wasm inside build/ and make the wasm script (index.js)

index.html will automatically access the generated script and also load a custom js synth from my website. you just have to serve the build folder and index.html on your own server.

demo is available at brodybigwood.com/pages/touch-instrument

## Synth

you can change the synth to something else such as tone.js, but you have to modify it or use a helper to fit the function calls in synth.cpp
