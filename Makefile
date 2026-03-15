CXX = em++
SRC = src/*.cpp
OUT = build/index.html

# Emscripten flags
CXXFLAGS = -O2
LDFLAGS = -sUSE_SDL=3 -sALLOW_MEMORY_GROWTH=1

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) $(LDFLAGS) -o $(OUT)

clean:
	rm -f build/index.html build/index.js build/index.wasm
