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

## Configuring for Web

    Install [emscripten](https://emscripten.org/docs/getting_started/downloads.html)

## Coding standard

1. Run clang-format
2. Keep it simple
3. Don't use exceptions

And best efforts:

1. Don't crash
2. Don't leak
