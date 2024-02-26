# QuickGL

A javascript runtime with (incomplete) tinygl and opencv bindings for experimenting with mixed reality.

<img width="533" alt="Screenshot 2024-02-06 at 10 16 26 PM" src="https://github.com/bnolan/quickgl/assets/17499/ae021156-da43-423e-a61c-1949bd63d72f">

## Limitations

- RGB565
- 480x480

## Configuring on MacOS

    brew install sdl2 clang-format cmake opencv
    cd third_party/quickjs && make
    cd third_party/tinygl && make
    mkdir build
    cd build
    cmake ..

## Build and run

    make && ./QuickGL

## Configure for wasm

1. install emscripten/emcc
2. clone opencv-4.9 sources
3. install python@3.11
4. `export EMSCRIPTEN=...`
5. Apply [build fix](https://github.com/opencv/opencv/issues/19493#issuecomment-857167996) to emscripten
5. `emcmake python ./opencv/platforms/js/build_js.py build_js``

(This expects that you have `/Users/you/Projects/build/opencv` and `/Users/you/Projects/build/build_js`)

## Build for wasm

    cd lab/emcc
    make
    make http
    open localhost:8000

## Coding standard

1. Run clang-format
2. Keep it simple
3. Don't use exceptions

And best efforts:

1. Don't crash
2. Don't leak
