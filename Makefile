
ifeq ($(PLATFORM), wasm)
    CXX = em++
    OUT = build/index.html
    LDFLAGS = -sUSE_SDL=3 -sALLOW_MEMORY_GROWTH=1 -lembind
else
    CXX = g++
    OUT = build/app
    LDFLAGS = -lSDL3
endif

CXXFLAGS = -O2
SRC = src/*.cpp

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) $(LDFLAGS) -o $(OUT)

clean:
	rm -f build/index.html build/index.js build/index.wasm build/app
