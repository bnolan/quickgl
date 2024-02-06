# QuickGL

A javascript runtime with (incomplete) tinygl and opencv bindings for experimenting with mixed reality.

<img width="566" alt="Screenshot 2024-02-06 at 9 08 04 PM" src="https://github.com/bnolan/quickgl/assets/17499/2b099ffe-1d4a-4ce5-a8e1-fd51f8430596">


## Configuring on MacOS

    brew install sdl2
    brew install sdl2_ttf
    brew install clang-format
    cd third_party/quickjs && make
    cd third_party/tinygl && make
    mkdir build
    cd build
    cmake ..

## Build and run

    make && ./QuickGL

## Coding standard

1. Run clang-format
2. Keep it simple
3. Don't use exceptions
4. Don't crash
5. Don't leak
